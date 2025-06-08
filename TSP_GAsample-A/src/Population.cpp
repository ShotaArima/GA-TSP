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

// p1: 親個体1の添字
// p2: 親個体2の添字
// c1: 子個体1の添字
// c2: 子個体2の添字
void Population::crossover(int p1, int p2, int c1, int c2)
{
	if (crossoverMethod == "pmx") {
        crossoverPMX(p1, p2, c1, c2);
    } else if (crossoverMethod == "ox") {
        crossoverOX(p1, p2, c1, c2);
    } else if (crossoverMethod == "one_point") {
        crossoverOnePoint(p1, p2, c1, c2);
    } else if (crossoverMethod == "two_point") {
        crossoverTwoPoint(p1, p2, c1, c2);
    } else if (crossoverMethod == "uniform") {
        crossoverUniform(p1, p2, c1, c2);
    } else {
        std::cerr << "Unsupported crossover method: " << crossoverMethod << std::endl;
        std::exit(1);
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

// OX:順序交叉
void Population::crossoverOX(int p1, int p2, int c1, int c2) {
    int point1 = rand() % (field->nodeNum - 1);
    int point2;
    do {
        point2 = rand() % (field->nodeNum - 1);
    } while (point1 == point2);
    if (point1 > point2) std::swap(point1, point2);

    // 初期化
    bool used1[field->nodeNum] = {};
    bool used2[field->nodeNum] = {};

    // 中央部をコピー
    for (int i = point1; i <= point2; i++) {
        nextInd[c1]->chrom[i] = ind[p1]->chrom[i];
        used1[ind[p1]->chrom[i]] = true;
        nextInd[c2]->chrom[i] = ind[p2]->chrom[i];
        used2[ind[p2]->chrom[i]] = true;
    }

    // 外側を親2から順に埋める（循環）
    int idx1 = (point2 + 1) % field->nodeNum;
    int fill1 = (point2 + 1) % field->nodeNum;
    for (int i = 0; i < field->nodeNum; i++) {
        int gene = ind[p2]->chrom[(point2 + 1 + i) % field->nodeNum];
        if (!used1[gene]) {
            nextInd[c1]->chrom[fill1] = gene;
            fill1 = (fill1 + 1) % field->nodeNum;
        }
    }

    int idx2 = (point2 + 1) % field->nodeNum;
    int fill2 = (point2 + 1) % field->nodeNum;
    for (int i = 0; i < field->nodeNum; i++) {
        int gene = ind[p1]->chrom[(point2 + 1 + i) % field->nodeNum];
        if (!used2[gene]) {
            nextInd[c2]->chrom[fill2] = gene;
            fill2 = (fill2 + 1) % field->nodeNum;
        }
    }
}

// 部分写像交叉でind[p1]とind[p2]からnextInd[c1]とnextInd[c2]を生成する．
void Population::crossoverPMX(int p1, int p2, int c1, int c2) {
    int point1, point2, tmp, i;

    int used1[field->nodeNum-1] = {0}, used2[field->nodeNum-1] = {0};

    point1 = rand() % (field->nodeNum - 2 + 1);
    do {
        point2 = rand() % (field->nodeNum - 2 + 1);
    } while(point1 == point2);
    if (point1 > point2) std::swap(point1, point2);

    for (i = point1 + 1; i <= point2 - 1; i++) {
        nextInd[c1]->chrom[i] = ind[p2]->chrom[i];
        nextInd[c2]->chrom[i] = ind[p1]->chrom[i];
    }

    for (i = 0; i <= point1; i++) {
        nextInd[c1]->chrom[i] = resolvePMXGene(ind[p1]->chrom, ind[p2]->chrom, point1, point2, i);
        nextInd[c2]->chrom[i] = resolvePMXGene(ind[p2]->chrom, ind[p1]->chrom, point1, point2, i);
    }
    for (i = point2; i < field->nodeNum; i++) {
        nextInd[c1]->chrom[i] = resolvePMXGene(ind[p1]->chrom, ind[p2]->chrom, point1, point2, i);
        nextInd[c2]->chrom[i] = resolvePMXGene(ind[p2]->chrom, ind[p1]->chrom, point1, point2, i);
    }
}


// 一点交叉
void Population::crossoverOnePoint(int p1, int p2, int c1, int c2) {
    int point = rand() % field->nodeNum;

    bool used1[field->nodeNum] = {};
    bool used2[field->nodeNum] = {};

    for (int i = 0; i < point; i++) {
        nextInd[c1]->chrom[i] = ind[p1]->chrom[i];
        used1[ind[p1]->chrom[i]] = true;
        nextInd[c2]->chrom[i] = ind[p2]->chrom[i];
        used2[ind[p2]->chrom[i]] = true;
    }

    int idx1 = point, idx2 = point;
    for (int i = 0; i < field->nodeNum; i++) {
        int gene1 = ind[p2]->chrom[i];
        int gene2 = ind[p1]->chrom[i];

        if (!used1[gene1]) nextInd[c1]->chrom[idx1++] = gene1;
        if (!used2[gene2]) nextInd[c2]->chrom[idx2++] = gene2;
    }
}

// 2点交叉
void Population::crossoverTwoPoint(int p1, int p2, int c1, int c2) {
    int point1 = rand() % field->nodeNum;
    int point2;
    do {
        point2 = rand() % field->nodeNum;
    } while (point1 == point2);
    if (point1 > point2) std::swap(point1, point2);

    bool used1[field->nodeNum] = {};
    bool used2[field->nodeNum] = {};

    for (int i = point1; i <= point2; i++) {
        nextInd[c1]->chrom[i] = ind[p1]->chrom[i];
        used1[ind[p1]->chrom[i]] = true;
        nextInd[c2]->chrom[i] = ind[p2]->chrom[i];
        used2[ind[p2]->chrom[i]] = true;
    }

    int idx1 = (point2 + 1) % field->nodeNum;
    int idx2 = (point2 + 1) % field->nodeNum;
    for (int i = 0; i < field->nodeNum; i++) {
        int gene1 = ind[p2]->chrom[(point2 + 1 + i) % field->nodeNum];
        int gene2 = ind[p1]->chrom[(point2 + 1 + i) % field->nodeNum];

        if (!used1[gene1]) {
            nextInd[c1]->chrom[idx1] = gene1;
            idx1 = (idx1 + 1) % field->nodeNum;
        }
        if (!used2[gene2]) {
            nextInd[c2]->chrom[idx2] = gene2;
            idx2 = (idx2 + 1) % field->nodeNum;
        }
    }
}

// 一様交叉
void Population::crossoverUniform(int p1, int p2, int c1, int c2) {
    bool used1[field->nodeNum] = {};
    bool used2[field->nodeNum] = {};
    int temp1[field->nodeNum], temp2[field->nodeNum];

    for (int i = 0; i < field->nodeNum; i++) {
        if (rand() % 2) {
            temp1[i] = ind[p1]->chrom[i];
            used1[temp1[i]] = true;
            temp2[i] = ind[p2]->chrom[i];
            used2[temp2[i]] = true;
        } else {
            temp1[i] = -1;
            temp2[i] = -1;
        }
    }

    for (int i = 0; i < field->nodeNum; i++) {
        if (temp1[i] == -1) {
            for (int j = 0; j < field->nodeNum; j++) {
                if (!used1[ind[p2]->chrom[j]]) {
                    temp1[i] = ind[p2]->chrom[j];
                    used1[temp1[i]] = true;
                    break;
                }
            }
        }
        if (temp2[i] == -1) {
            for (int j = 0; j < field->nodeNum; j++) {
                if (!used2[ind[p1]->chrom[j]]) {
                    temp2[i] = ind[p1]->chrom[j];
                    used2[temp2[i]] = true;
                    break;
                }
            }
        }
    }

    for (int i = 0; i < field->nodeNum; i++) {
        nextInd[c1]->chrom[i] = temp1[i];
        nextInd[c2]->chrom[i] = temp2[i];
    }
}
