#include <stdio.h>

#include "linkedlist.h"

int main()
{
    linkedlist_t *newList = CreateLinkedList();
    AppendToLinkedList(newList, 1);
    AppendToLinkedList(newList, 3);
    AppendToLinkedList(newList, 5);

    PrintLinkedList(newList);

    FreeLinkedList(newList);

    return 0;
}