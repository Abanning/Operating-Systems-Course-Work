#include "mergesort.h"

int main()
{
	Node* myLinkedList1 = createLinkedList(10);
	Node* myLinkedList2 = createLinkedList(10);
	
	printf("\n");
	printf("----------------------------------------------------------------------------------------------------------------- \n");
	
	printf("Unsorted List 1: \n");
	printLinkedList(myLinkedList1);
	
	myLinkedList1 = mergesort(myLinkedList1, getSize(myLinkedList1));
	
	printf("Sorted List 1: \n");
	printLinkedList(myLinkedList1);
	
	printf("----------------------------------------------------------------------------------------------------------------- \n");
	printf("\n");
	
	printf("Testing another linked list...");
	printf("\n");
	
	printf("\n");
	printf("----------------------------------------------------------------------------------------------------------------- \n");
	
	printf("Unsorted List 2: \n");
	printLinkedList(myLinkedList2);
	
	myLinkedList2 = mergesort(myLinkedList2, getSize(myLinkedList2));
	
	printf("Sorted List 2: \n");
	printLinkedList(myLinkedList2);
	
	printf("----------------------------------------------------------------------------------------------------------------- \n");
	printf("\n");
	
	return 0;
}

Node* merge(Node* head1, Node* head2)
{
	Node* newptr = NULL;
	
	// Base Case
	if (head1 == NULL)
	{
		return head2;
	}
	else if (head2 == NULL)
	{
		return head1;
	}
	
	// Else
	if ((head1 -> data) <= (head2 -> data))
	{
		newptr = head1;
		newptr -> next = merge(head1 -> next, head2);
	}
	else
	{
		newptr = head2;
		newptr -> next = merge(head1, head2 -> next);
	}
	
	return newptr;
}

Node* mergesort(Node* lhead, int lsize)
{
	Node* cutptr = lhead; // start at front
	Node* rhead;
	int lsizeNew = 0;
	int rsize = 0;
	
	// Base Case
	if (lsize == 0)
	{
		return NULL;
	}
	// Alternate Case
	else if (lsize == 1)
	{
		return lhead;
	}
	// Else case
	else
	{
		int mid = (lsize - 1) / 2; // middle index
	
		for (int i = 0; i < mid; ++i)
		{
			cutptr = cutptr -> next; // increment tracker
			lsizeNew++;
		}
		
		// Make cut
		rhead = cutptr -> next;
		cutptr -> next = NULL;
		cutptr = NULL;
	
		// Handle Size
		lsizeNew++;
		rsize = lsize - lsizeNew;
		
		Node* newptr = merge(mergesort(lhead, lsizeNew), mergesort(rhead, rsize));
		
		return newptr;
	}
}

Node* createNode(int data)
{
	Node* new_node = (Node*) malloc(sizeof(Node*));
	if (new_node == NULL)
	{
		printErrorMemory();
	}
	else
	{
		new_node -> data = data;
		new_node -> next = NULL;
	}
	
	return new_node;
}

Node* createLinkedList(int size)
{
	Node* head = NULL;
	Node* currentptr = NULL;
	Node* new_node = createNode(randNum());
	if (new_node != NULL)
	{
		head = new_node;
		currentptr = head; // currentptr start at head
	}
	else
	{
		printErrorMemory();
		return NULL;
	}
	
	for (int i = 0; i < (size - 1); i++)
	{
		new_node = createNode(randNum()); // create new node
		currentptr -> next = new_node; // set next
		currentptr = currentptr -> next; // increment current to next
	}
	
	// Secure data
	currentptr = NULL;
	new_node = NULL;
	
	return head;
}

int getSize(Node* head)
{
	Node* currentptr = head;
	int size = 0;
	
	while (currentptr != NULL)
	{
		size++;
		currentptr = currentptr -> next;
	}
	
	return size;
}

int randNum()
{
	return (rand() % 101); // return a number between 0 - 100
}

void printErrorMemory()
{
	printf("NO MORE MEMORY! \n");
	printf("FATAL ERROR. PROGRAM ENDING... \n");
}

void printLinkedList(Node* head)
{
	Node* currentptr = head; // currenttracker
	
	printf("\n"); // for spacing
	while (currentptr != NULL)
	{
		printNode(currentptr);
		printArrow();
		currentptr = currentptr -> next;
	}
	
	printf("|NULL| \n");
	printf("\n"); // for spacing
}

void printNode(Node* node)
{
	printf("|%3d |", node -> data);
}

void printArrow()
{
	printf(" -> ");
}
