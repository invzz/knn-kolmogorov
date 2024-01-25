// point.c
#include "model.point.h"
#include "model.logging.h"
#include <stdio.h>
#include <stdlib.h>

Point *allocatePoint()
{
  DEBUG_PRINT("\r[ Allocated ]   ::  Point\n");
  Point *newPoint = (Point *)malloc(sizeof(Point));
  if(newPoint == NULL)
    {
      fprintf(stderr, "Memory allocation failed for Point\n");
      exit(EXIT_FAILURE);
    }
  newPoint->text  = NULL;
  newPoint->klass = 0;
  return newPoint;
}

void deallocatePoints(Point *point, int count)
{
  DEBUG_PRINT("Deallocating Points\n");
  for(int i = 0; i < count; i++) free(point[i].text);
  free(point);
}