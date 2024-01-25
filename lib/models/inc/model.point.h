#ifndef _POINT_H
#define _POINT_H
#define deallocatePoint(point) deallocatePoints(point, 1)
typedef struct point_s
{
  char *text;
  int   klass;
} Point;

Point *allocatePoint();
void   deallocatePoints(Point *point, int count);

#endif