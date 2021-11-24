#pragma once

typedef struct {
	int size;
	int index;
} stackElem_t;

typedef struct {
	int cur;
	stackElem_t* arr;
} stack_t;

stack_t* StackInit(int size);

void StackPush(stack_t* stack, int sizeElem, int indexElem);

void StackPop(stack_t* stack);

stackElem_t StackTop(stack_t* stack);

void FreeStack(stack_t* stack);
