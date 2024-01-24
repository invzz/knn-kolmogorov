
#ifndef KNN_LIB_H
#define KNN_LIB_H
#include "models.h"

size_t kp_predict(klass_predictor *kp, Point *text, size_t k);
void   kp_init(klass_predictor *kp, t_knn *knn, char **categories, int category_count, int nprocs);
void  *klassify_thread(void *args);
float  ncd(Point *a, Point *b);
int    kolmogorov_distance(char *text);
int    compare(const void *a, const void *b);
int    max(int a, int b);
int    min(int a, int b);
#endif