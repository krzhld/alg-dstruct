#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>
#include "AABinTree.h"
#include "BinTree.h"

#define INIT_TIMER \
    LARGE_INTEGER frequency; \
    LARGE_INTEGER start,end; \
    double result; \
    QueryPerformanceFrequency(&frequency)

#define TIMER_START QueryPerformanceCounter(&start)

#define TIMER_STOP \
    QueryPerformanceCounter(&end); \
    result =(float)(end.QuadPart-start.QuadPart)/frequency.QuadPart

#define MAX_NUMBER_OF_VALUES 1000

int main(void) {
    node_AA_tree_t* AATree = NULL;
    AATree_Init();

    node_tree_t* bTree = NULL;

    srand(time(NULL));
    FILE* file = fopen("compare.txt", "wt");
    float resultAA, resultB;
    fprintf(file, "%s", "insert bTree;insert AATree\n");
    if (file == NULL) {
        fprintf(stderr, "Opening file error!\n");
        return 1;
    }
    INIT_TIMER;
    for (int i = 0; i < MAX_NUMBER_OF_VALUES; i++) {
        TIMER_START;
        bTree = Tree_Insert(rand(), bTree);
        TIMER_STOP;
        resultB = result;

        TIMER_START;
        AATree = AATree_Insert(rand(), AATree);
        TIMER_STOP;
        resultAA = result;
        fprintf(file, "%.8f;%.8f\n", resultB, resultAA);
    }
    fprintf(file, "%s", "delete bTree; delete AATree\n");
    for (int i = 0; i < MAX_NUMBER_OF_VALUES; i++) {
        TIMER_START;
        bTree = Tree_Delete(rand(), bTree);
        TIMER_STOP;
        resultB = result;

        TIMER_START;
        AATree = AATree_Delete(rand(), AATree);
        TIMER_STOP;
        resultAA = result;
        fprintf(file, "%.8f;%.8f\n", resultB, resultAA);
    }
    
    fclose(file);
    return 0;
}
