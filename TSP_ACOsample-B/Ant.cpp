#include "Ant.h"

// コンストラクタ
// argColony: 属しているコロニー
Ant::Ant(Colony *argColony)
{
	colony = argColony;
	route = new int [colony->field->nodeNum];
	candidate = new int [colony->field->nodeNum];
	route[0] = 0;
	candidate[0] = 0;
	totalDis = 0.0;
}

// デストラクタ
Ant::~Ant()
{
	delete [] route;
	delete [] candidate;
}

// コピーする
// org: コピー元の蟻へのポインタ
void Ant::copy(Ant *org)
{
	int i;

	for(i = 0; i < colony->field->nodeNum; i++) {
		route[i] = org->route[i];
	}
	totalDis = org->totalDis;
}

// 経路を選択する
void Ant::selectRoute()
{
	int i, j, next, next2;
	double denom, r, prob;

	// 未訪問ノードを初期化する
/*
	candidate[1]～candidate[colony->field->nodeNum-1]を1にする．
*/

	// 経路を選択する
/*
	totalDisを0にする．
	iを0からcolony->field->nodeNum-3まで1ずつ増やしながら以下を繰り返す．
*/

		// 確率の分母を算出する
/*
		denomを0にする．
		jを1からcolony->field->nodeNum-1まで1ずつ増やしながら以下を繰り返す．
			candidate[j]が1ならdenomにcolony->nume[route[i]][j]を加える．
*/

		// 次のノードを選択する
/*
		nextを-1にする．
*/

		// フェロモン量に基づいて選択する
/*
		denomが0.0でなく，オブジェクト形式マクロを使って発生させた0～1の実数乱数がPHERO_R以下だったら以下を実行する．
			rに0～1の実数の乱数を代入する（オブジェクト形式マクロを活用する）．
			nextを1からcolony->field->nodeNum-1まで1ずつ増やしながら以下を繰り返す．
				candidate[next]が1なら以下を実行する．
					probにcolony->nume[route[i]][next] / denomの計算結果を代入する．
					rがprob以下なら繰返しから抜ける．
					rからprobを引く．
			nextがcolony->field->nodeNumと等しかったらnextに-1を代入する．
*/

		// ランダムに選択する
/*
		nextが-1なら以下を実行する．
			next2に0～colony->field->nodeNum-i-2の乱数を代入する．
			nextを1からcolony->field->nodeNum-2まで1ずつ増やしながら以下を繰り返す．
				candidate[next]が1なら以下を実行する．
					next2が0なら繰返しから抜け，そうでなければnext2から1を引く．
		route[i+1]にnextを代入する．
		candidate[next]に0を代入する．
		totalDisにcolony->field->distance[route[i]][next]を加える．
*/

	// 最後の1ノードを探索する
/*
	nextを1からcolony->field->nodeNum-1まで1ずつ増やしながら以下を繰り返す．
		candidate[next]が1なら繰返しから抜ける．
	route[colony->field->nodeNum-1]にnextを代入する．
	totalDisに colony->field->distance[route[colony->field->nodeNum-2]][next]を加える．
*/

	// 出発地点への距離を加算する
/*
	totalDisにcolony->field->distance[next][route[0]]を加える．
*/
}

// 最適経路を選択する
void Ant::selectOptRoute()
{
	int i, j, next;
	double maxPheromone;

	// 未訪問ノードを初期化する
	for(i = 1; i < colony->field->nodeNum; i++) {
		candidate[i] = 1;
	}

	// 経路を選択する
	totalDis = 0.0;
	for(i = 0; i < colony->field->nodeNum - 2; i++) {
		// 次のノードを選択する
		next = -1;
		maxPheromone = -1.0;
		for(j = 0; j < route[i]; j++) {
			if((candidate[j] == 1) && (maxPheromone < colony->field->pheromone[j][route[i]])) {
				next = j;
				maxPheromone = colony->field->pheromone[j][route[i]];
			}
		}
		for(j++; j < colony->field->nodeNum; j++) {
			if((candidate[j] == 1) && (maxPheromone < colony->field->pheromone[route[i]][j])) {
				next = j;
				maxPheromone = colony->field->pheromone[route[i]][j];
			}
		}
		route[i + 1] = next;
		candidate[next] = 0;
		totalDis += colony->field->distance[route[i]][next];
	}
	// 最後の1ノードを探索する
	for(next = 1; next < colony->field->nodeNum; next++) {
		if(candidate[next] == 1) {
			break;
		}
	}
	route[colony->field->nodeNum - 1] = next;
	totalDis += colony->field->distance[route[colony->field->nodeNum - 2]][next];

	// 出発地点への距離を加算する
	totalDis += colony->field->distance[next][0];
}

// フェロモンを分泌する
void Ant::putPheromone()
{
	int i;
	double p;

/*
	pにPHERO_Q/totalDisを代入する．
	iを0からcolony->field->nodeNum-2まで1ずつ増やしながら以下を繰り返す．
		route[i]がroute[i + 1]より小さいならcolony->field->pheromone[route[i]][route[i+1]]にpを加え，そうでないならcolony->field->pheromone[route[i+1]][route[i]]にpを加える．
	colony->field->pheromone[0][route[colony->field->nodeNum-1]]にpを加える．
*/
}
