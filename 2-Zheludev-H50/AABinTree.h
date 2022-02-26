#pragma once
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

void AATree_Init(void);

node_AA_tree_t* AATree_CreateNode(int value, int level, node_AA_tree_t* left, node_AA_tree_t* right);

node_AA_tree_t* AATree_Skew(node_AA_tree_t* node);

node_AA_tree_t* AATree_Split(node_AA_tree_t* node);

node_AA_tree_t* AATree_Insert(int value, node_AA_tree_t* node);

node_AA_tree_t* AATree_Delete(int delValue, node_AA_tree_t* node);

int AATree_Search(int value, node_AA_tree_t* node);

void AATree_Print(node_AA_tree_t* node, int level);

void AATree_Free(node_AA_tree_t* node);
