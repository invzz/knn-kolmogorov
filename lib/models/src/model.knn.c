// knn.c
#include "model.knn.h"
#include <stdio.h>
#include <stdlib.h>

t_knn *allocateKNN()
{
  t_knn *newKNN = (t_knn *)malloc(sizeof(t_knn));
  if(newKNN == NULL)
    {
      fprintf(stderr, "Memory allocation failed for t_knn\n");
      exit(EXIT_FAILURE);
    }
  newKNN->k              = 0;
  newKNN->training       = NULL;
  newKNN->training_count = 0;
  newKNN->testing        = NULL;
  newKNN->testing_size   = 0;
  return newKNN;
}

void deallocateKNN(t_knn *knn)
{
  if(knn != NULL)
    {
      deallocateDataset(knn->training);
      deallocateDataset(knn->testing);
      free(knn);
    }
}