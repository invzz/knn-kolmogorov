// kp_state.c
#include "model.kp_state.h"
#include "model.logging.h"
#include <stdio.h>
#include <stdlib.h>

kp_state *allocateKPState()
{
  DEBUG_PRINT("\r[ Allocated ]   ::  kp_state\n");
  kp_state *newState = (kp_state *)malloc(sizeof(kp_state));
  if(newState == NULL)
    {
      fprintf(stderr, "Memory allocation failed for kp_state\n");
      exit(EXIT_FAILURE);
    }
  newState->train_count  = 0;
  newState->train        = NULL;
  newState->testing      = NULL;
  newState->neighbours   = NULL;
  newState->taskQueue    = NULL;
  newState->predictQueue = NULL;
  return newState;
}

void deallocateKPState(kp_state *state)
{
  DEBUG_PRINT("Deallocating kp_state\n");
  if(state->neighbours != NULL) { deallocateDistancePoints(state->neighbours, state->train_count); }
  if(state->taskQueue != NULL) deallocateQueue(state->taskQueue);
  if(state->predictQueue != NULL) deallocateQueue(state->predictQueue);
  free(state);
}