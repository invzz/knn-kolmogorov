
#include "knn.h"
#include "agnewsloader.h"
#include "models.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
extern char *klasses[];

#define malloc my_alloc
// take a random sample from the test set

void cross_validation(int *good_predictions, dataset *test, int number_of_tests, int k, klass_predictor *kp,
                      float *best_accuracy, int *best_k)
{
  *good_predictions = 0;
  int predicted     = 0;
  int actual        = 0;
  int g             = *good_predictions;

  // for each sample in the test set
  for(int i = 0; i < number_of_tests; i++)
    {
      Point p   = (test->samples)[i + 100];
      actual    = p.klass;
      predicted = kp_predict(kp, &p, k);
      if(predicted == p.klass) ++g;
      printf("\r" RED "[ %2.2f%% ] " CYN "[ %2.2d ]-[ %-15s ] :: "
             "[ %2.2d ]-[ %-15s ] ::  " BRED "[%2.2f%%]" CRESET,
             (float)i / number_of_tests * 100, predicted, klasses[predicted], actual, klasses[actual],
             (float)g / (i + 1) * 100);
      fflush(stdout);
    }

  // calculate accuracy
  float accuracy = (float)g / number_of_tests;

  // update best k
  if(accuracy > *best_accuracy)
    {
      *best_accuracy = accuracy;
      *best_k        = k;
    }
}

int main(int argc, char **argv)
{
  klass_predictor *kp = allocateKlassPredictor();

  // t_knn *knn = allocateKNN();

  dataset *test  = get_dataset(TEST_PATH);
  dataset *train = get_dataset(TRAIN_PATH);

  // knn->training = train;
  // knn->testing  = test;
  // knn->k        = 2;

  kp                = kp_init(train, test, 2);
  kp->klasses       = klasses;
  kp->klasses_count = 4;

  int   good_predictions = 0;
  int   number_of_tests  = 500;
  int   best_k           = 0;
  float best_accuracy    = 0.0;

  for(int k = 3; k <= 10; k++)
    {
      cross_validation(&good_predictions, test, number_of_tests, k, kp, &best_accuracy, &best_k);
    }

  printf("\33[2K\r");
  fflush(stdout);
  printf("\rBest: [%2d], Best Accuracy: %.2f", best_k, best_accuracy);

  deallocateDataset(test);
  deallocateDataset(train);
  deallocateKlassPredictor(kp);
  return 0;
}
