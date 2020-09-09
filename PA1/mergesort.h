#ifndef MERGESORT_H
#define MERGESORT_H

// Includes
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

// Structs
typedef struct node{
	int data;
	struct node* next;
} Node;

// Function declarations

// @description: merges two sorted lists in ascending order and returns a pointer to the head of the list
// @pre: head1 points to the head of list 1, head2 points to the head of list 2. Both lists
//		 are sorted.
// @post: Returns a pointer to the head in the resulting sorted list.
// @usage: Node* newSortedList = merge(head1, head2);
Node* merge(Node* head1, Node* head2);

// @description: sorts a linked list in ascending order and returns a pointer to the head of the list
// @pre: lhead is a pointer to the first item in the list. lsize contains the size of the list
// @post: Returns a pointer to a sorted list
// @usage: Node* newptr = mergesort(head1, listSize);
Node* mergesort(Node* lhead, int lsize);

// @description: Creates a new node in heap memory
// @pre: data contains the number that should be considered the data item in the node
// @post: Returns a pointer to a node that now exists in the heap
// @usage: Node* newNode = createNode(6);
Node* createNode(int data);

// @description: Creates a linked list of a desired size.
// @pre: size contains the desired size of the linked list
// @post: Returns a pointer to the first node in a linked list of size 'size'
// @usage: Node* newLinkedList = createLinkedList(10);
Node* createLinkedList(int size);

// @description: Returns the size of a linked list
// @pre: head is a pointer to the first node of a linked list
// @post: Returns the size of the linked list
// @usage: int LLSize = getSize(head);
int getSize(Node* head);

// @description: Generates and returns a random number
// @pre: None
// @post: Returns a random number
// @usage: int newRandNum = randNum();
int randNum();

// @description: Prints an error message in case of running out of memory
// @pre: Memory has been overloaded and the function is called from error handler
// @post: Prints an error message to the screen.
// @usage: if (newptr == NULL) { printErrorMemory(); }
void printErrorMemory();

// @description: Prints the contents of a linked list
// @pre: head is a pointer to the first node in a linked list
// @post: Prints the linked list in a nice way
// @usage: printLinkedList(head);
void printLinkedList(Node* head);

// @description: Prints a single node. Helper function for printLinkedList.
// @pre: node is a pointer to the node being printed
// @post: The contents of the node are printed
// @usage: printNode(nodeptr);
void printNode(Node* node);

// @description: Prints an arrow. Helper function for printLinkedList.
// @pre: None
// @post: Prints an arrow to separate Node contents
// @usage: printArrow();
void printArrow();

#endif
