

#define TRAIN_PATH "/home/invzz/git/KNN/resources/train.csv"
#define TEST_PATH "/home/invzz/git/KNN/resources/test.csv"

#include "knn.h"
#include "agnewsloader.h"
#include "models.h"
#include <stdio.h>
#include <time.h>
extern char *klasses[];
// take a random sample from the test set
Point *pick_random(t_knn *kp)
{
  srand(time(NULL)); // Seed the random number generator
  int random_index
    = rand()
      % kp->testing->size; // Generate a random index within the test set size
  return kp->testing->samples[random_index];
}

int main(int argc, char **argv)
{
  dataset *test = malloc(sizeof(dataset));
  dataset *train = malloc(sizeof(dataset));
  klass_predictor *kp = malloc(sizeof(klass_predictor));

  t_knn *knn = malloc(sizeof(t_knn));

  read_agn(TRAIN_PATH, TEST_PATH, train, test);
  knn->training = train;
  knn->testing = test;
  knn->k = 2;

  kp_init(kp, knn, klasses, 5);

  int good_predictions = 0;
  int number_of_tests = 500;
  int best_k = 0;
  float best_accuracy = 0.0;

  // good old cross validation to find best k (multithreaded version!)
  for(int k = 2; k <= 10; k++)
    {
      printf("\rCalculating [%2d]  Best: [%2d], Best Accuracy: %.2f", k,
             best_k, best_accuracy);
      fflush(stdout);
      knn->k = k;
      good_predictions = 0;
      int predicted = 0;
      int actual = 0;

      // for each sample in the test set
      for(int i = 0; i < number_of_tests; i++)
        {
          Point *random_sample = knn->testing->samples[i];

          printf(
            "\rCalculating [%2d]  Best: [%2d %.2f%%], %d/%d :: [%2.2d] :: "
            "[%10.10s] [%.2f%%] -",
            k, best_k, best_accuracy * 100, i + 1, number_of_tests,
            random_sample->klass, random_sample->text,
            (float)good_predictions / (i + 1) * 100);

          fflush(stdout);

          actual = random_sample->klass;
          predicted = kp_predict(kp, random_sample, knn->k);
          if(predicted == random_sample->klass)
            good_predictions++;
        }

      // calculate accuracy
      float accuracy = (float)good_predictions / number_of_tests;

      // update best k
      if(accuracy > best_accuracy)
        {
          best_accuracy = accuracy;
          best_k = k;
        }

      printf("Done.\n");
    }
  // clean the line
  printf("\33[2K\r");
  fflush(stdout);

  // print the best k and accuracy
  printf("\rBest: [%2d], Best Accuracy: %.2f", best_k, best_accuracy);

  free(knn);
  free(kp);
  return 0;
}
