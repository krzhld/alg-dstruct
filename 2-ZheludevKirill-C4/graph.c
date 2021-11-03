#define _CRT_SECURE_NO_WARNINGS
#include <stdlib.h>
#include <stdio.h>
#include "graph.h"

int** CreateAdjacencyMatrix(FILE* fptr, int* numberVertexes) {
	if (fscanf(fptr, "%d", numberVertexes) != 1)
		return NULL;
	fgetc(fptr);

	int** resultMatrix = (int**)malloc(*numberVertexes * sizeof(int*));
	if (resultMatrix == NULL) {
		fprintf(stderr, "memallocation error!");
		return NULL;
	}
	for (int i = 0; i < *numberVertexes; i++) {
		resultMatrix[i] = (int*)calloc(i + 1, sizeof(int));
		if (resultMatrix[i] == NULL) {
			fprintf(stderr, "memallocation error!");
			free(resultMatrix);
			return NULL;
		}
	}

	int vertex1, vertex2;
	while (fscanf(fptr, "%d %d", &vertex1, &vertex2) != EOF) {
		if (vertex1 > vertex2)
			resultMatrix[vertex1][vertex2] = 1;
		else
			resultMatrix[vertex2][vertex1] = 1;
	}
	return resultMatrix;
}

boolean IsThereSuchVertex(int* graph, int size, int vertex) {
	for (int i = 0; i < size; i++)
		if (graph[i] == vertex)
			return TRUE;
	return FALSE;
}

void BFS(FILE* fptr) {
	int numberVertexes = 0;
	int** matrix = CreateAdjacencyMatrix(fptr, &numberVertexes);
	int* graph = (int*)malloc(numberVertexes * sizeof(int));
	if (graph == NULL) {
		fprintf(stderr, "memallocation error!");
		return;
	}
	int posExpandVertex = 0;
	int posForNextVertex = 0;
	graph[posForNextVertex] = 0;
	++posForNextVertex;
	for (int i = 1; i < numberVertexes; i++) {
		if (matrix[i][0] == 1) {
			matrix[i][0] = WAS;
			graph[posForNextVertex] = i;
			++posForNextVertex;
		}
	}

	++posExpandVertex;
	if (posExpandVertex == posForNextVertex)
		fprintf(stdout, "%d ", graph[0]);

	do {
		for (int j = 0; j < graph[posExpandVertex]; j++) {
			if (matrix[graph[posExpandVertex]][j] == 1) {
				matrix[graph[posExpandVertex]][j] = WAS;
				if (!IsThereSuchVertex(graph, posForNextVertex, j)) {
					graph[posForNextVertex] = j;
					++posForNextVertex;
				}
			}
		}
		for (int i = graph[posExpandVertex]; i < numberVertexes; i++) {
			if (matrix[i][graph[posExpandVertex]] == 1) {
				matrix[i][graph[posExpandVertex]] = WAS;
				if (!IsThereSuchVertex(graph, posForNextVertex, i)) {
					graph[posForNextVertex] = i;
					++posForNextVertex;
				}
			}
		}
		++posExpandVertex;
	} while (posExpandVertex != posForNextVertex);

	for (int i = 0; i < posForNextVertex; i++)
		fprintf(stdout, "%d ", graph[i]);

	for (int i = 0; i < numberVertexes; i++)
		free(matrix[i]);
	free(matrix);
	free(graph);
}
