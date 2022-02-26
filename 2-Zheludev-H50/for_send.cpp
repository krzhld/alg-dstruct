#include <stdio.h>
#include <stdlib.h>

#ifndef FALSE
#define FALSE 0
#endif
#ifndef TRUE
#define TRUE 1
#endif

struct node_AA_t_tag {
	int value;
	int level;
	struct node_AA_t_tag* left;
	struct node_AA_t_tag* right;
};

typedef struct node_AA_t_tag node_AA_tree_t;

static node_AA_tree_t* bottom = NULL;
static node_AA_tree_t* deleted = NULL;
static node_AA_tree_t* last = NULL;

void AATree_Init(void) {
	if (bottom == NULL) {
		bottom = (node_AA_tree_t*)malloc(sizeof(node_AA_tree_t));
		if (bottom == NULL)
			fprintf(stderr, "Allocation node error!\n");
		else {
			bottom->level = 0;
			bottom->left = bottom;
			bottom->right = bottom;
			deleted = bottom;
		}
	}
}

node_AA_tree_t* AATree_CreateNode(int value, int level, node_AA_tree_t* left, node_AA_tree_t* right) {
	node_AA_tree_t* resultNode = (node_AA_tree_t*)malloc(sizeof(node_AA_tree_t));
	if (resultNode == NULL)
		fprintf(stderr, "Allocation node error!\n");
	else {
		resultNode->value = value;
		resultNode->level = level;
		resultNode->left = left;
		resultNode->right = right;
	}
	return resultNode;
}

node_AA_tree_t* AATree_Skew(node_AA_tree_t* node) {
	if (node->left->level == node->level) {
		node_AA_tree_t* tempNode = node;
		node = node->left;
		tempNode->left = node->right;
		node->right = tempNode;
	}
	return node;
}

node_AA_tree_t* AATree_Split(node_AA_tree_t* node) {
	if (node->level == node->right->right->level) {
		node_AA_tree_t* tempNode = node;
		node = node->right;
		tempNode->right = node->left;
		node->left = tempNode;
		++node->level;
	}
	return node;
}

node_AA_tree_t* AATree_Insert(int value, node_AA_tree_t* node) {
	if (node == bottom || node == NULL)
		node = AATree_CreateNode(value, 1, bottom, bottom);
	if (value < node->value)
		node->left = AATree_Insert(value, node->left);
	else if (value > node->value)
		node->right = AATree_Insert(value, node->right);
	node = AATree_Skew(node);
	node = AATree_Split(node);
	return node;
}

node_AA_tree_t* AATree_Delete(int delValue, node_AA_tree_t* node) {
	if (node != bottom && node != NULL) {
		last = node;
		if (delValue < node->value)
			node->left = AATree_Delete(delValue, node->left);
		else {
			deleted = node;
			node->right = AATree_Delete(delValue, node->right);
		}
		if (node == last && deleted != bottom && delValue == deleted->value) {
			deleted->value = node->value;
			deleted = bottom;
			node = node->right;
			free(last);
		}
		else if ((node->left->level < node->level - 1) || (node->right->level < node->level - 1)) {
			--node->level;
			if (node->right->level > node->level)
				node->right->level = node->level;
			node = AATree_Skew(node);
			node->right = AATree_Skew(node->right);
			node->right->right = AATree_Skew(node->right->right);
			node = AATree_Split(node);
			node->right = AATree_Split(node->right);
		}
	}
	return node;
}

int AATree_Search(int value, node_AA_tree_t* node) {
	node_AA_tree_t* curNode = node;
	while (curNode != NULL) {
		if (curNode == bottom)
			return FALSE;
		if (curNode->value == value)
			return TRUE;
		else if (curNode->value < value)
			curNode = curNode->right;
		else
			curNode = curNode->left;
	}
	return FALSE;
}

void AATree_Free(node_AA_tree_t* node) {
	if (node == bottom || node == NULL)
		return;
	AATree_Free(node->left);
	AATree_Free(node->right);
	free(node);
}

int main(void) {
	int value;
	char command;
	node_AA_tree_t* node = NULL;
	AATree_Init();
	while (scanf("%c %d", &command, &value) >= 1) {
		switch (command) {
		case 'a':
			node = AATree_Insert(value, node);
			break;
		case 'r':
			node = AATree_Delete(value, node);
			break;
		case 'f':
			if (AATree_Search(value, node))
				puts("yes");
			else
				puts("no");
			break;
		}
	}
}
