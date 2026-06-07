#include "linkedlist.h"
#include <stdio.h>
#include <stdlib.h> // malloc, free, NULL

// malloc a new linked list, and then return a pointer to that linked list
linkedlist_t *CreateLinkedList() {
  linkedlist_t *newList = (linkedlist_t *)malloc(sizeof(linkedlist_t));
  newList->head = NULL;
  return newList;
}

// Walk through linked list, and print the data of each node
void PrintLinkedList(linkedlist_t *list) {
  node_t *currentNode = list->head;
  while (currentNode != NULL) {
    printf("data: %d\n", currentNode->data);
    currentNode = currentNode->next;
  }
}

// Walk through linked list, and delete all nodes
void FreeLinkedList(linkedlist_t *list) {
  node_t *currentNode = list->head;
  while (currentNode != NULL) {
    node_t *nextNode = currentNode->next;
    free(currentNode);
    currentNode = nextNode;
  }
  free(list);
}

// Create a new node_t, and add to the end of the linked list
void AppendToLinkedList(linkedlist_t *list, int data) {
  if (list->head == NULL) {
    list->head = (node_t *)malloc(sizeof(node_t));
    list->head->data = data;
    list->head->next = NULL;
  } else {
    node_t *currentNode = list->head;
    while (currentNode->next != NULL) {
      currentNode = currentNode->next;
    }
    currentNode->next = (node_t *)malloc(sizeof(node_t));
    currentNode->next->data = data;
    currentNode->next->next = NULL;
  }
}