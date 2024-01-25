#include "model.dataset.h"
#include "model.logging.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "model.logging.h"
dataset *allocateDataset(int capacity)
{
  DEBUG_PRINT("\r[ Allocated ]   ::  Dataset\n");
  dataset *newDataset = (dataset *)malloc(sizeof(dataset));
  if(newDataset == NULL)
    {
      fprintf(stderr, "Memory allocation failed for dataset\n");
      exit(EXIT_FAILURE);
    }
  newDataset->size     = 0;
  newDataset->capacity = capacity;
  newDataset->index    = 0;
  newDataset->samples  = (Point *)calloc(capacity, sizeof(Point));
  if(newDataset->samples == NULL)
    {
      fprintf(stderr, "Memory allocation failed for dataset samples\n");
      exit(EXIT_FAILURE);
    }
  return newDataset;
}

void deallocateDataset(dataset *data)
{
  DEBUG_PRINT("Deallocating Dataset\n");
  deallocatePoints(data->samples, data->size);
  free(data);
}
