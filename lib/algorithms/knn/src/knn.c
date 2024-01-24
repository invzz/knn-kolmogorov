
#include "knn.h"
#include <signal.h>
#include <assert.h>
#include <float.h>
#include <pthread.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/sysinfo.h>
#include <zlib.h>

int min(int a, int b) { return a < b ? a : b; }

int max(int a, int b) { return a > b ? a : b; }

int compare(const void *a, const void *b)
{
  DistancePoint *da = (DistancePoint *)a;
  DistancePoint *db = (DistancePoint *)b;
  if(da->distance < db->distance) return -1;
  if(da->distance > db->distance) return 1;
  return 0;
}

int kolmogorov_distance(char *text)
{
  Bytef *b           = (Bytef *)text;
  size_t size        = strlen(text);
  size_t output_size = 2 * size;

  char *output = malloc(output_size);
  assert(output != NULL);
  memset(output, 0, output_size);

  compress(output, &output_size, (Bytef *)text, size);
  return output_size;
}

float ncd(Point *a, Point *b)
{
  char *buffer = malloc(sizeof(char) * BUFSIZ);
  int   cX1    = kolmogorov_distance(a->text);
  int   cX2    = kolmogorov_distance(b->text);
  strcat(buffer, a->text);
  strcat(buffer, b->text);
  int   cX1X2        = kolmogorov_distance(buffer);
  float mini         = min(cX1, cX2);
  float maxi         = max(cX1, cX2);
  float return_value = (cX1X2 - mini) / maxi;
  free(buffer);
  return return_value;
}

typedef struct
{
  kp_state *state;
  Point    *point;
} stateMessage;

typedef struct
{
  DistancePoint *neighbours;
} resultMessage;

void *klassify_thread(void *args)
{
  Queue         *dataQueue = (Queue *)args;
  resultMessage *rm        = malloc(sizeof(resultMessage));
  int            count;
  while(1)
    {
      thread_message *tm = dequeue(dataQueue);

      if(tm == NULL) { continue; }
      switch(tm->message)
        {
        case THREAD_MESSAGE_COMPUTE_NCDS:
          stateMessage *sm = (stateMessage *)tm->data;

          kp_state      *state      = sm->state;
          Point         *point      = sm->point;
          DistancePoint *neighbours = malloc(sizeof(DistancePoint) * state->train_count);
          memset(neighbours, 0, sizeof(neighbours));
          assert(neighbours != NULL);

          for(int i = 0; i < state->train_count; i++)
            {
              neighbours[i].point    = state->train[i];
              neighbours[i].distance = ncd(state->train[i], point);
            }

          qsort(neighbours, state->train_count, sizeof(DistancePoint), compare);

          rm->neighbours = neighbours;

          thread_message *tm2 = malloc(sizeof(thread_message));
          tm2->message        = THREAD_MESSAGE_NCDS_DONE;
          tm2->data           = rm;

          enqueue(state->predictQueue, tm2);
          break;

        case THREAD_MESSAGE_KILL: pthread_exit(NULL); break;

        default: break;
        }
    }
}

void kp_init(klass_predictor *kp, t_knn *knn, char **categories, int category_count, int nprocs)
{
  int trsize = knn->training->size;
  // get amount of cores on the machine
  if(nprocs == 0) nprocs = get_nprocs();
  int        chunk_size = knn->training->size / nprocs;
  int        chunk_rem  = knn->training->size % nprocs;
  kp_state **states     = malloc(nprocs * sizeof(kp_state));

  kp->klasses       = categories;
  kp->klasses_count = category_count;
  kp->nprocs        = nprocs;
  kp->chunk_size    = chunk_size;
  kp->chunk_rem     = chunk_rem;
  kp->train_samples = knn->training->samples;
  kp->train_count   = knn->training->size;
  kp->threads       = malloc(nprocs * sizeof(pthread_t));
  kp->states        = malloc(nprocs * sizeof(kp_state *));
  kp->neighbours    = malloc(trsize * sizeof(DistancePoint));

  memset(states, 0, sizeof(kp_state) * nprocs);

  // Create a queue for each thread
  for(int i = 0; i < nprocs; ++i)
    {
      states[i]                   = malloc(sizeof(kp_state));
      states[i]->taskQueue        = createQueue(QUEUE_SIZE);
      states[i]->predictQueue     = createQueue(QUEUE_SIZE);
      kp->states[i]               = states[i];
      kp->states[i]->taskQueue    = states[i]->taskQueue;
      kp->states[i]->predictQueue = states[i]->predictQueue;
    }
  for(size_t i = 0; i < kp->nprocs; ++i)
    {
      if(pthread_create(&kp->threads[i], NULL, klassify_thread, kp->states[i]->taskQueue) != 0) { exit(1); }
    }
}

int *compute_frequencies(klass_predictor *kp, size_t k)
{
  int  klass_count   = kp->klasses_count;
  int *neighbourhood = malloc(sizeof(int) * klass_count);
  assert(neighbourhood != NULL);
  memset(neighbourhood, 0, sizeof(int) * klass_count);
  for(int i = 0; i < k && i < kp->train_count; ++i) { neighbourhood[kp->neighbours[i].point->klass] += 1; }
  return neighbourhood;
}

int mode(int *neighbourhood, klass_predictor *kp)
{
  int max   = 0;
  int index = 0;
  for(int i = 0; i < kp->klasses_count; ++i)
    {
      if(neighbourhood[i] == max)
        {
          if(kp->neighbours[i].distance < kp->neighbours[max].distance)
            {
              max   = neighbourhood[i];
              index = i;
            }
        }

      if(neighbourhood[i] > max)
        {
          max   = neighbourhood[i];
          index = i;
        }
    }
  return index;
}

size_t kp_predict(klass_predictor *kp, Point *text, size_t k)
{
  for(size_t i = 0; i < kp->nprocs; ++i)
    {
      kp->states[i]->train       = kp->train_samples + i * kp->chunk_size;
      kp->states[i]->train_count = kp->chunk_size;
      if(i == kp->nprocs - 1) { kp->states[i]->train_count += kp->chunk_rem; }

      kp->states[i]->testing = text;

      thread_message *tm = malloc(sizeof(thread_message));
      stateMessage   *sm = malloc(sizeof(stateMessage));

      sm->state = kp->states[i];
      sm->point = text;

      tm->message = THREAD_MESSAGE_COMPUTE_NCDS;
      tm->data    = sm;

      enqueue(kp->states[i]->taskQueue, tm);
    }
  int count = 0;

  while(1)
    {
      if(count == kp->nprocs) { break; }
      for(size_t i = 0; i < kp->nprocs; ++i)
        {
          thread_message *tm = dequeue(kp->states[i]->predictQueue);
          if(tm == NULL) continue;
          if(count == kp->nprocs) { break; }
          switch(tm->message)
            {
            case THREAD_MESSAGE_NCDS_DONE:
              resultMessage *rm = (resultMessage *)tm->data;
              for(int j = 0; j < kp->states[i]->train_count; ++j) { kp->neighbours[i * kp->chunk_size + j] = rm->neighbours[j]; }
              count++;
              break;

            case THREAD_MESSAGE_KILL:
              pthread_kill(kp->threads[i], SIGKILL);
              printf("Killing thread %zu\n", i);
              break;
            }
        }
    }
  // Sort the neighbours based on distance
  qsort(kp->neighbours, kp->train_count, sizeof(DistancePoint), compare);

  // Compute frequencies using the dynamically allocated Point data
  int *neighbourhood = compute_frequencies(kp, k);

  // Find the predicted class index
  int predicted_klass = mode(neighbourhood, kp);

  // Free the dynamically allocated memory for the neighbourhood array
  free(neighbourhood);

  // Return the predicted class index
  return predicted_klass;
}
