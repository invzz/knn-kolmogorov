#ifndef _KLASS_PREDICTOR_H
#define _KLASS_PREDICTOR_H

#include "model.point.h"
#include "model.kp_state.h"
#include "model.distancePoint.h"
#include <pthread.h>
typedef struct klass_predictor_s
{
  char         **klasses;
  int            klasses_count;
  int            number_of_cores;
  int            train_count;
  int            chunk_size;
  int            chunk_rem;
  pthread_t     *threads;
  kp_state      *states;
  Point         *train_samples;
  DistancePoint *neighbours;
} klass_predictor;

klass_predictor *allocateKlassPredictor();
void             deallocateKlassPredictor(klass_predictor *predictor);

#endif