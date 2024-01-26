#ifndef MODEL_LOGGING_H
#define MODEL_LOGGING_H

#ifdef DEBUG
#define DEBUG_PRINT(fmt, ...)                                                                                          \
  do {                                                                                                                 \
      fprintf(stdout, fmt, ##__VA_ARGS__);                                                                             \
  } while(0)
#else
#define DEBUG_PRINT(fmt, ...)                                                                                          \
  do {                                                                                                                 \
  } while(0)
#endif
#endif