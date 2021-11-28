#pragma once

stack_t* StackInit(uint size);

void StackPush(stack_t* stack, uint sizeElem, int indexElem);

void StackPop(stack_t* stack);

stackElem_t StackTop(stack_t* stack);

void FreeStack(stack_t* stack);
