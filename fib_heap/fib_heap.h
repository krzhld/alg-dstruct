#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <math.h>
#include <gtest/gtest.h>

typedef struct node_t {
	int value;
	int degree;
	struct node_t* father;
	struct node_t* child;
	struct node_t* left;
	struct node_t* right;
} node_t;

typedef struct {
	int count;
	node_t* min;
} fibHeap_t;

node_t* __createNode(int value);

void __joinNodes(node_t* node1, node_t* node2);

void __compaction(fibHeap_t* heap);

fibHeap_t* fibHeapInit(void);

void fibHeapAdd(fibHeap_t* heap, node_t* addingNode);

fibHeap_t* fibHeapUnion(fibHeap_t* heap1, fibHeap_t* heap2);

node_t* fibHeapGetMin(fibHeap_t* heap);
