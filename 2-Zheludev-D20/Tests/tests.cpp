#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include "gtest/gtest.h"
#include "..\sum_sizes.h"
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
	int res = SumSizes();
	EXPECT_TRUE(res == READING_ERROR);
}

TEST(SumSizesTest, HandleNotCompletelyInitialization) {
	int plenty[] = { 1, 2, 3 };
	int sizePlenty = 3;

	int N = 2; // N < sizePlenty
	int B = 6;

	PrintToInputFile(plenty, sizePlenty, N, B);
	int res = SumSizes();
	EXPECT_TRUE(res == INITIALIZATION_ERROR);
}

TEST(SumSizesTest, HandleZeroCodeError_AnswerFullPlenty) {
	int plenty[] = { 1, 2, 3 };
	int sizePlenty = 3;

	int N = 3;
	int B = 6;

	PrintToInputFile(plenty, sizePlenty, N, B);
	int res = SumSizes();
	EXPECT_TRUE(res == 0);

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
	int res = SumSizes();
	EXPECT_TRUE(res == 0);

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
	int res = SumSizes();
	EXPECT_TRUE(res == 0);

	FILE* fp = fopen("output.txt", "rt");
	char str[100];
	fgets(str, 100, fp);
	EXPECT_TRUE(strcmp(str, "0") == 0);
}

TEST(SumSizesTest, CorrectDataHandleZeroCodeError_AnswerTwoSubsets) {
	int plenty[] = { 1, 2, 3 };
	int sizePlenty = 3;

	int N = 3;
	int B = 3;

	PrintToInputFile(plenty, sizePlenty, N, B);
	int res = SumSizes();
	EXPECT_TRUE(res == 0);

	FILE* fp = fopen("output.txt", "rt");
	char str[100];
	fgets(str, 100, fp);
	EXPECT_TRUE(strcmp(str, "1 2 ") == 0 || strcmp(str, "3 ") == 0);
}
