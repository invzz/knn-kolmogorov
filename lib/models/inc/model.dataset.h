// dataset.h
#ifndef _DATASET_H
#define _DATASET_H

#include "model.point.h"

typedef struct data_array_s
{
  int    size;
  int    capacity;
  Point *samples;
} dataset;

void     deallocateDataset(dataset *data);
dataset *allocateDataset(int capacity);
void     dataset_add_point(dataset *data, Point *point);

#endif