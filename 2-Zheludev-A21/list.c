#include "list.h"

list_t* CreateList(void) {
	list_t* resultList = (list_t*)malloc(sizeof(list_t));
	if (resultList == NULL) {
		printf("Allocation memory error!");
		exit(MEMORY_ALLOCATION_ERROR);
	}
	else
		resultList->head = NULL;
	return resultList;
}

node_t* FormNode(int key, char* word) {
	node_t* resultNode = (node_t*)malloc(sizeof(node_t));
	if (resultNode == NULL) {
		printf("Allocation memory error!");
		exit(MEMORY_ALLOCATION_ERROR);
	}
	else {
		resultNode->key = key;
		strcpy(resultNode->word, word);
		resultNode->next = NULL;
	}
	return resultNode;
}

void AddNodeToEmptyList(list_t* list, node_t* node) {
	list->head = node;
}

void AddNodeAfter(node_t* node, node_t* newNode) {
	newNode->next = node->next;
	node->next = newNode;
}

void AddNodeBefore(node_t* node, node_t* newNode) {
	newNode->next = node->next;
	node->next = newNode;
	SWAP_INT(node->key, newNode->key);
	SWAP_STR(node->word, newNode->word);
}

void PrintList(list_t* list) {
	node_t* curNode = list->head;
	while (curNode != NULL) {
		printf("%d\t%s\n", curNode->key, curNode->word);
		curNode = curNode->next;
	}
}

FILE* OpenFileForReading_s(const char* path) {
	FILE* fp = fopen(path, "rt");
	if (fp == NULL) {
		printf("Opening file error!");
		exit(OPEN_FILE_ERROR);
	}
	return fp;
}

int FormSortedList(list_t* list, const char* filePath) {
	FILE* filePointer = OpenFileForReading_s(filePath);
	int curKey;
	char curWord[MAX_LENGTH];
	node_t* curNode = NULL;
	node_t* iterNode = NULL;
	if (fscanf(filePointer, "%s\t%d\n", curWord, &curKey) != EOF) {
		curNode = FormNode(curKey, curWord);
		AddNodeToEmptyList(list, curNode);
	}
	while (fscanf(filePointer, "%s\t%d\n", curWord, &curKey) != EOF) {
		iterNode = list->head;
		curNode = FormNode(curKey, curWord);
		while (true) {
			if (curNode->key == iterNode->key) {
				printf("Keys must not be similar!");
				return SIMILAR_KEYS_ERROR;
			}
			if (curNode->key < iterNode->key) {
				AddNodeBefore(iterNode, curNode);
				break;
			}
			else {
				if (iterNode->next != NULL)
					iterNode = iterNode->next;
				else {
					AddNodeAfter(iterNode, curNode);
					break;
				}
					
			}
		}
	}
	fclose(filePointer);
	return SUCCESS;
}

void FreeList(list_t* list) {
	node_t* curNode = list->head;
	while (curNode != NULL) {
		node_t* deletingNode = curNode;
		curNode = curNode->next;
		free(deletingNode);
	}
	free(list);
}

bool IsWordInList(list_t* list) {
	int key;
	printf("Please insert a key for word in list: ");
	scanf("%d", &key);
	node_t* curNode = list->head;
	while (curNode != NULL) {
		if (curNode->key == key) {
			printf("List has the word \"%s\" with key %d", curNode->word, key);
			return true;
		}
		else
			curNode = curNode->next;
	}
	printf("List has not word with key %d", key);
	return false;
}
