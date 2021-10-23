#include "memallocator.h"
#include <stdio.h>
#include <stdlib.h>

#ifndef TRUE
#define TRUE 1
#endif

#ifndef FALSE
#define FALSE (!(TRUE))
#endif

#define GET_CELL(header) ((void*)((byte*)header + sizeof(block_t)))
#define GET_HEADER(pCell) ((block_t*)((byte*)pCell - sizeof(block_t)))

enum {
	NOT_INITIALIZED_SYSTEM = -100,
	ALREADY_INITIALIZED_SYSTEM,
	EMPTY_POINTER,
	TOO_SMALL_SIZE,
	BUSY_BLOCK
} memory_system_errors;

struct Block {
	struct Block* next;
	int size;
	char isAllocated;
};

typedef struct Block block_t;
typedef char byte;
typedef byte boolean;

block_t* __firstHeader = NULL;
int __initSize = 0;

block_t* __getBestBlock(int size) {
	if (__firstHeader == NULL)
		return NULL;

	block_t* bestBlock = NULL;
	block_t* curBlock = __firstHeader;

	while (curBlock != NULL) {
		if (curBlock->isAllocated == FALSE && curBlock->size >= size) {
			if (bestBlock == NULL || bestBlock->size > curBlock->size)
				bestBlock = curBlock;
		}
		curBlock = curBlock->next;
	}
	return bestBlock;
}

block_t* __insertBlock(block_t* block, int size) {
	if (__firstHeader == NULL)
		return NULL;

	if (block == NULL)
		return NULL;

	if (block->isAllocated == TRUE)
		return NULL;

	if (block->size - size > (int)sizeof(block_t)) {
		block_t* newBlock = (block_t*)((byte*)GET_CELL(block) + size);

		newBlock->size = block->size - size - sizeof(block_t);
		newBlock->isAllocated = FALSE;
		newBlock->next = block->next;

		block->next = newBlock;
	}

	block->size = size;
	block->isAllocated = TRUE;

	return block;
}

int __getPoolSize() {
	if (__firstHeader == NULL)
		return NOT_INITIALIZED_SYSTEM;

	block_t* curBlock = __firstHeader;
	int size = 0;
	while (curBlock != NULL) {
		size += curBlock->size + sizeof(block_t);
		curBlock = curBlock->next;
	}

	return size;
}

boolean __isThereSuchBlock(block_t* block) {
	block_t* curBlock = __firstHeader;
	while (curBlock != NULL) {
		if (curBlock == block)
			return TRUE;

		curBlock = curBlock->next;
	}
	return FALSE;
}

block_t* __getPreviousBlock(block_t* block) {
	if (block == NULL)
		return NULL;

	if (block == __firstHeader)
		return NULL;

	if (__firstHeader == NULL)
		return NULL;

	block_t* prevBlock = __firstHeader;

	while (prevBlock->next != NULL) {
		if (prevBlock->next == block)
			return prevBlock;
		prevBlock = prevBlock->next;
	}
	return NULL;
}

int meminit(void* pMemory, int size) {
	if (__firstHeader != NULL)
		return ALREADY_INITIALIZED_SYSTEM;

	if (size < sizeof(block_t))
		return TOO_SMALL_SIZE;

	if (pMemory == NULL)
		return EMPTY_POINTER;

	__firstHeader = (block_t*)pMemory;
	__firstHeader->size = size - sizeof(block_t);
	__firstHeader->next = NULL;
	__firstHeader->isAllocated = FALSE;

	__initSize = size;

	return 0;
}

void memdone() {
	if (__firstHeader == NULL)
		return;

	int curSize = __getPoolSize();
	if (curSize == NOT_INITIALIZED_SYSTEM)
		return;

	if (curSize != __initSize)
		return;
}

void* memalloc(int size) {
	if (__firstHeader == NULL)
		return NULL;

	block_t* bestBlock = __getBestBlock(size);
	if (bestBlock == NULL)
		return NULL;

	block_t* resultBlock = __insertBlock(bestBlock, size);
	if (resultBlock == NULL)
		return NULL;

	return GET_CELL(resultBlock);
}

void memfree(void* p) {
	if (p == NULL)
		return;

	block_t* deleteBlock = GET_HEADER(p);
	if (__isThereSuchBlock(deleteBlock) == FALSE)
		return;

	if (deleteBlock->isAllocated == FALSE)
		return;

	block_t* mergeBlock = deleteBlock;
	mergeBlock->isAllocated = FALSE;

	block_t* prevBlock = __getPreviousBlock(deleteBlock);
	if (prevBlock != NULL) {
		if (prevBlock->isAllocated == FALSE) {
			deleteBlock->isAllocated = FALSE;
			int freeSize = 0;
			if (deleteBlock->next != NULL)
				freeSize = (byte*)deleteBlock->next - (byte*)GET_CELL(prevBlock);
			else
				freeSize = (byte*)GET_CELL(__firstHeader) + __initSize - (byte*)GET_CELL(prevBlock);

			prevBlock->next = deleteBlock->next;
			prevBlock->size = freeSize;
			mergeBlock = prevBlock;
		}
	}

	if (mergeBlock->next != NULL) {
		if (mergeBlock->next->isAllocated == FALSE) {
			mergeBlock->isAllocated = FALSE;
			int freeSize = 0;
			if (mergeBlock->next->next != NULL)
				freeSize = (byte*)mergeBlock->next->next - (byte*)GET_CELL(mergeBlock);
			else
				freeSize = (byte*)GET_CELL(__firstHeader) + __initSize - (byte*)GET_CELL(mergeBlock);

			mergeBlock->next = mergeBlock->next->next;
			mergeBlock->size = freeSize;
		}
	}

	if (mergeBlock->next != NULL) {
		if ((byte*)((byte*)GET_CELL(mergeBlock) + mergeBlock->size) != (byte*)mergeBlock->next) {
			byte* unallocMemoryPtr = (byte*)((byte*)GET_CELL(mergeBlock) + mergeBlock->size);
			int unallocMemorySize = (byte*)mergeBlock->next - unallocMemoryPtr;
			mergeBlock->size += unallocMemorySize;
		}
	}
	else {
		if ((byte*)((byte*)__firstHeader + __initSize) != (byte*)((byte*)GET_CELL(mergeBlock) + mergeBlock->size)) {
			byte* unallocMemoryPtr = (byte*)((byte*)GET_CELL(mergeBlock) + mergeBlock->size);
			int unallocMemorySize = (byte*)__firstHeader + __initSize - unallocMemoryPtr;
			mergeBlock->size += unallocMemorySize;
		}
	}
}

int memgetminimumsize() {
	return sizeof(block_t);
}

int memgetblocksize() {
	return sizeof(block_t);
}
