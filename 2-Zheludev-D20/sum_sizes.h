#pragma once
#include "stack.h"

enum {
	FILE_ERROR = -100,
	READING_ERROR,
	ALLOCATION_ERROR,
	INITIALIZATION_ERROR
} errors;

#ifndef TRUE
#define TRUE 1
#endif // !TRUE

#ifndef FALSE
#define FALSE 0
#endif

typedef char boolean;

typedef struct {
	int size;
	boolean isUsed;
} plentyElem_t;

int SumSizes();

int FindSubset(plentyElem_t* plenty, stack_t* curSolution, int N, int requiredAmount);

void PrintSolution(stack_t* stack, FILE* fp);
