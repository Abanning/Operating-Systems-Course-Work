// @filename: sorting.c
// @author: Alex Banning
// @assignment: PA4 - Sorting a list using multithreaded programming
// @date: October 30th, 2019
// @description: This program takes an unsorted list and sorts the list by
//				 splitting it into two halves and sorting each half in a 
//				 different thread. After the threads are joined, a new
//				 thread is created and is used to merge the sublists.
//				 This program uses global arrays for the lists, and
//				 a struct parameters for passing to the threads.

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#define SIZE				10
#define NUMBER_OF_THREADS	3

// Function prototypes
void* sorter(void* params);	/* thread that performs basic sorting algorithm */
void* merger(void* params);	/* thread that performs merging of results */
void displayArray(void); 	/* displays the global array */
void printNewLines(int numOfLines);	/* prints a desired number of lines on the screen */

// Declare global variables
int list[SIZE] = {7,12,19,3,18,4,2,6,15,8};
int result[SIZE];

// Define struct parameters
typedef struct parameters
{
	int from_index;
	int to_index;
	int threadCount;
} parameters;

// Main program. Executes the sorting of the list and creation of threads.
int main (int argc, const char* argv[]) 
{
	printf("Unsorted List: ");
	for (int i = 0; i < SIZE; i++) {
		printf("%d ", list[i]);
	}
	printNewLines(2);
    
	pthread_t workers[NUMBER_OF_THREADS];
	
	/* establish the first sorting thread */
	//1. call malloc to allocate a �parameters�
	//2. use �parameters� to specify the first half of the array
    //3. create the first thread
	parameters* p1 = malloc(sizeof(parameters*)); // Allocate memory for parameter set 1
	p1 -> from_index = 0;
	p1 -> to_index = (SIZE - 1) / 2;
	p1 -> threadCount = 1;
	pthread_create(&workers[0], 0, sorter, p1);
	

	/* establish the second sorting thread */
	//1. call malloc to allocate a �parameters�
	//2. use �parameters� to specify the first half of the array
    //3. create the second thread
	parameters* p2 = malloc(sizeof(parameters*)); // Allocate memory for parameter set 2
	p2 -> from_index = p1 -> to_index + 1;
	p2 -> to_index = (SIZE -1);
	p2 -> threadCount = 2;
	pthread_create(&workers[1], 0, sorter, p2);
  
	
	/* now wait for the 2 sorting threads to finish */
	// use ptheread_join; wait for 2 sorting threads to finish 
	pthread_join(workers[0], 0);
	pthread_join(workers[1], 0);


	/* establish the merge thread */
	//reuse �parameters� to hold the beginning index in each half
	//create the third thread: merge 
	p1 -> to_index = p2 -> from_index; // overwrite p1 to index 
	p1 -> threadCount = 3;
	pthread_create(&workers[2], 0, merger, p1);
	pthread_join(workers[2], 0);
	/* wait for the merge thread to finish */
	
	/* output the sorted array */
	displayArray();
	
    return 0;
}

// @description: Sorts the list given a starting and ending index in parameters
// @pre: parameters contains the values of from_index and to_index used for sorting
// @post: list[from_index - to_index] is sorted
// @usage: pthread_create(&workers[i], 0, sorter, p);
void* sorter(void* params)
{
	parameters* p = (parameters*)params;
	int startIndex = p -> from_index;
	int endIndex = p -> to_index;
	int threadCount = p -> threadCount;
	int min_index;
	int temp;

	printf("Sorting Sub-List: %d...\n", threadCount);

	for (int i = startIndex; i < endIndex; i++) {
		min_index = i;
		for (int j = i + 1; j <= endIndex; j++) {
			if (list[j] < list[min_index]) {
				min_index = j;
			}
		}

		temp = list[min_index];
		list[min_index] = list[i];
		list[i] = temp;
	}

	printf("Sorted Sub-List %d: ", threadCount);
	for (int i = startIndex; i <= endIndex; i++) {
		printf("%d ", list[i]);
	}
	printNewLines(1);

	pthread_exit(0);
}

// @description: Merges the sorted sub-lists using parameters
// @pre: parameters contains the start index of list1 in from_index and the start index of list2 in to_index
// @post: result contains the sorted list
// @usage: pthread_create(&workers[i], 0, merger, p);
void* merger(void* params)
{
	printf("Merging the Sub-Lists...\n");
	parameters* p = (parameters*)params;
	int index_list1 = p -> from_index;
	int index_list2 = p -> to_index;
	printf("List 1 begins at index: %d\n", index_list1);
	printf("List 2 begins at index: %d\n", index_list2);
	for (int i = 0; i < SIZE; i++) {
		if (list[index_list1] < list[index_list2]) {
			result[i] = list[index_list1];
			printf("%d added to result from List 1!", result[i]);
			index_list1++;
			printf("\t new index: %d\n", index_list1);
		}
		else {
			if (index_list2 <= 9) {
				result[i] = list[index_list2];
				printf("%d added to result from List 2!", result[i]);
				index_list2++;
				printf("\t new index: %d\n", index_list2);
			}
			else {
				result[i] = list[index_list1];
				printf("%d added to result from List 1!", result[i]);
				index_list1++;
				printf("\t new index: %d\n", index_list1);
			}
		}
	}
	printNewLines(2);
	
	pthread_exit(0);
}

// @description: Displays the global array result
// @pre: None
// @post: contents of result is printed on the screen
// @usage: displayArray();
void displayArray(void) {
	printf("The sorted array: ");
	for (int i = 0; i < SIZE; i++) {
		printf("%d ", result[i]);
	}
	printNewLines(2);
}

// @description: prints a desired number of new lines
// @pre: numOfLines contains the desired number of new lines to be printed
// @post: prints the desired number of new lines on the screen
// @usage: printNewLines(2);
void printNewLines(int numOfLines) {
	for (int i = 0; i < numOfLines; i++) {
		printf("\n");
	}
}