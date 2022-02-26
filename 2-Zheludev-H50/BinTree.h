#pragma once
#ifndef FALSE
#define FALSE 0
#endif
#ifndef TRUE
#define TRUE 1
#endif

struct node_t_tag {
	int value;
	struct node_t_tag* left;
	struct node_t_tag* right;
};

typedef struct node_t_tag node_tree_t;

node_tree_t* Tree_Insert(int value, node_tree_t* node);

node_tree_t* Del(int delValue, node_tree_t* node);

node_tree_t* Tree_Delete(int delValue, node_tree_t* node);

int Tree_Search(int value, node_tree_t* node);

void Tree_Print(node_tree_t* node, int level);

void Tree_Free(node_tree_t* node);
