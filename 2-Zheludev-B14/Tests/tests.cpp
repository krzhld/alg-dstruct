#include "gtest/gtest.h"

extern "C" {
#include "..\memallocator.h"
#include "..\memallocator.cpp"
}

#define FREE_SYSTEM(ptr) \
__initSize = 0; \
__firstHeader = NULL; \
free(ptr);

TEST(meminit_test, allocate_system_for_one_block_which_size_one) {
	int memsize = memgetminimumsize() + 1;
	void *ptr = malloc(memsize);
	meminit(ptr, memsize);

	EXPECT_EQ(__initSize, memsize);

	EXPECT_TRUE(__firstHeader != NULL);
	EXPECT_FALSE(__firstHeader->isAllocated);
	EXPECT_EQ(__firstHeader->size, memsize - memgetblocksize());
	EXPECT_TRUE(__firstHeader->next == NULL);

	FREE_SYSTEM(ptr);
}

TEST(meminit_test, meminit_test_allocate_system_for_one_empty_block) {
	void* p = 0;
	void* ptr = malloc(memgetminimumsize());
	meminit(ptr, memgetminimumsize());

	EXPECT_TRUE(__initSize == memgetminimumsize());

	EXPECT_TRUE(__firstHeader != NULL);
	EXPECT_FALSE(__firstHeader->isAllocated);
	EXPECT_EQ(__firstHeader->size, 0);
	EXPECT_TRUE(__firstHeader->next == NULL);

	FREE_SYSTEM(ptr);
}

TEST(memalloc_memfree_test, allocate_two_blocks_with_enough_memory_size) {
	void* p1 = 0, * p2 = 0;
	int memsize = 2 * memgetblocksize() + 1 + 2;
	void *ptr = malloc(memsize);
	meminit(ptr, memsize);

	p1 = memalloc(1);
	EXPECT_TRUE(p1 != NULL);
	EXPECT_TRUE(GET_HEADER(p1) == __firstHeader);
	EXPECT_EQ(GET_HEADER(p1)->size, 1);
	EXPECT_TRUE(GET_HEADER(p1)->isAllocated);
	EXPECT_FALSE(GET_HEADER(p1)->next->isAllocated);

	/* next block must be nearly first block without spaces between */
	EXPECT_TRUE((byte*)GET_HEADER(p1)->next == (byte*)GET_HEADER(p1) + memgetblocksize() + GET_HEADER(p1)->size);
	/* next separated block must not be allocated */
	EXPECT_FALSE(GET_HEADER(p1)->next->isAllocated);

	p2 = memalloc(2);
	EXPECT_TRUE(p2 != NULL);
	EXPECT_TRUE(GET_HEADER(p2) == __firstHeader->next);
	EXPECT_EQ(GET_HEADER(p2)->size, 2);
	EXPECT_TRUE(GET_HEADER(p2)->isAllocated);

	/* next block must be null because there will be no memory for another blocks */
	EXPECT_TRUE(GET_HEADER(p2)->next == NULL);
	EXPECT_TRUE(GET_HEADER(p1)->next == GET_HEADER(p2));

	/* do headers have pointers to memory which has been allocated? */
	block_t* curBlock = __firstHeader;
	EXPECT_EQ(GET_CELL(curBlock), p1);
	
	curBlock = curBlock->next;
	EXPECT_EQ(GET_CELL(curBlock), p2);
	
	memfree(p1);
	EXPECT_FALSE(GET_HEADER(p1)->isAllocated);

	memfree(p2);
	EXPECT_FALSE(GET_HEADER(p2)->isAllocated);

	FREE_SYSTEM(ptr);
}

TEST(memalloc_memfree_test, allocate_two_blocks_with_not_enough_memory_size) {
	void* p1 = 0, * p2 = 0;
	int  memsize = memgetminimumsize() + memgetblocksize() + 1;
	void* ptr = malloc(memsize);
	meminit(ptr, memsize);

	p1 = memalloc(1);
	EXPECT_TRUE(p1 != NULL);
	EXPECT_TRUE(GET_HEADER(p1) == __firstHeader);
	EXPECT_EQ(GET_HEADER(p1)->size, 1);
	EXPECT_TRUE(GET_HEADER(p1)->isAllocated);

	/* next block must be null because there will be no memory for another blocks */
	EXPECT_TRUE(GET_HEADER(p1)->next == NULL);

	/* no memory for another blocks */
	p2 = memalloc(1);
	EXPECT_TRUE(p2 == NULL);

	block_t* curBlock = __firstHeader;
	EXPECT_EQ(GET_CELL(curBlock), p1);

	curBlock = curBlock->next;
	EXPECT_TRUE(curBlock == NULL);

	memfree(p1);
	EXPECT_FALSE(GET_HEADER(p1)->isAllocated);

	memfree(p2);

	FREE_SYSTEM(ptr);
}

TEST(memalloc_memfree_test, allocate_three_blocks_free_second_and_allocate_same_again) {
	void* a = 0, * b = 0, * c = 0;
	int memsize = 3 * memgetblocksize() + 2 + 3 + 2;
	void* ptr = malloc(memsize);
	meminit(ptr, memsize);

	a = memalloc(2);
	EXPECT_TRUE(a != NULL);
	EXPECT_TRUE(GET_HEADER(a) == __firstHeader);
	EXPECT_EQ(GET_HEADER(a)->size, 2);
	EXPECT_TRUE(GET_HEADER(a)->isAllocated);
	EXPECT_FALSE(GET_HEADER(a)->next->isAllocated);

	b = memalloc(3);
	EXPECT_TRUE(b != NULL);
	EXPECT_TRUE(GET_HEADER(b) == __firstHeader->next);
	EXPECT_EQ(GET_HEADER(b)->size, 3);
	EXPECT_TRUE(GET_HEADER(b)->isAllocated);
	EXPECT_TRUE(GET_HEADER(a)->next == GET_HEADER(b));
	EXPECT_FALSE(GET_HEADER(b)->next->isAllocated);

	c = memalloc(2);
	EXPECT_TRUE(c != NULL);
	EXPECT_TRUE(GET_HEADER(c) == __firstHeader->next->next);
	EXPECT_EQ(GET_HEADER(c)->size, 2);
	EXPECT_TRUE(GET_HEADER(c)->isAllocated);
	EXPECT_TRUE(GET_HEADER(b)->next == GET_HEADER(c));
	/* next block must be null because there will be no memory for another blocks */
	EXPECT_TRUE(GET_HEADER(c)->next == NULL);

	memfree(b);
	EXPECT_FALSE(GET_HEADER(b)->isAllocated);

	/* if we allocate again 3-byte block then it must be in the middle of 'a' and 'c' without spaces */
	b = memalloc(3);
	block_t* probablyC = (block_t*)((byte*)b + memgetblocksize() + GET_HEADER(b)->size);
	EXPECT_TRUE(probablyC == c);
	
	FREE_SYSTEM(ptr);
}

TEST(memalloc_memfree_test, allocate_three_blocks_free_second_and_allocate_same_again_where_system_has_lots_memory) {
	void* a = 0, * b = 0, * c = 0;
	int memsize = 3 * memgetblocksize() + 2 + 3 + 2 + 100;
	void* ptr = malloc(memsize);
	meminit(ptr, memsize);

	a = memalloc(2);
	EXPECT_TRUE(a != NULL);
	EXPECT_TRUE(GET_HEADER(a) == __firstHeader);
	EXPECT_EQ(GET_HEADER(a)->size, 2);
	EXPECT_TRUE(GET_HEADER(a)->isAllocated);
	EXPECT_FALSE(GET_HEADER(a)->next->isAllocated);

	b = memalloc(3);
	EXPECT_TRUE(b != NULL);
	EXPECT_TRUE(GET_HEADER(b) == __firstHeader->next);
	EXPECT_EQ(GET_HEADER(b)->size, 3);
	EXPECT_TRUE(GET_HEADER(b)->isAllocated);
	EXPECT_TRUE(GET_HEADER(a)->next == GET_HEADER(b));
	EXPECT_FALSE(GET_HEADER(b)->next->isAllocated);

	c = memalloc(2);
	EXPECT_TRUE(c != NULL);
	EXPECT_TRUE(GET_HEADER(c) == __firstHeader->next->next);
	EXPECT_EQ(GET_HEADER(c)->size, 2);
	EXPECT_TRUE(GET_HEADER(c)->isAllocated);
	EXPECT_TRUE(GET_HEADER(b)->next == GET_HEADER(c));
	EXPECT_FALSE(GET_HEADER(c)->next->isAllocated);

	memfree(b);
	EXPECT_FALSE(GET_HEADER(b)->isAllocated);

	/* if we allocate again 3-byte block then it must be in the middle of 'a' and 'c' without spaces
	not after third block because of findBestBlock strategy */
	b = memalloc(3);
	block_t* probablyC = (block_t*)((byte*)b + memgetblocksize() + GET_HEADER(b)->size);
	EXPECT_TRUE(probablyC == c);

	FREE_SYSTEM(ptr);
}

TEST(memalloc_memfree_test, allocate_three_blocks_free_second_and_allocate_big_block_where_system_has_lots_memory) {
	void* a = 0, * b = 0, * c = 0;
	int memsize = 3 * memgetblocksize() + 2 + 3 + 2 + 100;
	void* ptr = malloc(memsize);
	meminit(ptr, memsize);

	a = memalloc(2);
	EXPECT_TRUE(a != NULL);
	EXPECT_TRUE(GET_HEADER(a) == __firstHeader);
	EXPECT_EQ(GET_HEADER(a)->size, 2);
	EXPECT_TRUE(GET_HEADER(a)->isAllocated);
	EXPECT_FALSE(GET_HEADER(a)->next->isAllocated);

	b = memalloc(3);
	EXPECT_TRUE(b != NULL);
	EXPECT_TRUE(GET_HEADER(b) == __firstHeader->next);
	EXPECT_EQ(GET_HEADER(b)->size, 3);
	EXPECT_TRUE(GET_HEADER(b)->isAllocated);
	EXPECT_TRUE(GET_HEADER(a)->next == GET_HEADER(b));
	EXPECT_FALSE(GET_HEADER(b)->next->isAllocated);

	c = memalloc(2);
	EXPECT_TRUE(c != NULL);
	EXPECT_TRUE(GET_HEADER(c) == __firstHeader->next->next);
	EXPECT_EQ(GET_HEADER(c)->size, 2);
	EXPECT_TRUE(GET_HEADER(c)->isAllocated);
	EXPECT_TRUE(GET_HEADER(b)->next == GET_HEADER(c));
	EXPECT_FALSE(GET_HEADER(c)->next->isAllocated);

	memfree(b);
	EXPECT_FALSE(GET_HEADER(b)->isAllocated);

	/* if we allocate bigger than 3-byte block then it must be after third block because of findBestBlock strategy */
	b = memalloc(4);
	block_t* probablyBigBlock = (block_t*)((byte*)c + memgetblocksize() + GET_HEADER(c)->size);
	EXPECT_TRUE(probablyBigBlock == b);

	FREE_SYSTEM(ptr);
}
