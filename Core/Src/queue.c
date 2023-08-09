
#include "queue.h"

#include "logSTM.h"

/*-------------------------------------------------*/
//Global Function
/*-------------------------------------------------*/

void initQueue(Queue* queue)
{
    queue->front = 0;
    queue->rear = -1;
    queue->count = 0;
}

int isQueueEmpty(Queue* queue)
{
    return (queue->count == 0);
}

int isQueueFull(Queue* queue)
{
    return (queue->count == QUEUE_SIZE);
}

void enqueue(Queue* queue, const char* item)
{
    if (isQueueFull(queue))
    {
    	logI("Queue is full. Unable to enqueue.\n");
        return;
    }
    if (strlen(item) > STRING_LENGTH)
    {
        logI("enqueue string length exceeds maximum length.\n");
        return;
    }

    queue->rear = (queue->rear + 1) % QUEUE_SIZE;
    memset(queue->buffer[queue->rear],0,STRING_LENGTH);
    strncpy(queue->buffer[queue->rear], item, STRING_LENGTH);
    queue->count++;
}

char* dequeue(Queue* queue)
{
    if (isQueueEmpty(queue))
    {
    	logI("Queue is empty. Unable to dequeue.\n");
        return NULL;
    }

    char* item = queue->buffer[queue->front];
    queue->front = (queue->front + 1) % QUEUE_SIZE;

    queue->count--;

    return item;
}
