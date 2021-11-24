#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include "sum_sizes.h"
#include "stack.h"

int SumSizes() {
	FILE* fp = fopen("input.txt", "rt");
	if (fp == NULL) {
		fprintf(stderr, "Opening file error!\n");
		return FILE_ERROR;
	}

	int B, N;
	if (fscanf(fp, "%d\n%d", &B, &N) != 2) {
		fprintf(stderr, "Reading B and N error!\n");
		return READING_ERROR;
	}

	plentyElem_t* plenty = (plentyElem_t*)malloc(N * sizeof(plentyElem_t));
	if (plenty == NULL) {
		fprintf(stderr, "Allocation memory error!\n");
		return ALLOCATION_ERROR;
	}
	for (int i = 0; i < N; i++) {
		if (fscanf(fp, "%d", &plenty[i].size) != 1) {
			fprintf(stderr, "Reading plenty error!\n");
			return READING_ERROR;
		}
		plenty[i].isUsed = FALSE;
	}

	int checkEndFile;
	if (fscanf(fp, "%d", &checkEndFile) != -1) {
		fprintf(stderr, "Plenty hasn't been initialized completely\n");
		return INITIALIZATION_ERROR;
	}

	fclose(fp);

	stack_t* curSolution = StackInit(N);

	FindSubset(plenty, curSolution, N, B);

	fp = fopen("output.txt", "wt");
	if (fp == NULL) {
		fprintf(stderr, "Opening file error!\n");
		return FILE_ERROR;
	}
	
	PrintSolution(curSolution, fp);
	FreeStack(curSolution);
	fclose(fp);

	return 0;
}

int FindSubset(plentyElem_t* plenty, stack_t* curSolution, int N, int requiredAmount) {
	static int returnRes = 0;
	for (int i = 0; i < N; i++) {
		if (plenty[i].size < requiredAmount && plenty[i].isUsed == FALSE) {
			StackPush(curSolution, plenty[i].size, i);
			plenty[i].isUsed = TRUE;
			requiredAmount -= plenty[i].size;
			returnRes = FindSubset(plenty, curSolution, N, requiredAmount);
			if (returnRes == 0)
				return 0;
			requiredAmount += returnRes;
		}
		else {
			if (plenty[i].size == requiredAmount && plenty[i].isUsed == FALSE) {
				StackPush(curSolution, plenty[i].size, i);
				plenty[i].isUsed = TRUE;
				requiredAmount -= plenty[i].size; // for breakpoint
				return 0;
			}
		}
	}
	if (requiredAmount == 0)
		return 0;
	stackElem_t delElem = StackTop(curSolution);
	requiredAmount += delElem.size;
	plenty[delElem.index].isUsed = FALSE;
	StackPop(curSolution);
	return delElem.size;
}

void PrintSolution(stack_t* stack, FILE* fp) {
	if (stack->cur == -1) {
		fprintf(fp, "0");
		return;
	}	
	for (int i = 0; i <= stack->cur; i++)
		fprintf(fp, "%d ", stack->arr[i].size);
}
