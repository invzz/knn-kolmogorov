#ifndef _MODELS_H
#define _MODELS_H
#include <stdlib.h>

typedef struct record_s {
  char *category;
  char *headline;
  char *short_description;
  char *authors;
  char *link;
  char *pubDate;

} t_sample;

typedef struct point_s {
  t_sample *data;
  int klass;

} Point;

typedef struct data_array_s {
  size_t size;
  size_t capacity;
  size_t index;
  Point **points;
} dataset;
typedef struct knn {
  int k;
  dataset *training;
  int training_count;
  dataset *testing;
  int testing_size;

} t_knn;
typedef struct {
  Point *point;
  double distance;
} DistancePoint;
typedef struct {
  Point **train;
  Point *testing;
  int train_count;
  DistancePoint *neighbours;
} kp_state;
typedef struct klass_predictor_s {
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
