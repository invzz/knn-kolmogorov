// distance_point.h
#ifndef _DISTANCE_POINT_H
#define _DISTANCE_POINT_H

#include "model.point.h"

typedef struct distance_s
{
  Point *point;
  double distance;
} DistancePoint;

DistancePoint *allocateDistancePoint();
void           deallocateDistancePoint(DistancePoint *distancePoint);

#endif