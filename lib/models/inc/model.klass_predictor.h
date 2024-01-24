#ifndef _KLASS_PREDICTOR_H
#define _KLASS_PREDICTOR_H

#include "model.point.h"
#include "model.kp_state.h"
#include "model.distancePoint.h"
#include <pthread.h>
typedef struct klass_predictor_s
{
  int            nprocs;
  int            chunk_size;
  int            chunk_rem;
  Point        **train_samples;
  int            train_count;
  pthread_t     *threads;
  kp_state     **states;
  char         **klasses;
  int            klasses_count;
  DistancePoint *neighbours;
} klass_predictor;

klass_predictor *allocateKlassPredictor();
void             deallocateKlassPredictor(klass_predictor *predictor);

#endif