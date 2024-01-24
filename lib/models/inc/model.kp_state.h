#ifndef _KP_STATE_H
#define _KP_STATE_H

#include "model.queue.h"
#include "model.point.h"
#include "model.distancePoint.h"

struct queue_s;

typedef struct kp_state_s
{
  int             train_count;
  Point         **train;
  Point          *testing;
  DistancePoint  *neighbours;
  struct queue_s *taskQueue;
  struct queue_s *predictQueue;
} kp_state;

kp_state *allocateKPState();
void      deallocateKPState(kp_state *state);

#endif