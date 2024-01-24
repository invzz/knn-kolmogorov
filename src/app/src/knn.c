
#include "knn.h"
#include "agnewsloader.h"
#include "models.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
extern char *klasses[];
// take a random sample from the test set

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

  // good old cross validation to find best k (multithreaded version!)
  for(int k = 2; k <= 2; k++)
    {
      knn->k           = k;
      good_predictions = 0;
      int predicted    = 0;
      int actual       = 0;

      // for each sample in the test set
      for(int i = 0; i < number_of_tests; i++)
        {
          Point *random_sample = knn->testing->samples[i + 100];
          actual               = random_sample->klass;
          predicted            = kp_predict(kp, random_sample, knn->k);
          if(predicted == random_sample->klass) good_predictions++;
          printf("\r" RED "[%2.2f%%] " WHT "Predicted: [%d][%-10.15s] <::> [%d][%-10.10s] ::" BRED "[%2.2f%%]" CRESET, (float)i / number_of_tests * 100, predicted, klasses[predicted], actual, klasses[actual], (float)good_predictions / (i + 1) * 100);
          fflush(stdout);
        }

      for(int i = 0; i < kp->nprocs; ++i)
        {
          thread_message msg;
          msg.message = THREAD_MESSAGE_KILL;
          enqueue(kp->states[i]->predictQueue, &msg);
        }

      // calculate accuracy
      float accuracy = (float)good_predictions / number_of_tests;

      // update best k
      if(accuracy > best_accuracy)
        {
          best_accuracy = accuracy;
          best_k        = k;
        }

      printf("\nDone.\n");
    }
  printf("\33[2K\r");
  fflush(stdout);
  printf("\rBest: [%2d], Best Accuracy: %.2f", best_k, best_accuracy);

  // deallocateDataset(test);
  deallocateKlassPredictor(kp);
  // deallocateDataset(train);
  deallocateKNN(knn);
  return 0;
}
