#include "knn.h"
#include "data-handler.h"
#include "models.h"
#include <assert.h>
#include <data-handler.h>
#include <float.h>
#include <pthread.h>
#include <stdio.h>
#include <string.h>
#include <sys/sysinfo.h>
#include <zlib.h>

int main(int argc, char **argv) {
  dataset *data = gedataset_from_file(argv[1]);
  char **categories = get_klass_array();
  int category_count = get_klass_count();
  t_knn *knn = malloc(sizeof(t_knn));
  divide_dataset(data, knn, 0.9); // Set your desired training ratio here
  knn->k = 5;
  klass_predictor *kp = malloc(sizeof(klass_predictor));
  kp_init(kp, knn, categories, category_count);
  for (int i = 0; i < 4; i++) {
    kp_predict(kp, knn->testing->points[i], knn->k);

    printf("--------------------\n");
  }
  free(knn);
  free(data);
  free(kp);
  return 0;
}
