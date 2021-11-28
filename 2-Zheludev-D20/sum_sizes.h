#pragma once

enum {
	FILE_ERROR = -100,
	READING_ERROR,
	ALLOCATION_ERROR,
	INITIALIZATION_ERROR
} errors;

int SumSizes(uint* numberTouchedElements, long long* numberCombinations);

int FindSubset(plentyElem_t* plenty, stack_t* curSolution, uint N, uint requiredAmount, uint curIndex, long long* numberCombinations);

void PrintSolution(stack_t* stack, FILE* fp);
