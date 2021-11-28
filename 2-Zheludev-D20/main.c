#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include "types.h"
#include "sum_sizes.h"

/*
	CPU:
	Intel(R) Core(TM) i5-1035G1 CPU @ 1.00GHz
	Base speed:	1.19 GHz
	Sockets:	1
	Cores:	4
	Logical processors:	8
	Virtualization:	Enabled
	L1 cache:	320 KB
	L2 cache:	2.0 MB
	L3 cache:	6.0 MB

	Memory:
	8.0 GB
	Speed:	2667 MHz
	Slots used:	2 of 2
	Form factor:	SODIMM
	Hardware reserved:	195 MB
	SSD:
	Capacity:	239 GB
	Formatted:	239 GB

	Stress test results (case of full search, when sum of elements is less than required sum):
	Number of elements of the set: 30
	Execution time: 76 sec
	Number of touched elements: 30
	Number of combinations: 1073741823
*/

/* 0 < B < 2 ^ 32 */
uint pseudoRandomB(void) {
	srand(time(NULL));
	double probability = ((double)rand() / (RAND_MAX + 1)); // 0 <= res < 1
	if (probability == 0)
		return 1;
	uint res = (uint)(probability * pow(2, 32));
	return res;
}

void StressTest() {
	FILE* fp = fopen("input.txt", "wt");
	if (fp == NULL)
		return;

	uint N = 30;
	uint B = pseudoRandomB();

	srand(time(NULL));
	fprintf(fp, "%u\n%u\n", B, N);
	for (uint i = 0; i < N; i++)
		fprintf(fp, "%d ", 1 + rand());

	fprintf(fp, "\n");
	fclose(fp);
	
	long long numberCombinations = 0;
	uint numberTouchedElements = 0;
	int time = clock();
	SumSizes(&numberTouchedElements, &numberCombinations);
	time = (clock() - time) / 1000;

	fprintf(stdout, "Stress test\nNumber of elements of the set: %u\nExecution time: %d sec\nNumber of touched elements: %u\nNumber of combinations: %lld\n",
		N, time, numberTouchedElements, numberCombinations);
}

int main(void) {
	StressTest();
	return 0;
}
