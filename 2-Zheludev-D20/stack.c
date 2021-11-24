#include <stdlib.h>
#include <stdio.h>
#include "stack.h"

stack_t* StackInit(int size) {
	stack_t* stack = (stack_t*)malloc(sizeof(stack_t));
	if (stack == NULL) {
		fprintf(stderr, "Allocation memory error!\n");
		return NULL;
	}
	stack->arr = (stackElem_t*)malloc(size * sizeof(stackElem_t));
	if (stack->arr == NULL) {
		fprintf(stderr, "Allocation memory error!\n");
		return NULL;
	}
	for (int i = 0; i < size; i++) {
		stack->arr[i].size = 0;
		stack->arr[i].index = -1;
	}
	stack->cur = -1;
	return stack;
}

void StackPush(stack_t* stack, int sizeElem, int indexElem) {
	stack->cur++;
	stack->arr[stack->cur].size = sizeElem;
	stack->arr[stack->cur].index = indexElem;
}

void StackPop(stack_t* stack) {
	if (stack->cur == -1)
		return;
	stack->arr[stack->cur].size = 0;
	stack->arr[stack->cur].index = -1;
	stack->cur--;
}

stackElem_t StackTop(stack_t* stack) {
	stackElem_t result = { 0, -1 };
	if (stack->cur == -1)
		return result;
	return stack->arr[stack->cur];
}

void FreeStack(stack_t* stack) {
	free(stack->arr);
	free(stack);
}
