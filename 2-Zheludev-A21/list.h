#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define SUCCESS 0
#define OPEN_FILE_ERROR -1
#define MEMORY_ALLOCATION_ERROR -2
#define SIMILAR_KEYS_ERROR -3
#define MAX_LENGTH 65

#define SWAP_INT(a, b) {int t; t = a; a = b; b = t;}
#define SWAP_STR(a, b) {char t[MAX_LENGTH]; strcpy(t, a); strcpy(a, b); strcpy(b, t);}

typedef struct node_t{
	int key;
	char word[MAX_LENGTH];
	struct node_t* next;
} node_t;

typedef struct {
	node_t* head;
} list_t;

list_t* CreateList(void);

node_t* FormNode(int key, char* word);

void AddNodeToEmptyList(list_t* list, node_t* node);

void AddNodeAfter(node_t* node, node_t* newNode);

void AddNodeBefore(node_t* node, node_t* newNode);

void PrintList(list_t* list);

FILE* OpenFileForReading_s(const char* path);

int FormSortedList(list_t* list, const char* filePath);

void FreeList(list_t* list);

bool IsWordInList(list_t* list);
