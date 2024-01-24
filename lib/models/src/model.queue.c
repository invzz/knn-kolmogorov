#include "model.queue.h"
#include <stdio.h>
#include <stdlib.h>

Queue *createQueue(int capacity)
{
  Queue *queue    = (Queue *)malloc(sizeof(Queue));
  queue->array    = (QueueNode *)malloc(capacity * sizeof(QueueNode));
  queue->capacity = capacity;
  queue->front    = 0;
  queue->size     = 0;
  queue->rear     = capacity - 1;
  return queue;
}

bool isFull(Queue *queue) { return (queue->size == queue->capacity); }

bool isEmpty(Queue *queue) { return (queue->size == 0); }

void enqueue(Queue *queue, thread_message *data)
{
  if(data == NULL) return;
  if(isFull(queue)) return;

  queue->rear                    = (queue->rear + 1) % queue->capacity;
  queue->array[queue->rear].data = data;
  queue->size                    = queue->size + 1;
  //   printf("[%d] enqueued to queue : [%20.20s]\n", queue->size + 1, data->text);
}

thread_message *dequeue(Queue *queue)
{
  if(queue == NULL) return NULL;
  thread_message *data = NULL;

  if(!isEmpty(queue))
    {
      data         = queue->array[queue->front].data;
      queue->front = (queue->front + 1) % queue->capacity;
      queue->size  = queue->size - 1;
    }
  return data;
}