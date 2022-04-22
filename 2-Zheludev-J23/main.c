#define _CRT_SECURE_NO_WARNINGS
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define NOTSTATED -1
#define FALSE 0
#define TRUE 1

typedef struct {
	int key;
	char* str;
} table_elem;

table_elem* table;
char* deleted;
const int size = 57271;

void InitHashTable(void) {
	table = (table_elem*)malloc(size * sizeof(table_elem));
	if (table == NULL)
		return;
	deleted = (char*)malloc(size * sizeof(char));
	if (deleted == NULL)
		return;
	for (int i = 0; i < size; i++) {
		table[i].key = NOTSTATED;
		table[i].str = NULL;
		deleted[i] = FALSE;
	}
}

int F(int key, int mod) {
	int result = key % mod;
	return result;
}

int Search(int key) {
	int x = F(key, size);
	int y = F(key, size - 1) + 1;
	int cur = x % size;
	for (int i = 1; i <= size; i++) {
		if (table[x].key != NOTSTATED) {
			if ((table[x].key == key) && !deleted[x])
				return 1;
			else
				return 0;
		}
		cur = (x + i * y) % size;
	}
	return 0;
}

void Insert(int key, char* str) {
	if (Search(key))
		return;
	int x = F(key, size);
	int y = F(key, size - 1) + 1;
	int cur = x % size;
	for (int i = 1; i <= size; i++) {
		if ((table[x].key == NOTSTATED) || deleted[x]) {
			table[x].key = key;
			table[x].str = (char*)malloc((strlen(str) + 1) * sizeof(char));
			if (table[x].str == NULL)
				return;
			strcpy(table[x].str, str);
			table[x].str[strlen(str)] = '\0';
			deleted[x] = FALSE;
			return;
		}
		cur = (x + i * y) % size;
	}
}

void Delete(int key) {
	if (!Search(key))
		return;
	int x = F(key, size);
	int y = F(key, size - 1) + 1;
	int cur = x % size;
	for (int i = 1; i <= size; i++) {
		if (table[x].key != NOTSTATED) {
			if (table[x].key == key)
				deleted[x] = TRUE;
			else
				return;
		}
		cur = (x + i * y) % size;
	}
}

void DestroyHashTable(void) {
	if (table == NULL)
		return;
	for (int i = 0; i < size; i++)
		free(table[i].str);
	free(table);
	table = NULL;
	free(deleted);
}

int main() {
	InitHashTable();
	char command;
	int key;
	while (scanf("%c", &command) >= 1) {
		scanf("%d", &key);
		switch (command) {
		case 'a':
			Insert(key, "");
			break;
		case 'r':
			Delete(key);
			break;
		case 'f':
			if (Search(key))
				puts("yes");
			else
				puts("no");
			break;
		}
	}
	DestroyHashTable();
	return 0;
}
