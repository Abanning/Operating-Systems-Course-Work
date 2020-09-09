// @filename: buffer.h
// @author: Alex Banning
// @assignment: Project 5 - Buffer | Consumer and Producer Threads
// @date: November 8th, 2019
// @description: buffer.c header file

#ifndef BUFFER_H
#define BUFFER_H

typedef int buffer_item;

// @description: producer thread. Adds items to buffer.
// @pre: None
// @post: After sleeping a random number of seconds, will add an item to the buffer
//        as long as the buffer is not full
// @usage: pthread_create(&producers[i], 0, producer, (void *) &i);
void *producer(void *param);

// @description: consumer thread. Removes items from the buffer
// @pre: None
// @post: After sleeping a random number of seconds, will remove an item from the buffer
//        as long as the buffer is not empty
// @usage: pthread_create(&consumers[j], 0, consumer, (void *) &j);
void *consumer(void *param);

// @description: Inserts an item in the buffer
// @pre: item contains a buffer item to be added
// @post: adds item to the buffer
// @usage: if(!insert_item(item)) {...;}
int insert_item(buffer_item item);

// @description: Removes an item from the buffer
// @pre: None
// @post: returns the item removed from the buffer to item
// @usage: if(!remove_item(&item)) {...;}
int remove_item(buffer_item *item);

// @description: prints a desired number of new lines
// @pre: desiredNumber contains the desired number of new lines to be printed
// @post: prints the desiredNumber of new lines on the screen
// @usage: printNewLines(2);
void printNewLines(int desiredNumber);

#endif