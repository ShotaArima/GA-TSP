#include "Colony.h"

int main()
{
	int i, dataNo;
	char fname[256];
	Colony *colony;
	char dataName[DATA_NUM][16] = {"berlin52", "eil51", "eil76", "kroA100", "test"};

	srand((unsigned int)time(NULL));

	printf("実行するデータの番号を入力してください．\n");
	for(i = 0; i < DATA_NUM; i++) {
		printf("%d : %s\n", i + 1, dataName[i]);
	}
	scanf("%d", &dataNo);
	sprintf(fname, "%s.tsp", dataName[dataNo - 1]);
	colony = new Colony(fname);
	for(i = 1; i <= REPEAT_NUM; i++) {
		colony->selectRoute(i);
		colony->renewPheromone();
	}
	colony->printPheromone();
	colony->printRoute();
	delete colony;

	return 0;
}

