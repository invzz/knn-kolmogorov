#ifndef _MODELS_H
#define _MODELS_H
#include <pthread.h>
#include <stdlib.h>

typedef struct point_s
{
  char *text;
  char *label;
  int klass;
} Point;
typedef struct data_array_s
{
  size_t size;
  size_t capacity;
  size_t index;
  Point **samples;
} dataset;
typedef struct knn_s
{
  int k;
  dataset *training;
  int training_count;
  dataset *testing;
  int testing_size;
} t_knn;
typedef struct distance_s
{
  Point *point;
  double distance;
} DistancePoint;
typedef struct kp_state_s
{
  int train_count;
  Point **train;
  Point *testing;
  DistancePoint *neighbours;
} kp_state;
typedef struct klass_predictor_s
{
  size_t nprocs;
  size_t chunk_size;
  size_t chunk_rem;
  Point **train_samples;
  int train_count;
  pthread_t *threads;
  kp_state **states;
  char **klasses;
  int klasses_count;
  DistancePoint *neighbours;
} klass_predictor;

#endif
