
#include "models.h"

size_t kp_predict(klass_predictor *kp, Point *text, size_t k);
void kp_init(klass_predictor *kp, t_knn *knn, char **categories,
             int category_count);
void *klassify_thread(void *args);
void divide_dataset(dataset *data, t_knn *knn, float training_ratio);
float ncd(Point *a, Point *b);
int kolmogorov_distance(char *text);
int compare(const void *a, const void *b);
int max(int a, int b);
int min(int a, int b);
