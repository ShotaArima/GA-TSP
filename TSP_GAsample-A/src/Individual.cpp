#include "Individual.h"

int* Individual::randArray = NULL;

// コンストラクタ
// argPop: 属している集団
Individual::Individual(Population *argPop)
{
	pop = argPop;
	chrom = new int [pop->field->nodeNum];
	setChrom();
}

// デストラクタ
Individual::~Individual()
{
	delete [] chrom;
}

// ランダムに染色体を決める
void Individual::setChrom()
{
	int i;

	chrom[0] = 0;
	for (i = 1; i <= pop->field->nodeNum-1; i++) {
	    chrom[i] = i;
	    randArray[i] = rand(); // randArray[i]に乱数を代入する．
	}
	sortRandArray(1, pop->field->nodeNum-1);
    fitness = -1.0;
}

// 適応度を算出する
void Individual::evaluate()
{
	int i;

	fitness = pop->field->distance[chrom[pop->field->nodeNum-1]][chrom[0]];
	for(i=0;i <= pop->field->nodeNum-2; i++) {
	    fitness += pop->field->distance[chrom[i]][chrom[i+1]];
	}
}

// 突然変異を起こす
void Individual::mutate()
{
	int i, r, tmp;

	for(i=1; i <= pop->field->nodeNum-1; i++) {
	    // オブジェクト形式マクロを活用して発生させた0～1の実数の乱数がMUTATE_PROB未満なら以下を実行する
	    if (rand()/(double)RAND_MAX < MUTATE_PROB) {
	        // rにiとは異なる1～pop->field->nodeNum-1の乱数を代入する．
	        do{
	            r = rand()%(pop->field->nodeNum-1 - 1 + 1) + 1;
	        } while(r == i);

	        // chrom[i]とchrom[r]を入れ替える．
            tmp = chrom[i];
            chrom[i] = chrom[r];
            chrom[r] = tmp;
	    }
	}
}

// randArray[lb]～randArray[ub]を並び替えつつ，chrom[lb]～chrom[ub]も並び替える
// lb: 並び替えの対象要素の添え字の下限
// ub: 並び替えの対象要素の添え字の上限
void Individual::sortRandArray(int lb, int ub)
{
	int mid, i, j, key, tmp;

	if(lb < ub) {
		mid = (lb + ub) / 2;
		key = randArray[mid];
		i = lb;
		j = ub;
		do {
			while(randArray[i] < key)
				i++;
			while(randArray[j] > key)
				j--;
			if(i <= j) {
				tmp = randArray[i];
				randArray[i] = randArray[j];
				randArray[j] = tmp;
				tmp = chrom[i];
				chrom[i] = chrom[j];
				chrom[j] = tmp;
				i++;
				j--;
			}
		} while(i <= j);
		sortRandArray(lb, j);
		sortRandArray(i, ub);
	}
}
