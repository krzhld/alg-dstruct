#include "gtest/gtest.h"

extern "C" {
#include "../list.h"
}

#define FORM_TWO_LINKED_NODES \
list_t* list = CreateList(); \
node_t* firstNode = FormNode(1, (char*)"one"); \
AddNodeToEmptyList(list, firstNode); \
node_t* secondNode = FormNode(2, (char*)"two"); \
AddNodeAfter(firstNode, secondNode); 

#define LONG_WORD "Tweebuffelsmeteenskootmorsdoodgeskietfontein"

TEST(createList, handlesEmptyList) {
	list_t* list = CreateList();
	EXPECT_TRUE(list != NULL);
	EXPECT_TRUE(list->head == NULL);
}

TEST(formNode, handlesNodeWithEmptyWord) {
	node_t* node = FormNode(0, (char*)"");
	EXPECT_TRUE(node->key == 0);
	EXPECT_TRUE(!strcmp(node->word, ""));
	EXPECT_TRUE(node->next == NULL);
}

TEST(formNode, handlesNodeWithLongWord) {
	node_t* node = FormNode(1, (char*)LONG_WORD);
	EXPECT_TRUE(node->key == 1);
	EXPECT_TRUE(!strcmp(node->word, LONG_WORD));
	EXPECT_TRUE(node->next == NULL);
}

TEST(addNodeAfter, handlesLinkedNodes) {
	FORM_TWO_LINKED_NODES;
	node_t* curNode = list->head;
	EXPECT_TRUE(curNode->key == 1);
	EXPECT_TRUE(!strcmp(curNode->word, "one"));
	curNode = curNode->next;
	EXPECT_TRUE(curNode->key == 2);
	EXPECT_TRUE(!strcmp(curNode->word, "two"));
	EXPECT_TRUE(curNode->next == NULL);
}

TEST(addNodeBefore, handlesLinkedNodes) {
	FORM_TWO_LINKED_NODES;
	node_t* curNode = list->head;
	node_t* zeroNode = FormNode(0, (char*)"zero");
	AddNodeBefore(firstNode, zeroNode);
	EXPECT_TRUE(curNode->key == 0);
	EXPECT_TRUE(!strcmp(curNode->word, "zero"));
	curNode = curNode->next;
	EXPECT_TRUE(curNode->key == 1);
	EXPECT_TRUE(!strcmp(curNode->word, "one"));
	curNode = curNode->next;
	EXPECT_TRUE(curNode->key == 2);
	EXPECT_TRUE(!strcmp(curNode->word, "two"));
	EXPECT_TRUE(curNode->next == NULL);
}

TEST(formSortedList, handlesSimilarKeys) {
	list_t* list = CreateList();
	EXPECT_TRUE(FormSortedList(list, "UnitTest1.txt") == SIMILAR_KEYS_ERROR);
}

TEST(formSortedList, handlesEmptyList) {
	list_t* list = CreateList();
	EXPECT_TRUE(FormSortedList(list, "UnitTest2.txt") == SUCCESS);
	node_t* curNode = list->head;
	EXPECT_TRUE(list->head == NULL);
}

TEST(formSortedList, handlesFullyList) {
	list_t* list = CreateList();
	EXPECT_TRUE(FormSortedList(list, "UnitTest3.txt") == SUCCESS);
	node_t* curNode = list->head;
	EXPECT_TRUE(curNode->key == 1);
	EXPECT_TRUE(!strcmp(curNode->word, "Abc"));
	curNode = curNode->next;
	EXPECT_TRUE(curNode->key == 2);
	EXPECT_TRUE(!strcmp(curNode->word, "Def"));
	curNode = curNode->next;
	EXPECT_TRUE(curNode->key == 3);
	EXPECT_TRUE(!strcmp(curNode->word, "Ghi"));
	curNode = curNode->next;
	EXPECT_TRUE(curNode == NULL);
}
