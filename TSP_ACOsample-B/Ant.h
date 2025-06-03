#pragma once

#include "Field.h"
#include "Colony.h"
class Colony;

class Ant
{
public:
	Ant(Colony *argColony);
	~Ant();
	void copy(Ant *org);	// コピーする
	void selectRoute();		// 経路を選択する
	void selectOptRoute();	// 最適経路を選択する
	void putPheromone();	// フェロモンを分泌する

	Colony *colony;		// 属しているコロニー
	int *route;			// 経路
	double totalDis;	// 総移動距離

private:
	int *candidate;		// 未訪問ノード
};
