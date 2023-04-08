#include "fib_heap.h"

TEST(TEST_CREATE_NODE, CreateNode) {
	node_t* node = __createNode(1);
	EXPECT_EQ(node->degree, 0);
	EXPECT_EQ(node->value, 1);
	EXPECT_EQ(node->father, nullptr);
	EXPECT_EQ(node->child, nullptr);	EXPECT_EQ(node->right, node);
	EXPECT_EQ(node->left, node);
}

TEST(TEST_CREATE_HEAP, CreateHeap) {
	fibHeap_t* heap = fibHeapInit();
	EXPECT_EQ(heap->count, 0);
	EXPECT_EQ(heap->min, nullptr);
}

TEST(TEST_ADD_ELEMENT_TO_HEAP, AddElementToHeap) {
	fibHeap_t* heap = fibHeapInit();
	node_t* node = __createNode(0);
	fibHeapAdd(heap, node);
	node_t* arrNode[10] = { NULL };
	for (int i = 0; i < 10; i++) {
		arrNode[i] = __createNode(i + 1);
		fibHeapAdd(heap, arrNode[i]);
	}
	EXPECT_EQ(heap->count, 11);
	EXPECT_EQ(heap->min, node);
}

TEST(TEST_ADD_ELEMENT_TO_HEAP1, AddElementToHeap) {
	fibHeap_t* heap = fibHeapInit();
	node_t* node1 = __createNode(1);
	node_t* node2 = __createNode(3);
	node_t* node3 = __createNode(2);
	fibHeapAdd(heap, node1);
	EXPECT_EQ(heap->count, 1);
	EXPECT_EQ(heap->min, node1);
	EXPECT_EQ(heap->min->right, node1);
	fibHeapAdd(heap, node2);
	EXPECT_EQ(heap->count, 2);
	EXPECT_EQ(heap->min, node1);
	EXPECT_EQ(heap->min->right, node2);
	fibHeapAdd(heap, node3);
	EXPECT_EQ(heap->count, 3);
	EXPECT_EQ(heap->min, node1);
	EXPECT_EQ(heap->min->right, node3);
	EXPECT_EQ(heap->min->left, node2);
}

TEST(TEST_HEAPS_UNION, HeapsUnion) {
	fibHeap_t* heap1 = fibHeapInit();
	fibHeap_t* heap2 = fibHeapInit();
	node_t* node1 = __createNode(2);
	node_t* node2 = __createNode(1);
	node_t* node3 = __createNode(10);
	fibHeapAdd(heap1, node1);
	fibHeapAdd(heap1, node2);
	fibHeapAdd(heap2, node3);
	fibHeap_t* heap3 = fibHeapInit();
	heap3 = fibHeapUnion(heap1, heap2);
	EXPECT_EQ(heap3->count, 3);
	EXPECT_EQ(heap3->min, node2);
}

TEST(TEST_HEAPS_UNION1, HeapsUnion) {
	fibHeap_t* heap1 = fibHeapInit();
	fibHeap_t* heap2 = fibHeapInit();
	fibHeap_t* heap3 = fibHeapInit();
	node_t* node2 = __createNode(1);
	node_t* node3 = __createNode(3);
	fibHeapAdd(heap1, node2);
	fibHeapAdd(heap1, node3);
	node_t* arrNode[15] = { NULL };
	for (int i = 0; i < 10; i++) {
		arrNode[i] = __createNode(i + 4);
		fibHeapAdd(heap1, arrNode[i]);
	}
	heap3 = fibHeapUnion(heap1, heap2);
	EXPECT_EQ(heap3->count, 12);
	EXPECT_EQ(heap3->min->degree, 0);
	EXPECT_EQ(heap3->min->left->degree, 0);
}

TEST(TEST_EXTRACT_MIN, ExtractMin) {
	fibHeap_t* heap = fibHeapInit();
	node_t* node1 = __createNode(1);
	node_t* node2 = __createNode(2);
	node_t* node3 = __createNode(3);
	fibHeapAdd(heap, node1);
	fibHeapAdd(heap, node2);
	fibHeapAdd(heap, node3);
	EXPECT_EQ(heap->count, 3);
	EXPECT_EQ(heap->min, node1);
	fibHeapGetMin(heap);
	EXPECT_EQ(heap->count, 2);
	EXPECT_EQ(heap->min, node2);
}

TEST(TEST_EXTRACT_MIN1, ExtractMin) {
	fibHeap_t* heap = fibHeapInit();
	fibHeapGetMin(heap);
	EXPECT_EQ(heap->count, 0);
	EXPECT_EQ(heap->min, nullptr);
	node_t* node1 = __createNode(2);
	node_t* node2 = __createNode(1);
	node_t* node3 = __createNode(3);
	fibHeapAdd(heap, node1);
	fibHeapAdd(heap, node2);
	fibHeapAdd(heap, node3);
	EXPECT_EQ(heap->count, 3);
	EXPECT_EQ(heap->min, node2);
	EXPECT_EQ(heap->min->degree, 0);
	EXPECT_EQ(heap->min->child, nullptr);
	fibHeapGetMin(heap);
	EXPECT_EQ(heap->count, 2);
	EXPECT_EQ(heap->min, node1);
	EXPECT_EQ(heap->min->father, nullptr);
}

TEST(TEST_NODES_JOIN, JoinNodes) {
	node_t* arrNode[2] = { NULL };
	for (int i = 0; i < 2; i++)
		arrNode[i] = __createNode(i + 1);
	EXPECT_EQ(arrNode[0]->degree, 0);
	EXPECT_EQ(arrNode[0]->child, nullptr);
	EXPECT_EQ(arrNode[1]->father, nullptr);
	__joinNodes(arrNode[0], arrNode[1]);
	EXPECT_EQ(arrNode[0]->degree, 1);
	EXPECT_EQ(arrNode[0]->child, arrNode[1]);
	EXPECT_EQ(arrNode[1]->father, arrNode[0]);
}

TEST(TEST_NODES_JOIN1, JoinNodes) {
	node_t* arrNode[4] = { NULL };
	for (int i = 0; i < 4; i++)
		arrNode[i] = __createNode(i + 1);
	__joinNodes(arrNode[2], arrNode[3]);
	__joinNodes(arrNode[0], arrNode[2]);
	EXPECT_EQ(arrNode[0]->degree, 1);
	EXPECT_EQ(arrNode[0]->child->left, arrNode[2]);
	EXPECT_EQ(arrNode[2]->left, arrNode[2]);
	EXPECT_EQ(arrNode[0]->child->left->child, arrNode[3]);
	EXPECT_EQ(arrNode[0]->child->left->father, arrNode[0]);
}

TEST(TEST_CONSOLIDATE, Compaction) {
	fibHeap_t* heap = fibHeapInit();
	__compaction(heap);
	EXPECT_EQ(heap->min, nullptr);
	EXPECT_EQ(heap->count, 0);
	node_t* node = __createNode(2);
	fibHeapAdd(heap, node);
	__compaction(heap);
	EXPECT_EQ(heap->min, node);
	EXPECT_EQ(heap->count, 1);
}
