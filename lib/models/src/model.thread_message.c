#include "model.thread_message.h"
#include "model.logging.h"
#include <stdio.h>
#include <stdlib.h>

thread_message *allocateThreadMessage()
{
  DEBUG_PRINT("\r[ Allocated ]   ::  thread_message\n");
  thread_message *newMessage = (thread_message *)malloc(sizeof(thread_message));
  if(newMessage == NULL)
    {
      fprintf(stderr, "Memory allocation failed for thread_message\n");
      exit(EXIT_FAILURE);
    }
  newMessage->message = 0;
  newMessage->data    = NULL;
  return newMessage;
}

void deallocateThreadMessage(thread_message *message)
{
  DEBUG_PRINT("Deallocating thread_message\n");
  //  free(message->data);
  free(message);
}