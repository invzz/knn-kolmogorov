// knn.h
#ifndef _KNN_H
#define _KNN_H

#include "model.dataset.h"

typedef struct knn_s
{
  int      k;
  dataset *training;
  int      training_count;
  dataset *testing;
  int      testing_size;
} t_knn;

t_knn *allocateKNN();
void   deallocateKNN(t_knn *knn);

#endif