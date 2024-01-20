
#include "knn.h"
#include <assert.h>
#include <float.h>
#include <pthread.h>
#include <stdio.h>
#include <string.h>
#include <sys/sysinfo.h>
#include <zlib.h>
int min(int a, int b) { return a < b ? a : b; }
int max(int a, int b) { return a > b ? a : b; }
int compare(const void *a, const void *b) {
  DistancePoint *da = (DistancePoint *)a;
  DistancePoint *db = (DistancePoint *)b;
  if (da->distance < db->distance)
    return -1;
  if (da->distance > db->distance)
    return 1;
  return 0;
}

int kolmogorov_distance(char *text) {
  Bytef *b = (Bytef *)text;
  size_t size = strlen(text);
  size_t output_size = compressBound(size);
  char *output = malloc(output_size);
  compress(output, &output_size, text, size);
  free(output);
  return output_size;
}

float ncd(Point *a, Point *b) {
  t_sample *sample1 = a->data;
  t_sample *sample2 = b->data;

  // concactenate headline and short description
  char *sample1_text = malloc(strlen(sample1->headline) +
                              strlen(sample1->short_description) + 1);
  strcpy(sample1_text, sample1->headline);
  strcat(sample1_text, sample1->short_description);

  // same for sample2
  char *sample2_text = malloc(strlen(sample2->headline) +
                              strlen(sample2->short_description) + 1);
  strcpy(sample2_text, sample2->headline);
  strcat(sample2_text, sample2->short_description);

  // concatenate both texts
  char *text = malloc(strlen(sample1_text) + strlen(sample2_text) + 1);
  strcpy(text, sample1_text);
  strcat(text, sample2_text);

  int cX1 = kolmogorov_distance(sample1_text);
  int cX2 = kolmogorov_distance(sample2_text);
  int cX1X2 = kolmogorov_distance(text);

  // release memory
  free(sample1_text);
  free(sample2_text);
  free(text);

  // print formula calculation
  float mini = min(cX1, cX2);
  float maxi = max(cX1, cX2);

  // calculate the distance between the two deflated texts
  float distance = 0;

  distance = (cX1X2 - mini) / maxi;

  return distance;
}

void divide_dataset(dataset *data, t_knn *knn, float training_ratio) {
  // Calculate the number of samples for training and testing
  int training_count = data->size * training_ratio;
  int testing_size = data->size - training_count;
  knn->k = 2; // Set your desired value for k here
  knn->training = malloc(sizeof(dataset));
  knn->testing = malloc(sizeof(dataset));
  knn->training->size = training_count;
  knn->training->points = malloc(training_count * sizeof(Point));
  knn->testing->size = testing_size;
  knn->testing->points = malloc(testing_size * sizeof(Point));
  // Copy samples from the original dataset to the training and testing datasets
  for (int i = 0; i < training_count; i++) {
    knn->training->points[i] = data->points[i];
  }
  for (int i = 0; i < testing_size; i++) {
    knn->testing->points[i] = data->points[i + training_count];
  }
}

void *klassify_thread(void *args) {
  kp_state *state = (kp_state *)args;
  dataset *data = malloc(sizeof(dataset));
  data->size = state->train_count;
  data->points = state->train;

  for (int i = 0; i < state->train_count; i++) {
    state->neighbours[i].distance = ncd(state->testing, data->points[i]);
    state->neighbours[i].point = data->points[i];
  }
}

void kp_init(klass_predictor *kp, t_knn *knn, char **categories,
             int category_count) {
  int trsize = knn->training->size;
  // get amount of cores on the machine
  int nprocs = get_nprocs();
  kp_state **states = malloc(nprocs * sizeof(kp_state));
  assert(states != NULL);
  int chunk_size = knn->training->size / nprocs;
  int chunk_rem = knn->training->size % nprocs;
  memset(states, 0, sizeof(kp_state) * nprocs);
  kp->klasses = categories;
  kp->klasses_count = category_count;
  kp->nprocs = nprocs;
  kp->chunk_size = chunk_size;
  kp->chunk_rem = chunk_rem;
  kp->train_samples = knn->training->points;
  kp->train_count = knn->training->size;
  kp->threads = malloc(kp->nprocs * sizeof(pthread_t));
  assert(kp->threads != NULL);
  memset(kp->threads, 0, kp->nprocs * sizeof(pthread_t));
  kp->states = malloc(kp->nprocs * sizeof(kp_state *));
  assert(kp->states != NULL);
  memset(kp->states, 0, kp->nprocs * sizeof(kp_state));
  kp->neighbours = malloc(sizeof(DistancePoint) * trsize);
  assert(kp->neighbours != NULL);
  memset(kp->neighbours, 0, kp->nprocs * sizeof(kp_state));
}

int *compute_frequencies(klass_predictor *kp, size_t k) {
  int klass_count = kp->klasses_count; // get the number of classes
  int *neighbourhood = malloc(
      sizeof(int) *
      klass_count); // create an array to store the frequency of each class
  assert(neighbourhood != NULL);
  // initialize neighbourhood to 0
  memset(neighbourhood, 0, sizeof(int) * klass_count);
  // compute frequency of each class
  for (int i = 0; i < k; ++i) {
    neighbourhood[kp->neighbours[i].point->klass] += 1;
  }
  return neighbourhood;
}

int mode(int *neighbourhood, int klass_count) {
  int max = 0;
  int index;
  for (int i = 0; i < klass_count; ++i) {
    if (neighbourhood[i] > max) {
      max = neighbourhood[i];
      index = i;
    }
  }
  return index;
}

void kp_init_states(klass_predictor *kp, Point *text) {
  for (size_t i = 0; i < kp->nprocs; ++i) {
    kp->states[i] = malloc(sizeof(kp_state));
    kp->states[i]->train = kp->train_samples + i * kp->chunk_size;
    kp->states[i]->train_count = kp->chunk_size;
    if (i == kp->nprocs - 1) {
      kp->states[i]->train_count += kp->chunk_rem;
    }
    kp->states[i]->testing = text;
    kp->states[i]->neighbours =
        malloc(sizeof(DistancePoint) * kp->states[i]->train_count);
    memset(kp->states[i]->neighbours, 0,
           sizeof(DistancePoint) * kp->states[i]->train_count);
    if (pthread_create(&kp->threads[i], NULL, klassify_thread, kp->states[i]) !=
        0) {
      exit(1);
    }
  }

  for (size_t i = 0; i < kp->nprocs; ++i) {
    pthread_join(kp->threads[i], NULL);
  }
  free(kp->threads);
}

size_t kp_predict(klass_predictor *kp, Point *text, size_t k) {
  printf("[ INPUT  ] : [ %-2.2d ] :: [ %-10.10s ] => %s\n", text->klass,
         text->data->category, text->data->headline);

  // initialize states and run compression threads
  kp_init_states(kp, text);

  // merging all neighbours calculated by each thread
  for (size_t i = 0; i < kp->nprocs; ++i) {
    for (size_t j = 0; j < kp->states[i]->train_count; ++j) {
      kp->neighbours[i * kp->chunk_size + j] = kp->states[i]->neighbours[j];
      kp->neighbours[i * kp->chunk_size + j].point =
          kp->states[i]->neighbours[j].point;
    }
  }

  qsort(kp->neighbours, kp->train_count, sizeof(DistancePoint), compare);

  int *neighbourhood = compute_frequencies(kp, k);
  int predicted_klass = mode(neighbourhood, kp->klasses_count);
  char *predicted_klass_name = kp->klasses[predicted_klass];

  printf("[ OUTPUT ] : [ %2.2d ] :: [ %-10.10s ]\n", predicted_klass,
         predicted_klass_name);
}
