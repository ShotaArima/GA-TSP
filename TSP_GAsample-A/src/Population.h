#pragma once

#include "Individual.h"
#include <string>
#include <cstdio>

class Individual;
class Field;

class Population
{
public:
    Population(Field* argField,
		const std::string& datasetName,
		const std::string& selectionMethod,
		const std::string& crossoverMethod,
		int popSize,
		double mutateRate);

	~Population();
	void alternate();		// 世代交代をする
	void printRoute();		// 経路を表示する

	static Field* field;
	Individual **ind; // 現世代の個体群

private:
    int rankingSelect();
    int rouletteSelect();
    int tournamentSelect();

    Individual** nextInd;

    int* used1;
    int* used2;
    FILE* log_fp;

    std::string selectionMethod;
    std::string crossoverMethod;
    int popSize;
    double mutateRate;

    void evaluate();
    int selectParent();
    void crossover(int p1, int p2, int c1, int c2);
    int resolvePMXGene(const int* parent1, const int* parent2, int point1, int point2, int i);
    void sort(int lb, int ub);

	double* trFit;    // 適応度正規化用（ルーレット選択用）
    double denom;     // trFitの合計
};