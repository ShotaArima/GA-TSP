#include "Colony.h"

// コンストラクタ
// fileName: データセットのファイル名
Colony::Colony(char *fileName)
{
	int i;

	field = new Field(fileName);
	ant = new Ant* [ANT_NUM];
	for(i = 0; i < ANT_NUM; i++) {
		ant[i] = new Ant(this);
	}
	nume = new double* [field->nodeNum];
	for(i = 0; i < field->nodeNum; i++) {
		nume[i] = new double [field->nodeNum];
	}
	optAnt = new Ant(this);
	optAnt->totalDis = DBL_MAX;
	optRep = -1;
}

// デストラクタ
Colony::~Colony()
{
	int i;

	for(i = 0; i < ANT_NUM; i++) {
		delete ant[i];
	}
	delete [] ant;
	for(i = 0; i < field->nodeNum; i++) {
		delete [] nume[i];
	}
	delete [] nume;
	delete field;
	delete optAnt;
}

// 経路を選択する
void Colony::selectRoute(int rep)
{
	int i, j;
	
	// 確率の分子を算出する
/*
	iを0からfield->nodeNum-1まで1ずつ増やしながら以下を繰り返す．
		jを1からi-1まで1ずつ増やしながら以下を繰り返す．
			nume[i][j]にfield->pheromone[j][i]のPHERO_L乗とfield->distance[i][j]の逆数のHEU_L乗の積を代入する．
		jをi+1からfield->nodeNum-1まで1ずつ増やしながら以下を繰り返す．
			nume[i][j]にfield->pheromone[i][j]のPHERO_L乗とfield->distance[i][j]の逆数のHEU_L乗の積を代入する．
*/

	// 経路を選択する
/*
	iを0からANT_NUM-1まで1ずつ増やしながら以下を繰り返す．
		ant[i]でselectRoute()を呼び出す．
		ant[i]->totalDisがoptAnt->totalDis未満なら以下を実行する．
			optAntでcopy(ant[i])を呼び出す．
			optRepにrepを代入する．
*/
}

// フェロモン量を更新する
void Colony::renewPheromone()
{
	int i, j;

	// 蒸発させる
/*
	iを0からfield->nodeNum-1まで1ずつ増やしながら以下を繰り返す．
		jをi+1からfield->nodeNum-1まで1ずつ増やしながら以下を繰り返す．
			field->pheromone[i][j]に1-EVA_Rをかける．
*/

	// アリによる追加分を加算する
/*
	iを0からANT_NUM-1まで1ずつ増やしながら以下を繰り返す．
		ant[i]でputPheromone()を呼び出す．
*/
}

// フェロモン量を表示する
void Colony::printPheromone()
{
	int i, j;

	for(i = 0; i < field->nodeNum; i++) {
		for(j = 0; j < field->nodeNum; j++) {
			printf("%8.3f", field->pheromone[i][j]);
		}
		printf("\n");
	}
}

// 経路を表示する
void Colony::printRoute()
{
	int i;
	Ant* optAnt2;

	printf("Best Route\n");
	for(i = 0; i < field->nodeNum; i++) {
		printf("%d ", optAnt->route[i] + 1);
	}
	printf("\n");
	printf("Total Distance : %f\n", optAnt->totalDis);
	printf("Repeat : %d\n", optRep);

	optAnt2 = new Ant(this);
	optAnt2->selectOptRoute();
	printf("Final Route\n");
	for(i = 0; i < field->nodeNum; i++) {
		printf("%d ", optAnt2->route[i] + 1);
	}
	printf("\n");
	printf("Total Distance : %f\n", optAnt2->totalDis);
	delete optAnt2;
}