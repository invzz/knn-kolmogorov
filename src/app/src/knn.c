
#include "knn.h"
#include "agnewsloader.h"
#include "models.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

static char *klasses[] = {"world", "sports", "business", "sci/tech"};
char *template =
  REDB "[ %2.2f%% ]" CRESET " :: " RED "[ Neighbours = %zu ]" CRESET " :: " CYN "[ %2.2d ]-[ %-9.9s ]" CRESET " ::"
       " [ %2.2d ]-[ %-9.9s ]" CRESET " :: " BRED " Accuracy [ %2.2f%% ]";

// take a random sample from the test set

void choose_accuracy(int *good_predictions, dataset *test, int number_of_tests, int k, klass_predictor *kp,
                     float *best_accuracy, int *best_k)
{
  *good_predictions = 0;
  int  predicted    = 0;
  int  actual       = 0;
  int  g            = *good_predictions;
  char OUTPUT_BUFFER[BUFSIZ];
  // for each sample in the test set
  for(int i = 0; i < number_of_tests; i++)
    {
      Point p            = (test->samples)[i];
      actual             = p.klass;
      predicted          = kp_predict(kp, &p, k);
      float progress     = (float)i / number_of_tests * 100;
      float cur_accuracy = (float)g / (i + 1) * 100;
      if(predicted == p.klass) ++g;
      sprintf(OUTPUT_BUFFER, template, progress, k, predicted, klasses[predicted], actual, klasses[actual],
              cur_accuracy);
      printf("\33[2K\r%s", OUTPUT_BUFFER);
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
void print_table_footer()
{
  for(int i = 0; i < 100; ++i) { printf("-"); }
  // printf("\33[2K\r");
}

void print_table_header()
{
  print_table_footer();
  printf("\n");
  printf("| %-*s ", 10, "Predicted ");
  printf("| %-*s ", 10, "Actual ");
  printf("| %-*s |\n", 70, "text ");
  print_table_footer();
  printf("\n");
}

void print_table_record(int predicted, int actual, char *text)
{
  printf("| %-*s ", 10, klasses[predicted]);
  printf("| %-*s ", 10, klasses[actual]);
  printf("| %-70.70s |\n", text);
  print_table_footer();
}

void save_csv(FILE *fd, int predicted, char *text) {}

int main(int argc, char **argv)
{
  int   good_predictions = 0;
  int   number_of_tests  = 0;
  int   best_k           = 0;
  float best_accuracy    = 0.0;

  klass_predictor *kp    = allocateKlassPredictor();
  dataset         *test  = get_dataset(TEST_PATH);
  dataset         *train = get_dataset(TRAIN_PATH);
  kp                     = kp_init(train, test, 2);
  kp->klasses            = klasses;
  kp->klasses_count      = 4;

  // calculate best k
  // for(int k = 2; k <= 2; k++)
  //   {
  //     choose_accuracy(&good_predictions, test, number_of_tests, k, kp, &best_accuracy, &best_k);
  //   }
  // printf("\33[2K\r");
  // fflush(stdout);
  // printf(CYN "\33[2K\n[ Best k is " REDB "%2d" CYN " (Acc:" RED " %2.2f%%" CYN ")]" CRESET " :: "
  //            "( Based on " RED "%zu" CRESET " samples )\n",
  //        best_k, best_accuracy, number_of_tests);

  best_k = 2;

  int predicted = 0;

  number_of_tests = test->size;

  FILE *fp = fopen("output.csv", "w");
  fprintf(fp, "predicted,actual,text\n");
  print_table_header();
  int g = 0; // good predictions
  for(int i = 0; i < number_of_tests; i++)
    {
      Point p = (test->samples)[i];
      print_table_record(predicted, p.klass, p.text);
      printf("\33[2K\r" REDB "[ %*d / %*d ]" CRESET "::" YEL " Accuracy : " RED "[ %-2.2f%% ] " CRESET "::" YEL
             " Progress : " RED "[ %-2.2f%% ]" CRESET,
             5, i, 5, number_of_tests, (float)g / (i + 1) * 100, (float)i / number_of_tests * 100);
      fflush(stdout);
      predicted = kp_predict(kp, &p, best_k);
      if(predicted == p.klass) ++g;
      fprintf(fp, "%d,%s\n", predicted, p.text);
      save_csv(fp, predicted, p.text);
      printf("\33[2K\r");
      print_table_footer();
      printf("\n");
    }
  fclose(fp);

  deallocateDataset(test);
  deallocateDataset(train);
  deallocateKlassPredictor(kp);
  return 0;
}
