// distance_point.c
#include "model.distancePoint.h"
#include "model.logging.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

DistancePoint *allocateDistancePoints(int count)
{
  DistancePoint *newDistancePoint = (DistancePoint *)calloc(count, sizeof(DistancePoint));
  memset(newDistancePoint, 0, sizeof(DistancePoint) * count);
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
  DEBUG_PRINT("\r[ Allocated ]   ::  DistancePoints [%*d] \n", 5, count);
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