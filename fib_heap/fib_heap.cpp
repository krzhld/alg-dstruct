#include "fib_heap.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

node_t* __createNode(int value) {
	node_t* newNode = (node_t*)malloc(sizeof(node_t));
	if (newNode == NULL) {
		fprintf(stderr, "Allocation memory error!\n");
		exit(EXIT_FAILURE);
	}
	newNode->degree = 0;
	newNode->value = value;
	newNode->father = NULL;
	newNode->left = newNode;
	newNode->right = newNode;
	newNode->child = NULL;
	return newNode;
}

void __joinNodes(node_t* node1, node_t* node2) {
	node_t* degreeNode = node1;
	node2->father = node1;
	int num = 0;
	if (node1->degree == 0)
		node1->child = node2;
	else {
		for (int i = 0; i < node1->degree; i++) {
			node_t* buf2 = node2->right;
			node2->right = node1->child->right;
			node1->child->right->left = node2;
			buf2->left = node1->child;
			node1->child->right = buf2;
			for (int j = 0; j < num; j++)
				node2 = node2->left;
			node2 = node2->child;
			node1 = node1->child;
			for (int j = 0; j < num + 1; j++)
				node1 = node1->left;
			num++;
		}
	}
	degreeNode->degree++;
}

void __compaction(fibHeap_t* heap) {
	if (heap->min != NULL) {
		int d = 0;
		int edge = (int)trunc(log2(heap->count)) + 1;
		node_t* y = NULL;
		int nBuf = heap->count;
		node_t** arr = (node_t**)malloc(edge * sizeof(node_t*));
		for (int i = 0; i < edge; i++)
			arr[i] = NULL;
		node_t* tmp = heap->min;
		node_t* tmpBuf = tmp->right;
		if (tmp != NULL) {
			while (tmp->left != tmp) {
				tmp->left->right = tmp->right;
				tmp->right->left = tmp->left;
				tmp->left = tmp;
				tmp->right = tmp;
				d = tmp->degree;
				while (arr[d] != NULL) {
					y = arr[d];
					if (tmp->value > y->value) {
						node_t* buf = y;
						y = tmp;
						tmp = buf;
					}
					__joinNodes(tmp, y);
					arr[d] = NULL;
					d++;
				}
				arr[d] = tmp;
				tmp = tmpBuf;
				tmpBuf = tmp->right;
			}
			d = tmp->degree;
			while (arr[d] != NULL) {
				y = arr[d];
				if (tmp->value > y->value) {
					node_t* buf = y;
					y = tmp;
					tmp = buf;
				}
				__joinNodes(tmp, y);
				arr[d] = NULL;
				d++;
			}
			arr[d] = tmp;
		}
		heap->min = NULL;
		for (int i = 0; i < edge; i++) {
			if (arr[i] != NULL) {
				fibHeapAdd(heap, arr[i]);
				if (heap->min == NULL || heap->min->value > arr[i]->value)
					heap->min = arr[i];
			}
		}
		heap->count = nBuf;
	}
}

fibHeap_t* fibHeapInit(void) {
	fibHeap_t* newHeap = (fibHeap_t*)malloc(sizeof(fibHeap_t));
	if (newHeap == NULL) {
		fprintf(stderr, "Allocation memory error!\n");
		exit(EXIT_FAILURE);
	}
	newHeap->count = 0;
	newHeap->min = NULL;
	return newHeap;
}

void fibHeapAdd(fibHeap_t* heap, node_t* addingNode) {
	if (heap->min == NULL) {
		heap->min = addingNode;
		heap->count = 1;
	}
	else {
		node_t* buf = heap->min->right;
		heap->min->right = addingNode;
		addingNode->left = heap->min;
		addingNode->right = buf;
		buf->left = addingNode;
		heap->count += 1;
		if (heap->min->value > addingNode->value)
			heap->min = addingNode;
	}
}

fibHeap_t* fibHeapUnion(fibHeap_t* heap1, fibHeap_t* heap2) {
	fibHeap_t* resultHeap = fibHeapInit();
	if (heap1->min == NULL) {
		resultHeap->min = heap2->min;
		resultHeap->count = heap2->count;
		free(heap1);
		free(heap2);
		return resultHeap;
	}
	if (heap2->min == NULL) {
		resultHeap->min = heap1->min;
		resultHeap->count = heap1->count;
		free(heap1);
		free(heap2);
		return resultHeap;
	}
	resultHeap->min = heap1->min;
	node_t* buf = resultHeap->min->right;
	resultHeap->min->right = heap2->min;
	heap2->min->left->right = buf;
	buf->left = heap2->min->left;
	heap2->min->left = resultHeap->min;
	resultHeap->count = heap1->count + heap2->count;
	if (heap1->min->value > heap2->min->value)
		resultHeap->min = heap2->min;
	free(heap1);
	free(heap2);
	int edge = 4 * (int)trunc(log2(resultHeap->count)) + 1;
	if (resultHeap->count > edge)
		__compaction(resultHeap);
	return resultHeap;
}

node_t* fibHeapGetMin(fibHeap_t* heap) {
	node_t* minNode = heap->min;
	if (minNode != NULL) {
		node_t* tmp = minNode->child;
		if (tmp != NULL) {
			while (tmp != minNode->child->left) {
				fibHeapAdd(heap, tmp);
				tmp->father = NULL;
				tmp = tmp->right;
			}
			fibHeapAdd(heap, tmp);
			heap->count -= 1;
			tmp->father = NULL;
		}
		if (minNode->right == minNode)
			heap->min = NULL;
		else {
			minNode->right->left = minNode->left;
			minNode->left->right = minNode->right;
			heap->min = minNode->right;
			minNode->right = minNode;
			minNode->left = minNode;
			__compaction(heap);
		}
		heap->count -= 1;
	}
	return minNode;
}

int main(int argc, char* argv[]) {
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}
