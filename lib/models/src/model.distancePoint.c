// distance_point.c
#include "model.distancePoint.h"
#include "model.logging.h"
#include <stdio.h>
#include <stdlib.h>

DistancePoint *allocateDistancePoints(int count)
{
  DEBUG_PRINT("\r[ Allocated ]   ::  DistancePoint\n");
  DistancePoint *newDistancePoint = (DistancePoint *)calloc(count, sizeof(DistancePoint));
  if(newDistancePoint == NULL)
    {
      fprintf(stderr, "Memory allocation failed for DistancePoint\n");
      exit(EXIT_FAILURE);
    }
  for(int i = 0; i < count; i++)
    {
      newDistancePoint[i].point    = NULL;
      newDistancePoint[i].distance = 0.0;
    }
  return newDistancePoint;
}
DistancePoint *allocateDistancePoint()
{
  printf("\r[ Allocated ]   ::  DistancePoint\n");
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

void deallocateDistancePoints(DistancePoint *distancePoint, int count)
{
  DEBUG_PRINT("Deallocating DistancePoint\n");
  deallocatePoint(distancePoint->point);
  free(distancePoint);
}