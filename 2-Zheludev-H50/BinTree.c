#include <stdio.h>
#include <stdlib.h>
#include "BinTree.h"

#define SWAP(a, b) {int temp = a; a = b; b = temp;}

node_tree_t* prevNode = NULL;

node_tree_t* Tree_Insert(int value, node_tree_t* node) {
	if (node == NULL) {
		node = (node_tree_t*)malloc(sizeof(node_tree_t));
		if (node == NULL) {
			fprintf(stderr, "Allocation node error!\n");
			return node;
		}
		node->value = value;
		node->left = NULL;
		node->right = NULL;
	}
	else if (value < node->value)
		node->left = Tree_Insert(value, node->left);
	else if (value > node->value)
		node->right = Tree_Insert(value, node->right);
	return node;
}

node_tree_t* Tree_Delete(int delValue, node_tree_t* node) {
	prevNode = NULL;
	if (Del(delValue, node) == NULL)
		node = NULL;
	return node;
}

node_tree_t* Del(int delValue, node_tree_t* node) {
	node_tree_t *curNode = node;
	while (curNode) {
		if (curNode->value < delValue) {
			prevNode = curNode;
			curNode = curNode->right;
		}
		else if (curNode->value > delValue) {
			prevNode = curNode;
			curNode = curNode->left;
		}
		else {
			if (curNode->left == NULL && curNode->right == NULL) {
				if (prevNode == NULL) {
					free(curNode);
					return NULL;
				}
				else if (prevNode->left == curNode)
					prevNode->left = NULL;
				else if (prevNode->right == curNode)
					prevNode->right = NULL;
				free(curNode);
				return node;
			}
			else if (curNode->right) {
				prevNode = curNode;
				node_tree_t* successor = curNode->right;
				while (successor->left) {
					prevNode = successor;
					successor = successor->left;
				}
				SWAP(curNode->value, successor->value);
				Del(successor->value, curNode->right);
				return node;
			}
			else {
				prevNode = curNode;
				node_tree_t* predecessor = curNode->left;
				while (predecessor->right) {
					prevNode = predecessor;
					predecessor = predecessor->right;
				}
				SWAP(curNode->value, predecessor->value);
				Del(predecessor->value, curNode->left);
				return node;
			}
		}
	}
	return node;
}

int Tree_Search(int value, node_tree_t* node) {
	node_tree_t* curNode = node;
	while (curNode != NULL) {
		if (curNode->value == value)
			return TRUE;
		else if (curNode->value < value)
			curNode = curNode->right;
		else
			curNode = curNode->left;
	}
	return FALSE;
}

void Tree_Print(node_tree_t* node, int level) {
	if (node) {
		Tree_Print(node->left, level + 1);
		for (int i = 0; i < level; i++)
			printf("    ");
		printf("%d\n", node->value);
		Tree_Print(node->right, level + 1);
	}
}

void Tree_Free(node_tree_t* node) {
	if (node == NULL)
		return;
	Tree_Free(node->left);
	Tree_Free(node->right);
	free(node);
}
