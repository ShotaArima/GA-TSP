#pragma once

#include "Ant.h"
class Ant;

class Colony
{
public:
	Colony(char* fileName);
	~Colony();
	void selectRoute(int rep);	// 経路を選択する
	void renewPheromone();		// フェロモン量を更新する
	void printPheromone();		// フェロモン量を表示する
	void printRoute();			// 経路を表示する

	Field *field;	// 採餌行動の場
	Ant **ant;		// コロニーのメンバ
	double **nume;	// 経路選択確率の分子
	Ant *optAnt;	// 最適経路を歩いたメンバのコピー
	int optRep;		// 最適経路を歩いたメンバがいたときの繰返し回数
};
