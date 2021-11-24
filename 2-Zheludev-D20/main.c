#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
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

	Test results:
	Number of elements: 40.000.000
	Time: 35s
	Memory: 313 MB
*/

void StressTest() {
	FILE* fp = fopen("input.txt", "wt");
	if (fp == NULL)
		return;

	int N = (int)4e7;
	srand(time(NULL));
	int B = rand();

	fprintf(fp, "%d\n%d\n", B, N);
	for (int i = 0; i < N; i++)
		fprintf(fp, "%d ", 1 + rand());

	fprintf(fp, "\n");
	fclose(fp);
	
	int time = clock();
	SumSizes();
	time = (clock() - time) / 1000;

	fprintf(stdout, "Stress test\nNumber of elements of the set: %d\nExecution time: %d sec", N, time);
}

int main(void) {
	StressTest();
	return 0;
}
