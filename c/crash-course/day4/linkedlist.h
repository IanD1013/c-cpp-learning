#ifndef LINKEDLIST_H
#define LINKEDLIST_H

// Individual node in the chain
typedef struct node {
  int data;
  struct node *next;
} node_t;

// Linked list data structure, which always holds the first node, in our 'chain'
typedef struct linkedlist {
  node_t *head;
} linkedlist_t;

// malloc a new linked list, and then return a pointer to that linked list
linkedlist_t *CreateLinkedList();

// Walk through linked list, and print the data of each node
void PrintLinkedList(linkedlist_t *list);

// Walk through linked list, and delete all nodes
void FreeLinkedList(linkedlist_t *list);

// Create a new node_t, and add to the end of the linked list
void AppendToLinkedList(linkedlist_t *list, int data);

#endif