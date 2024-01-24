
#include "knn.h"
#include "agnewsloader.h"
#include "models.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
extern char *klasses[];
// take a random sample from the test set

void cross_validation(int *good_predictions, int number_of_tests, int k, t_knn *knn, klass_predictor *kp, float *best_accuracy, int *best_k)
{
  *good_predictions = 0;
  int predicted     = 0;
  int actual        = 0;
  int g             = *good_predictions;

  // for each sample in the test set
  for(int i = 0; i < number_of_tests; i++)
    {
      Point *random_sample = knn->testing->samples[i + 100];
      actual               = random_sample->klass;
      predicted            = kp_predict(kp, random_sample, k);
      if(predicted == random_sample->klass) ++g;
      printf("\r" RED "[ %2.2f%% ] " CYN "[ %2.2d ]-[ %-15s ] :: "
             "[ %2.2d ]-[ %-15s ] ::  " BRED "[%2.2f%%]" CRESET,
             (float)i / number_of_tests * 100, predicted, klasses[predicted], actual, klasses[actual], (float)g / (i + 1) * 100);
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
  dataset         *test  = allocateDataset();
  dataset         *train = allocateDataset();
  klass_predictor *kp    = allocateKlassPredictor();

  t_knn *knn = allocateKNN();

  read_agn(TRAIN_PATH, TEST_PATH, train, test);
  knn->training = train;
  knn->testing  = test;
  knn->k        = 2;

  kp_init(kp, knn, klasses, 4, 0);

  int   good_predictions = 0;
  int   number_of_tests  = 100;
  int   best_k           = 0;
  float best_accuracy    = 0.0;

  for(int k = 1; k < 100; k++) { cross_validation(&good_predictions, number_of_tests, k, knn, kp, &best_accuracy, &best_k); }

  // good old cross validation to find best k (multithreaded version!)
  printf("\33[2K\r");
  fflush(stdout);
  printf("\rBest: [%2d], Best Accuracy: %.2f", best_k, best_accuracy);

  // deallocateDataset(test);
  deallocateKlassPredictor(kp);
  // deallocateDataset(train);
  deallocateKNN(knn);
  return 0;
}
