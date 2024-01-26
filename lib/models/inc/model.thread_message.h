// thread_message.h
#ifndef _THREAD_MESSAGE_H
#define _THREAD_MESSAGE_H

#include "model.point.h"
#include "model.kp_state.h"

#define THREAD_MESSAGE_COMPUTE_NCDS 100
#define THREAD_MESSAGE_NCDS_DONE    101
#define THREAD_MESSAGE_STOP         102
#define THREAD_MESSAGE_KILL         103
#define THREAD_MESSAGE_UPDATE       104

struct kp_state_s;

typedef struct thread_message_s
{
  int   message;
  void *data;
} thread_message;

thread_message *allocateThreadMessage();
void            deallocateThreadMessage(thread_message *message);

#endif