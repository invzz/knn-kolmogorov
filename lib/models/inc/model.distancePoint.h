// distance_point.h
#ifndef _DISTANCE_POINT_H
#define _DISTANCE_POINT_H

#include "model.point.h"
#define deallocateDistancePoint(distancePoint) deallocateDistancePoints(distancePoint, 1)
typedef struct distance_s
{
  Point *point;
  double distance;
} DistancePoint;

DistancePoint *allocateDistancePoint();
void           deallocateDistancePoints(DistancePoint *distancePoint, int count);

#endif