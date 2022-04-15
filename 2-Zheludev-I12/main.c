#define _CRT_SECURE_NO_WARNINGS
#include <stdlib.h>
#include <stdio.h>
#include <Windows.h>

#define T 3  
#define FAIL -3
#define SUCCESS -2 
#define LESSKEYS -1

typedef struct Node {
	struct Node* childs[2 * T];
	int keys[2 * T - 1];
	int n;
} Node;

Node* root;
int x = 1;

Node* _node() {
	Node* node = malloc(sizeof(Node));
	if (node == NULL)
		return NULL;
	if (node != NULL) {
		for (int i = 0; i < 2 * T; i++)
			node->childs[i] = NULL;
		node->n = 0;
	}
	return node;
}

int _delete(Node* node, int key) {
	int pos;
	int i;
	int pivot;
	int n;
	int* key_arr;
	int value;
	Node* lptr;
	Node* rptr;
	Node** p;
	if (node == NULL)
		return FAIL;
	n = node->n;
	key_arr = node->keys;
	p = node->childs;
	pos = 0;
	while (pos < n && key > key_arr[pos])
		pos++;
	if (p[0] == NULL) {
		if (pos == n || key < key_arr[pos])
			return FAIL;
		for (i = pos + 1; i < n; i++) {
			key_arr[i - 1] = key_arr[i];
			p[i] = p[i + 1];
		}
		if (--node->n >= (node == root ? 1 : (T - 1)))
			return SUCCESS;
		else
			return LESSKEYS;
	}
	if (pos < n && key == key_arr[pos]) {
		Node* qp = p[pos];
		Node* qp1;
		int nkey;
		while (1) {
			nkey = qp->n;
			qp1 = qp->childs[nkey];
			if (qp1 == NULL)
				break;
			qp = qp1;
		}
		key_arr[pos] = qp->keys[nkey - 1];
		qp->keys[nkey - 1] = key;
	}
	value = _delete(p[pos], key);
	if (value != LESSKEYS)
		return value;
	if (pos > 0 && p[pos - 1]->n > (T - 1)) {
		pivot = pos - 1;
		lptr = p[pivot];
		rptr = p[pos];
		rptr->childs[rptr->n + 1] = rptr->childs[rptr->n];
		for (i = rptr->n; i > 0; i--) {
			rptr->keys[i] = rptr->keys[i - 1];
			rptr->childs[i] = rptr->childs[i - 1];
		}
		rptr->n++;
		rptr->keys[0] = key_arr[pivot];
		rptr->childs[0] = lptr->childs[lptr->n];
		key_arr[pivot] = lptr->keys[--lptr->n];
		return SUCCESS;
	}
	if (pos < n && p[pos + 1]->n > (T - 1)) {
		pivot = pos;
		lptr = p[pivot];
		rptr = p[pivot + 1];
		lptr->keys[lptr->n] = key_arr[pivot];
		lptr->childs[lptr->n + 1] = rptr->childs[0];
		key_arr[pivot] = rptr->keys[0];
		lptr->n++;
		rptr->n--;
		for (i = 0; i < rptr->n; i++) {
			rptr->keys[i] = rptr->keys[i + 1];
			rptr->childs[i] = rptr->childs[i + 1];
		}
		rptr->childs[rptr->n] = rptr->childs[rptr->n + 1];
		return SUCCESS;
	}
	if (pos == n)
		pivot = pos - 1;
	else
		pivot = pos;
	lptr = p[pivot];
	rptr = p[pivot + 1];
	lptr->keys[lptr->n] = key_arr[pivot];
	lptr->childs[lptr->n + 1] = rptr->childs[0];
	for (i = 0; i < rptr->n; i++) {
		lptr->keys[lptr->n + 1 + i] = rptr->keys[i];
		lptr->childs[lptr->n + 2 + i] = rptr->childs[i + 1];
	}
	lptr->n = lptr->n + rptr->n + 1;
	free(rptr);
	for (i = pos + 1; i < n; i++) {
		key_arr[i - 1] = key_arr[i];
		p[i] = p[i + 1];
	}
	if (--node->n >= (node == root ? 1 : (T - 1)))
		return SUCCESS;
	else
		return LESSKEYS;
}

void Delete(int key) {
	Node* uproot;
	int value = _delete(root, key);
	if (value == LESSKEYS) {
		uproot = root;
		root = root->childs[0];
		free(uproot);
	}
}

void _split(Node* upnode, int index) {
	Node* rptr = _node();
	Node* lptr = upnode->childs[index];
	rptr->n = (T - 1);
	for (int i = 0; i < (T - 1); i++)
		rptr->keys[i] = lptr->keys[i + T];
	if (lptr->childs[0] != NULL)
		for (int i = 0; i < T; i++)
			rptr->childs[i] = lptr->childs[i + T];
	lptr->n = (T - 1);
	for (int i = upnode->n; i >= index; i--) 
		upnode->childs[i + 1] = upnode->childs[i];
	upnode->childs[index + 1] = rptr;
	for (int i = upnode->n - 1; i >= index; i--)
		upnode->keys[i + 1] = upnode->keys[i];
	upnode->keys[index] = lptr->keys[(T - 1)];
	upnode->n++;
}

void _insertNonfull(Node* node, int data) {
	int len = node->n - 1;
	if (node->childs[0] == NULL) {
		while (len >= 0 && data < node->keys[len]) {
			node->keys[len + 1] = node->keys[len];
			--len;
		}
		node->keys[len + 1] = data;
		node->n++;
	}
	else {
		while (len >= 0 && data < node->keys[len])
			--len;
		++len;
		if (node->childs[len]->n == (2 * T - 1)) {
			_split(node, len);
			if (data > node->keys[len])
				++len;
		}
		_insertNonfull(node->childs[len], data);
	}
}

void Insert(int data) {
	if (Search(data))
		return;
	if (root == NULL) {
		root = _node();
		root->keys[0] = data;
		root->n = 1;
	}
	else if (root->n == (2 * T - 1)) {
		Node* old_root = root;
		Node* new_root_node = _node();
		root = new_root_node;
		new_root_node->childs[0] = old_root;
		_split(new_root_node, 0);
		_insertNonfull(new_root_node, data);
	}
	else
		_insertNonfull(root, data);
}

int Search(int data) {
	Node* cur = root;
	int child_index = 0;
	while (cur != NULL) {
		for (int i = 0; i < cur->n; i++) {
			if (data == cur->keys[i])
				return 1;
			if (data < cur->keys[i]) {
				child_index = i;
				break;
			}
			else 
				child_index = i + 1;
		}
		cur = cur->childs[child_index];
	}
	return 0;
}

void _print(Node* node, int y, int width) {
	if (node == NULL)
		return;
	_print(node->childs[0], y + 2, width);
	COORD c;
	c.X = x;
	c.Y = y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), c);
	printf("( ");
	for (int i = 0; i < node->n; i++)
		printf("%d ", node->keys[i]);
	printf(")");
	for (int i = 1; i <= node->n; i++) {
		x += width + 5;
		_print(node->childs[i], y + 2, width);
	}
}

void Print(int y) {
	x = 1;
	int width = 0;
	for (int i = 0; i < root->n; i++) {
		int x = root->keys[i];
		while (x != 0) {
			x /= 10;
			width++;
		}
	}
	_print(root, y, width);
}

void _destroy(Node* node) {
	if (node->n == 0) {
		free(node);
		return;
	}
	free(node->keys);
	for (int i = 0; i <= node->n; i++)
		_destroy(node->childs[i]);
	free(node);
	return;
}

void Destroy(void) {
	_destroy(root);
	root = NULL;
}

int main() {
	char command;
	int value;
	root = NULL;
	while (scanf("%c", &command) >= 1) {
		scanf("%d", &value);
		switch (command) {
		case 'a':
			Insert(value);
			break;
		case 'r':
			Delete(value);
			break;
		case 'f':
			if (Search(value))
				puts("yes");
			else
				puts("no");
			break;
		}
	}
	return 0;
}
