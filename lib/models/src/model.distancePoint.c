// distance_point.c
#include "model.distancePoint.h"
#include <stdio.h>
#include <stdlib.h>

DistancePoint *allocateDistancePoint()
{
  DistancePoint *newDistancePoint = (DistancePoint *)malloc(sizeof(DistancePoint));
  if(newDistancePoint == NULL)
    {
      fprintf(stderr, "Memory allocation failed for DistancePoint\n");
      exit(EXIT_FAILURE);
    }
  newDistancePoint->point    = NULL;
  newDistancePoint->distance = 0.0;
  return newDistancePoint;
}

void deallocateDistancePoint(DistancePoint *distancePoint) { free(distancePoint); }