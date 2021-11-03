#define _CRT_SECURE_NO_WARNINGS
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "stressTest.h"
#include "graph.h"

inline int GetRand() {
	return ((rand() % 7) * 1e3 + (rand() % 10) * 1e2 + (rand() % 10) * 1e1 + (rand() % 10));
}

void StressTest() {
	FILE* fptr = fopen("StressTest.txt", "w+");
	int numVertexes = 70000;
	fprintf(fptr, "%d\n", numVertexes);
	fprintf(fptr, "0 1\n");
	srand(time(NULL));
	for (int i = 1; i < numVertexes; i++) {
		fprintf(fptr, "%d %d\n", GetRand(), GetRand());
	}
	fseek(fptr, SEEK_SET, 0);
	clock_t start = clock();
	BFS(fptr);
	double time = (double)(clock() - start) / CLOCKS_PER_SEC;
	fprintf(stdout, "\n%f\n", time);
}
