#include "model.dataset.h"
#include <stdio.h>
#include <stdlib.h>

dataset *allocateDataset()
{
  dataset *newDataset = (dataset *)malloc(sizeof(dataset));
  if(newDataset == NULL)
    {
      fprintf(stderr, "Memory allocation failed for dataset\n");
      exit(EXIT_FAILURE);
    }
  newDataset->size     = 0;
  newDataset->capacity = 0;
  newDataset->index    = 0;
  newDataset->samples  = NULL;
  return newDataset;
}

void deallocateDataset(dataset *data)
{
  if(data != NULL)
    {
      // for(int i = 0; i < data->size; ++i) { free(data->samples[i]); }
      //free(data->samples);
      free(data);
    }
}

void dataset_add_point(dataset *data, Point *point)
{
  ++data->size;
  if(data->capacity <= sizeof(data) * data->size)
    {
      data->capacity = sizeof(data) * data->size * 2;
      data->samples  = realloc(data->samples, data->capacity);
    }
  data->samples[data->size - 1] = point;
}