
#ifndef KNN_LIB_H
#define KNN_LIB_H
#include "models.h"
#if __linux__
int min_num(int a, int b) { return a < b ? a : b; }
int max_num(int a, int b) { return a > b ? a : b; }
#endif
typedef struct
{
  kp_state *state;
  Point    *point;
} stateMessage;

typedef struct
{
  DistancePoint *neighbours;
} resultMessage;

klass_predictor *kp_init(dataset *train, dataset *test, int k);
size_t           kp_predict(klass_predictor *kp, Point *text, size_t k);
void            *klassify_thread(void *args);
float            ncd(Point *a, Point *b);
int              kolmogorov_distance(char *text);
int              compare(const void *a, const void *b);

#endif