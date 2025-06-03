#pragma once

// 標準ヘッダのインクルード
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <float.h>
#include <math.h>

// 定数の定義
#define REPEAT_NUM	1000	// 繰返し数
#define ANT_NUM		100		// 蟻の数
#define PHERO_Q		100		// 1回の巡回で分泌するフェロモン量
#define EVA_R		0.3		// フェロモンの蒸発率
#define PHERO_R		0.95	// フェロモンに基づいて経路を選択する確率
#define PHERO_L		1		// フェロモンを考慮する度合い
#define HEU_L		1		// ヒューリスティック情報を考慮する度合い
#define LINE_LEN	1024	// ファイル読込み時の配列変数の長さ
#define DATA_NUM	5		// データ数

// 0以上1以下の実数乱数
#define RAND_01 ((double)rand() / RAND_MAX)

class Field
{
public:
	Field(char *fileName);
	~Field();

	int nodeNum;		// ノード数
	double **distance;	// ノード間距離
	double **pheromone;	// エッジのフェロモン量
};

