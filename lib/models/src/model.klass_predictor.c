#include "model.klass_predictor.h"
#include <stdio.h>
#include <stdlib.h>

klass_predictor *allocateKlassPredictor()
{
  klass_predictor *newPredictor = (klass_predictor *)malloc(sizeof(klass_predictor));
  if(newPredictor == NULL)
    {
      fprintf(stderr, "Memory allocation failed for klass_predictor\n");
      exit(EXIT_FAILURE);
    }
  newPredictor->nprocs        = 0;
  newPredictor->chunk_size    = 0;
  newPredictor->chunk_rem     = 0;
  newPredictor->train_count   = 0;
  newPredictor->klasses_count = 0;
  newPredictor->threads       = NULL;
  newPredictor->states        = NULL;
  newPredictor->klasses       = NULL;
  newPredictor->train_samples = NULL;
  newPredictor->neighbours    = NULL;
  return newPredictor;
}
void deallocateKlassPredictor(klass_predictor *predictor)
{
  //deallocateDistancePoint(predictor->neighbours);
  free(predictor->threads);
  deallocateKPState(predictor->states);
  free(predictor);
}