#pragma once

#ifndef TRUE
#define TRUE 1
#endif // !TRUE

#ifndef FALSE
#define FALSE !(TRUE)
#endif // !FALSE

#define MAX_BUFF_SIZE 32
#define WAS -2

typedef char boolean;

int** CreateAdjacencyMatrix(FILE* fptr, int* numberVertexes);

boolean IsThereSuchVertex(int* graph, int size, int vertex);

void BFS(FILE* fptr);
