/*
 * queue.h
 *
 *  Created on: Aug 4, 2023
 *      Author: user
 */

#ifndef INC_QUEUE_H_
#define INC_QUEUE_H_

#define QUEUE_SIZE 10
#define STRING_LENGTH 20

#include <stdio.h>
#include <string.h>

typedef struct {
    char buffer[QUEUE_SIZE][STRING_LENGTH];
    int front;
    int rear;
    int count;
} Queue;


void initQueue(Queue* queue);
int isQueueEmpty(Queue* queue);
int isQueueFull(Queue* queue);
void enqueue(Queue* queue, const char* item);
char* dequeue(Queue* queue);

#endif /* INC_QUEUE_H_ */
