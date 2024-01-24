// dataset.h
#ifndef _DATASET_H
#define _DATASET_H

#include "model.point.h"

typedef struct data_array_s
{
  int     size;
  int     capacity;
  int     index;
  Point **samples;
} dataset;

void     deallocateDataset(dataset *data);
dataset *allocateDataset();
void     dataset_add_point(dataset *data, Point *point);

#endif