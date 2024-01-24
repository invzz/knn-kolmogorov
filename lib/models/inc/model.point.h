#ifndef _POINT_H
#define _POINT_H

typedef struct point_s
{
  char *text;
  char *label;
  int   klass;
} Point;

Point *allocatePoint();
void   deallocatePoint(Point *point);

#endif