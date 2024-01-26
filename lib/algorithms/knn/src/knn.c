
#ifdef _WIN32
#include <windows.h>
#endif
#include "knn.h"
#include <signal.h>
#include <assert.h>
#include <float.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <zlib.h>
#ifndef _WIN32
#include <sys/sysinfo.h> // get_nprocs()
#endif
typedef struct s_stateMessage
{
  kp_state *state;
  Point    *point;
} msg_distance;

typedef struct s_resultMessage
{
  DistancePoint *neighbours;
} msg_result;

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
  uLong  size        = strlen(text);
  uLongf output_size = 2 * size;

  char *output = malloc(output_size);
  assert(output != NULL);
  memset(output, 0, output_size);

  compress((Bytef *)output, &output_size, (Bytef *)text, size);

  free(output);

  return output_size;
}

float ncd(Point *a, Point *b)
{
  char *buffer[KNN_BUFFER_SIZE] = {0};
  int   cX1                     = kolmogorov_distance(a->text);
  int   cX2                     = kolmogorov_distance(b->text);
#ifdef _WIN32
  strcat_s((char *)buffer, KNN_BUFFER_SIZE, a->text);
  strcat_s((char *)buffer, KNN_BUFFER_SIZE, b->text);
#else
  strcat((char *)buffer, a->text);
  strcat((char *)buffer, b->text);
#endif
  int   cX1X2        = kolmogorov_distance((char *)buffer);
  float mini         = (cX1 < cX2 ? cX1 : cX2);
  float maxi         = (cX1 > cX2 ? cX1 : cX2);
  float return_value = (cX1X2 - mini) / maxi;

  return return_value;
}

void *klassify_thread(void *args)
{
  Queue          *dataQueue = (Queue *)args;
  msg_result     *rm        = malloc(sizeof(msg_result));
  thread_message *tm2       = malloc(sizeof(thread_message));
  int             count;
  memset(rm, 0, sizeof(msg_result));
  memset(tm2, 0, sizeof(thread_message));

  while(1)
    {
      thread_message *tm = dequeue(dataQueue);
      if(tm == NULL) { continue; }
      if(tm->message == THREAD_MESSAGE_COMPUTE_NCDS)
        {
          msg_distance *sm    = (msg_distance *)(tm->data);
          kp_state     *state = sm->state;
          Point        *point = sm->point;
          if(rm->neighbours == NULL) { rm->neighbours = allocateDistancePoints(state->train_count); }
          for(int i = 0; i < state->train_count; i++)
            {
              rm->neighbours[i].point    = &state->train[i];
              rm->neighbours[i].distance = ncd(&state->train[i], point);
            }
          tm2->message = THREAD_MESSAGE_NCDS_DONE;
          tm2->data    = rm;
          enqueue(state->predictQueue, tm2);
          // deallocateThreadMessage(tm);
        }

      if(tm->message == THREAD_MESSAGE_KILL)
        {
          free(tm);
          pthread_exit(NULL);
        }
    }
}

klass_predictor *kp_init(dataset *train, dataset *test, int k)
{
  klass_predictor *kp = allocateKlassPredictor();

#ifdef _WIN32
  SYSTEM_INFO sysinfo;
  GetSystemInfo(&sysinfo);
  int number_of_cores = sysinfo.dwNumberOfProcessors;
#else
  int number_of_cores = get_nprocs();
#endif
  // get amount of cores on the machine

  kp->number_of_cores = number_of_cores;
  kp->chunk_size      = train->size / number_of_cores;
  kp->chunk_rem       = train->size % number_of_cores;
  kp->train_samples   = train->samples;
  kp->train_count     = train->size;
  kp->threads         = calloc(number_of_cores, sizeof(pthread_t));
  kp->states          = calloc(number_of_cores, sizeof(kp_state));
  kp->neighbours      = calloc(train->size, sizeof(DistancePoint));

  for(int i = 0; i < number_of_cores; ++i)
    {
      kp->states[i].taskQueue    = createQueue(QUEUE_SIZE);
      kp->states[i].predictQueue = createQueue(QUEUE_SIZE);
      if(pthread_create(&kp->threads[i], NULL, klassify_thread, kp->states[i].taskQueue) != 0) { exit(1); }
    }
  return kp;
}

int *compute_frequencies(klass_predictor *kp, size_t k)
{
  int  klass_count   = kp->klasses_count;
  int *neighbourhood = calloc(sizeof(int), klass_count);
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
  msg_distance *sm;

  for(size_t i = 0; i < kp->number_of_cores; ++i)
    {
      thread_message *tm = allocateThreadMessage();

      kp->states[i].train       = &kp->train_samples[i * kp->chunk_size];
      kp->states[i].train_count = kp->chunk_size;
      if(i == kp->number_of_cores - 1) { kp->states[i].train_count += kp->chunk_rem; }
      sm                    = malloc(sizeof(msg_distance));
      kp->states[i].testing = text;

      sm->state = &kp->states[i];
      sm->point = text;

      tm->message = THREAD_MESSAGE_COMPUTE_NCDS;
      tm->data    = sm;
      enqueue(kp->states[i].taskQueue, tm);
    }
  int count = 0;

  msg_result *rm;
  while(1)
    {
      if(count == kp->number_of_cores) { break; }
      for(size_t i = 0; i < kp->number_of_cores; ++i)
        {
          thread_message *msg = dequeue(kp->states[i].predictQueue);
          if(msg == NULL) continue;
          if(count != kp->number_of_cores) { DEBUG_PRINT("COUNT  >>  %zu/%zu\n", kp->number_of_cores, count); }
          if(count == kp->number_of_cores)
            {
              thread_message *tm = allocateThreadMessage();
              tm->message        = THREAD_MESSAGE_KILL;
              enqueue(kp->states[i].taskQueue, tm);
              break;
            }
          switch(msg->message)
            {
            case THREAD_MESSAGE_NCDS_DONE:
              rm = (msg_result *)msg->data;
              for(int j = 0; j < kp->states[i].train_count; ++j)
                {
                  kp->neighbours[i * kp->chunk_size + j] = rm->neighbours[j];
                }

              count++;
              // deallocateThreadMessage(msg);
              break;

            case THREAD_MESSAGE_KILL: printf("Killing thread %zu\n", i); break;
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
