
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

int compare(const void *a, const void *b)
{
  DistancePoint *da = (DistancePoint *)a;
  DistancePoint *db = (DistancePoint *)b;
  if(da->distance < db->distance)
    return -1;
  if(da->distance > db->distance)
    return 1;
  return 0;
}

int kolmogorov_distance(char *text)
{
  Bytef *b = (Bytef *)text;
  size_t size = strlen(text);
  size_t output_size = compressBound(size);
  char *output = malloc(output_size * sizeof(char));

  compress((Bytef *)output, &output_size, (Bytef *)text, size);
  free(output);
  return output_size;
}

float ncd(Point *a, Point *b)
{
  char *buffer = malloc(sizeof(char) * BUFSIZ);
  int cX1 = kolmogorov_distance(a->text);
  int cX2 = kolmogorov_distance(b->text);
  strcat(buffer, a->text);
  strcat(buffer, b->text);
  int cX1X2 = kolmogorov_distance(buffer);
  float mini = min(cX1, cX2);
  float maxi = max(cX1, cX2);
  float return_value = (cX1X2 - mini) / maxi;
  free(buffer);
  return return_value;
}

void *klassify_thread(void *args)
{
  kp_state *state = (kp_state *)args;
  dataset *data = malloc(sizeof(dataset));
  data->size = state->train_count;
  data->samples = state->train;
  state->neighbours = malloc(sizeof(DistancePoint) * state->train_count);
  // printf("\r\033[K"); // Clear the current line in stdout
  // fflush(stdout);
  for(int i = 0; i < state->train_count; i++)
    {
      state->neighbours[i].distance = ncd(state->testing, data->samples[i]);
      state->neighbours[i].point = data->samples[i];
    }
  return NULL;
}

void kp_init(klass_predictor *kp, t_knn *knn, char **categories,
             int category_count)
{
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
  kp->train_samples = knn->training->samples;
  kp->train_count = knn->training->size;
  kp->threads = malloc(kp->nprocs * sizeof(pthread_t));
  kp->states = malloc(kp->nprocs * sizeof(kp_state *));
  kp->neighbours = malloc(sizeof(DistancePoint) * trsize);
  assert(kp->neighbours != NULL);
  memset(kp->neighbours, 0, kp->nprocs * sizeof(kp_state));
}

int *compute_frequencies(klass_predictor *kp, size_t k)
{
  int klass_count = kp->klasses_count; // get the number of classes
  int *neighbourhood = malloc(
    sizeof(int)
    * klass_count); // create an array to store the frequency of each class
  assert(neighbourhood != NULL);
  // initialize neighbourhood to 0
  memset(neighbourhood, 0, sizeof(int) * klass_count);
  // compute frequency of each class
  for(int i = 0; i < k && i < kp->chunk_size; ++i)
    {
      neighbourhood[kp->neighbours[i].point->klass] += 1;
    }
  return neighbourhood;
}

int mode(int *neighbourhood, klass_predictor *kp)
{
  int max = 0;
  int index = 0;
  for(int i = 0; i < kp->klasses_count; ++i)
    {
      if(neighbourhood[i] == max)
        {
          // get the nearest point to the testing point
          if(kp->neighbours[i].distance < kp->neighbours[max].distance)
            {
              max = neighbourhood[i];
              index = i;
            }
        }

      if(neighbourhood[i] > max)
        {
          max = neighbourhood[i];
          index = i;
        }
    }
  return index;
}

void kp_init_states(klass_predictor *kp, Point *text)
{
  for(size_t i = 0; i < kp->nprocs; ++i)
    {
      kp->states[i] = malloc(sizeof(kp_state));
      kp->states[i]->train = kp->train_samples + i * kp->chunk_size;
      kp->states[i]->train_count = kp->chunk_size;
      if(i == kp->nprocs - 1)
        {
          kp->states[i]->train_count += kp->chunk_rem;
        }
      kp->states[i]->testing = text;
    }
  for(size_t i = 0; i < kp->nprocs; ++i)
    {
      if(pthread_create(&kp->threads[i], NULL, klassify_thread, kp->states[i])
         != 0)
        {
          exit(1);
        }
    }
  for(size_t i = 0; i < kp->nprocs; ++i)
    {
      pthread_join(kp->threads[i], NULL);
    }
}

size_t kp_predict(klass_predictor *kp, Point *text, size_t k)
{
  kp_init_states(kp, text);
  for(size_t i = 0; i < kp->nprocs; ++i)
    {
      for(size_t j = 0; j < kp->states[i]->train_count; ++j)
        {
          kp->neighbours[i * kp->chunk_size + j]
            = kp->states[i]->neighbours[j];
          kp->neighbours[i * kp->chunk_size + j].point
            = kp->states[i]->neighbours[j].point;
        }
    }
  qsort(kp->neighbours, kp->train_count, sizeof(DistancePoint), compare);
  int *neighbourhood = compute_frequencies(kp, k);
  int predicted_klass = mode(neighbourhood, kp);
  char *predicted_klass_name = kp->klasses[predicted_klass];
  return predicted_klass;
}
