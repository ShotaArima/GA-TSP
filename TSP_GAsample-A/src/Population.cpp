#include "Population.h"
#include <time.h>
#include <sys/stat.h>
#include <iostream>

Field* Population::field = NULL;

// コンストラクタ
Population::Population(Field* argField,
	const std::string& datasetName,
	const std::string& selectionMethod,
	const std::string& crossoverMethod,
	int popSize,
	double mutateRate)
    :	selectionMethod(selectionMethod),
		crossoverMethod(crossoverMethod),
		popSize(popSize),
		mutateRate(mutateRate)

{
	field = argField;
	// ログディレクトリ作成（なければ）
    mkdir("log", 0777);

	// ログファイル名生成
    char logname[256];
    sprintf(logname, "log/log_%s_%s_%s_%d_%.2f.csv",
        datasetName.c_str(),
        selectionMethod.c_str(),
        crossoverMethod.c_str(),
        popSize,
        mutateRate);

    log_fp = fopen(logname, "w");
    if (!log_fp) {
        std::cerr << "ログファイルのオープンに失敗しました\n";
        exit(1);
    }
    fprintf(log_fp, "Generation,BestDistance,ElapsedTime\n");

	// メモリ確保
    Individual::randArray = new int[field->nodeNum];
    used1 = new int[field->nodeNum];
    used2 = new int[field->nodeNum];
    ind = new Individual*[popSize];
    nextInd = new Individual*[popSize];

    for (int i = 0; i < popSize; i++) {
        ind[i] = new Individual(this);
        nextInd[i] = new Individual(this);
    }

    evaluate();
}

// デストラクタ
Population::~Population()
{

	for(int i = 0; i < POP_SIZE; i++) {
		delete ind[i];
		delete nextInd[i];
	}
	delete [] ind;
	delete [] nextInd;
	delete [] Individual::randArray;
	delete [] used1;
	delete [] used2;

	if (log_fp != NULL) {
        fclose(log_fp);
        log_fp = NULL;
    }
}

// 世代交代
void Population::alternate()
{
	int p1, p2;
	Individual **tmp;
	static int generation = 0;
    clock_t start = clock();  // 開始時間の記録

	// ルーレット選択のための処理
	/*
	denom = 0.0;
	for(int i = 0; i < POP_SIZE; i++) {
		trFit[i] = (ind[POP_SIZE - 1]->fitness - ind[i]->fitness) / (ind[POP_SIZE - 1]->fitness - ind[0]->fitness);
		denom += trFit[i];
	}
	*/

	// エリート保存戦略で子個体を作る
	for(int i = 0; i < ELITE; i++) {
		for(int j = 0; j < field->nodeNum; j++) {
			nextInd[i]->chrom[j] = ind[i]->chrom[j];
		}
	}

	// 親を選択し，交叉して子個体を作り，突然変異を起こす
	for(int i = ELITE; i< popSize - 1; i += 2) {
		int p1 = rankingSelect();
		int p2 = rankingSelect();
		
		crossover(p1, p2, i, i + 1);
		
		nextInd[i]->mutate();
		nextInd[i + 1]->mutate();
	}

	// 半端が出たらランダムに生成する
	if(popSize %2 != 0) {
		nextInd[popSize -1]->setChrom();
	}

	// 次世代を現世代に変更する
	tmp = ind;
	ind = nextInd;
	nextInd = tmp;

	// 評価する
	evaluate();

	// === ログ出力処理 ===
    clock_t end = clock();
    double elapsed = (double)(end - start) / CLOCKS_PER_SEC;
    fprintf(log_fp, "%d,%f,%f\n", generation, ind[0]->fitness, elapsed);
    generation++;
}

// すべての個体を評価して，適応度順に並び替える
void Population::evaluate()
{
	int i;

	for(i = 0; i < POP_SIZE; i++) {
		ind[i]->evaluate();
	}
	sort(0, POP_SIZE - 1);
}

// ランキング選択で親個体を1つ選択する
// 戻り値: 選択した親個体の添え字
int Population::rankingSelect()
{
	int num, denom, r;

	denom = POP_SIZE*(POP_SIZE+1)/2;
	r = rand()%(denom -1 +1)+ 1;
    for(num = POP_SIZE ; num <=1; num--){
        if (r <= num) {
            break;
        }
        r -= num;
    }
	return POP_SIZE - num;
}

// ルーレット選択で親個体を1つ選択する
// 戻り値: 選択した親個体の添え字
int Population::rouletteSelect()
{
	int rank;
	double prob, r;

	r = rand()/(double)RAND_MAX;
	for(rank = 1; rank < POP_SIZE-1; rank++) {
	    prob = trFit[rank-1]/denom;
	    if (r <= prob) {
	        break;
	    }
	    r -= prob;
	}
	return rank - 1;
}

// トーナメント選択で親個体を1つ選択する
// 戻り値: 選択した親個体の添え字
int Population::tournamentSelect()
{
	int i, ret = -1, num = 0, r;
	double bestFit = DBL_MAX;
	int tmp[POP_SIZE] = {0};

    while(1) {
        r = rand()%(POP_SIZE-1 + 1);
        if (tmp[r] == 0) {
            tmp[r] = 1;
            if (ind[r]->fitness < bestFit) {
                ret = r;
                bestFit = ind[r]->fitness;
            }
            num += 1;
            if (num==TOURNAMENT_SIZE) {
                break;
            }
        }
    };
	return ret;
}

// 部分写像交叉でind[p1]とind[p2]からnextInd[c1]とnextInd[c2]を生成する．
// p1: 親個体1の添字
// p2: 親個体2の添字
// c1: 子個体1の添字
// c2: 子個体2の添字
void Population::crossover(int p1, int p2, int c1, int c2)
{
	int point1, point2, tmp, i, j, key;

	// used1, used2の初期化
	int used1[field->nodeNum-1] = {0}, used2[field->nodeNum-1] = {0};

	// 交叉点の選択
	point1 = rand()%(field->nodeNum-2 + 1);
	do{
	    point2 = rand()%(field->nodeNum-2 + 1);
	} while(point1 == point2);
	if (point1 > point2) {
	    tmp = point1;
	    point1 = point2;
	    point2 = tmp;
	}

	// 交叉点間のコピー
	for(i = point1+1; i <= point2-1; i++) {
	    nextInd[c1]->chrom[i]=ind[p2]->chrom[i];
        nextInd[c2]->chrom[i]=ind[p1]->chrom[i];
	}

	// 交叉点外のコピー
	for (i = 0; i <= point1; i++) {
        nextInd[c1]->chrom[i] = resolvePMXGene(ind[p1]->chrom, ind[p2]->chrom, point1, point2, i); // 処理A
	    nextInd[c2]->chrom[i] = resolvePMXGene(ind[p2]->chrom, ind[p1]->chrom, point1, point2, i); // 処理B
	}
    for (i = point2; i < field->nodeNum; i++) {
         nextInd[c1]->chrom[i] = resolvePMXGene(ind[p1]->chrom, ind[p2]->chrom, point1, point2, i);
         nextInd[c2]->chrom[i] = resolvePMXGene(ind[p2]->chrom, ind[p1]->chrom, point1, point2, i);
    }
}

// 処理A, Bの関数
int Population::resolvePMXGene(const int* parent1, const int* parent2, int point1, int point2, int i) {
    int key = parent1[i];
    while (true) {
        int j;
        for (j = point1 + 1; j <= point2 - 1; ++j) {
            if (key == parent2[j]) {
                break;
            }
        }
        if (j == point2) break;
        key = parent1[j];
    }
    return key;
}

// ind[lb]～ind[ub]をクイックソートで並び替える
// lb: 並び替えの対象要素の添え字の下限
// ub: 並び替えの対象要素の添え字の上限
void Population::sort(int lb, int ub)
{
	int i, j, k;
	double pivot;
	Individual *tmp;

	if(lb < ub) {
		k = (lb + ub) / 2;
		pivot = ind[k]->fitness;
		i = lb;
		j = ub;
		do {
			while(ind[i]->fitness < pivot) {
				i++;
			}
			while(ind[j]->fitness > pivot) {
				j--;
			}
			if(i <= j) {
				tmp = ind[i];
				ind[i] = ind[j];
				ind[j] = tmp;
				i++;
				j--;
			}
		} while(i <= j);
		sort(lb, j);
		sort(i, ub);
	}
}

// 経路を表示する
void Population::printRoute()
{
	int i;

	for(i = 0; i < field->nodeNum; i++) {
		printf("%d ", ind[0]->chrom[i] + 1);
	}
	printf("\n");
	printf("Total Distance : %f\n", ind[0]->fitness);
}
