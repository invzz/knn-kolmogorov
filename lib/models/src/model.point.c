// point.c
#include "model.point.h"
#include <stdio.h>
#include <stdlib.h>

Point *allocatePoint()
{
  Point *newPoint = (Point *)malloc(sizeof(Point));
  if(newPoint == NULL)
    {
      fprintf(stderr, "Memory allocation failed for Point\n");
      exit(EXIT_FAILURE);
    }
  newPoint->text  = NULL;
  newPoint->label = NULL;
  newPoint->klass = 0;
  return newPoint;
}

void deallocatePoint(Point *point)
{
  if(point != NULL)
    {
      free(point->text);
      free(point->label);
      free(point);
    }
}