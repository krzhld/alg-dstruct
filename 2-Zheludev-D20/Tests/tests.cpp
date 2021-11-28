#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include "gtest/gtest.h"
#include "..\types.h"
#include "..\sum_sizes.c"
#include "..\stack.c"

void PrintToInputFile(int* plenty, int sizePlenty, int N, int B) {
	FILE* fp = fopen("input.txt", "wt");
	if (fp == NULL)
		return;

	fprintf(fp, "%d\n%d\n", B, N);
	for (int i = 0; i < sizePlenty; i++)
		fprintf(fp, "%d ", plenty[i]);

	fprintf(fp, "\n");
	fclose(fp);
}

TEST(SumSizesTest, HandleReadingOverBuffer) {
	int plenty[] = { 1, 2, 3 };
	int sizePlenty = 3;

	int N = 4; // N > sizePlenty
	int B = 6;
	PrintToInputFile(plenty, sizePlenty, N, B);
	uint numberTouchedElem = 0;
	long long numberCombinations = 0;
	int res = SumSizes(&numberTouchedElem, &numberCombinations);
	EXPECT_TRUE(res == READING_ERROR);
	EXPECT_EQ(numberTouchedElem, 0);
	EXPECT_EQ(numberCombinations, 0);
}

TEST(SumSizesTest, HandleNotCompletelyInitialization) {
	int plenty[] = { 1, 2, 3 };
	int sizePlenty = 3;

	int N = 2; // N < sizePlenty
	int B = 6;

	PrintToInputFile(plenty, sizePlenty, N, B);
	uint numberTouchedElem = 0;
	long long numberCombinations = 0;
	int res = SumSizes(&numberTouchedElem, &numberCombinations);
	EXPECT_TRUE(res == INITIALIZATION_ERROR);
	EXPECT_EQ(numberTouchedElem, 0);
	EXPECT_EQ(numberCombinations, 0);
}

TEST(SumSizesTest, HandleZeroCodeError_AnswerFullPlenty) {
	int plenty[] = { 1, 2, 3 };
	int sizePlenty = 3;

	int N = 3;
	int B = 6;

	PrintToInputFile(plenty, sizePlenty, N, B);
	uint numberTouchedElem = 0;
	long long numberCombinations = 0;
	int res = SumSizes(&numberTouchedElem, &numberCombinations);
	EXPECT_TRUE(res == 0);
	EXPECT_EQ(numberTouchedElem, 3); // 1 2 3
	EXPECT_EQ(numberCombinations, 3); // 1; 1 2; 1 2 3

	FILE* fp = fopen("output.txt", "rt");
	char str[100];
	fgets(str, 100, fp);
	EXPECT_TRUE(strcmp(str, "1 2 3 ") == 0);
}

TEST(SumSizesTest, CorrectDataHandleZeroCodeError_AnswerOneSubset) {
	int plenty[] = { 1, 2, 3 };
	int sizePlenty = 3;

	int N = 3;
	int B = 5;

	PrintToInputFile(plenty, sizePlenty, N, B);
	uint numberTouchedElem = 0;
	long long numberCombinations = 0;
	int res = SumSizes(&numberTouchedElem, &numberCombinations);
	EXPECT_TRUE(res == 0);
	EXPECT_EQ(numberTouchedElem, 3); // 1 2 3
	EXPECT_EQ(numberCombinations, 5); // 1; 1 2; 1 3; 2; 2 3

	FILE* fp = fopen("output.txt", "rt");
	char str[100];
	fgets(str, 100, fp);
	EXPECT_TRUE(strcmp(str, "2 3 ") == 0);
}

TEST(SumSizesTest, CorrectDataHandleZeroCodeError_AnswerNoSubset) {
	int plenty[] = { 1, 2, 3 };
	int sizePlenty = 3;

	int N = 3;
	int B = 7;

	PrintToInputFile(plenty, sizePlenty, N, B);
	uint numberTouchedElem = 0;
	long long numberCombinations = 0;
	int res = SumSizes(&numberTouchedElem, &numberCombinations);
	EXPECT_TRUE(res == 0);
	EXPECT_EQ(numberTouchedElem, 3); // 1 2 3
	EXPECT_EQ(numberCombinations, 7); // 1; 1 2; 1 2 3; 1 3; 2; 2 3; 3

	FILE* fp = fopen("output.txt", "rt");
	char str[100];
	fgets(str, 100, fp);
	EXPECT_TRUE(strcmp(str, "0 ") == 0);
}

TEST(SumSizesTest, CorrectDataHandleZeroCodeError_AnswerTwoSubsets) {
	int plenty[] = { 1, 2, 3 };
	int sizePlenty = 3;

	int N = 3;
	int B = 3;

	PrintToInputFile(plenty, sizePlenty, N, B);
	uint numberTouchedElem = 0;
	long long numberCombinations = 0;
	int res = SumSizes(&numberTouchedElem, &numberCombinations);
	EXPECT_TRUE(res == 0);
	EXPECT_TRUE(numberTouchedElem == 2); // 1 2
	EXPECT_TRUE(numberCombinations == 2); // 1; 1 2

	FILE* fp = fopen("output.txt", "rt");
	char str[100];
	fgets(str, 100, fp);
	EXPECT_TRUE(strcmp(str, "1 2 ") == 0);
}
