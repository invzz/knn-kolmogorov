#ifndef QUEUE_H
#define QUEUE_H
#define QUEUE_SIZE 1000

#include "model.thread_message.h"

#include <stdbool.h>

struct thread_message_s;

typedef struct queue_node_s
{
  struct thread_message_s *data;
} QueueNode;

typedef struct queue_s
{
  QueueNode *array;
  int        capacity;
  int        front;
  int        rear;
  int        size;
  int        end;
} Queue;
Queue                   *createQueue(int capacity);
bool                     isFull(Queue *queue);
bool                     isEmpty(Queue *queue);
void                     enqueue(Queue *queue, struct thread_message_s *data);
struct thread_message_s *dequeue(Queue *queue);

#endif // QUEUE_H