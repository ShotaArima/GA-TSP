// === TSP_GA.cpp ===
#include "Population.h"
#include "Field.h"

#include <string>
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <fstream>
#include <sstream>
#include <cmath>

// #define GEN_MAX 500
#define MAXNODE 200  // 必要に応じて最大ノード数を調整

Field* loadField(const std::string& filename) {
    Field* field = new Field((char*)(std::string("TSP_data/" + filename + ".tsp").c_str()));
    return field;
}

int main(int argc, char* argv[])
{
    if (argc < 6) {
        printf("Usage: %s [dataset] [selection] [crossover] [popSize] [mutateRate]\n", argv[0]);
        printf("  dataset   : berlin52 / eil51 / eil76 / kroA100 / test\n");
        printf("  selection : ranking / roulette / tournament\n");
        printf("  crossover : pmx / ox\n");
        printf("  popSize   : 集団サイズ（例：100）\n");
        printf("  mutateRate: 突然変異率（例：0.01）\n");
        return 1;
    }

    std::string dataset = argv[1];
    std::string selection = argv[2];
    std::string crossover = argv[3];
    int popSize = atoi(argv[4]);
    double mutateRate = atof(argv[5]);

    std::srand(static_cast<unsigned int>(std::time(NULL)));

    Field* field = new Field((char*)(std::string("TSP_data/" + dataset + ".tsp").c_str()));
    Population* pop = new Population(field, dataset, selection, crossover, popSize, mutateRate);

    for (int gen = 1; gen <= GEN_MAX; gen++) {
        pop->alternate();
    }
    pop->printRoute();

    delete pop;
    delete field;
    return 0;
}
