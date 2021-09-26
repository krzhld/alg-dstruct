#include "list.h"

int main(void)
{
    list_t* myList = CreateList();
    FormSortedList(myList, "input.txt");
    PrintList(myList);
    IsWordInList(myList);
    FreeList(myList);
    return 0;
}
