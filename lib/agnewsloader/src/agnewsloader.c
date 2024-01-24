#include "agnewsloader.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define SEPARATOR ","

Point *get_point(char *line)
{
  Point      *point = allocatePoint();
  const char *tok   = strtok(line, SEPARATOR);
  point->klass      = atoi(tok) - 1;
  int i             = 0;
  point->text       = strdup(&line[strlen(tok) + 1]);
  return point;
}

void from_file_stram(FILE *stream, dataset *data)
{
  data->size      = 0;
  data->capacity  = 0;
  int  line_count = 0;
  char line[BUFSIZ];
  while(fgets(line, BUFSIZ, stream))
    {
      if(line_count++ == 0) continue;
      char  *tmp = strdup(line);
      Point *p   = get_point(tmp);
      dataset_add_point(data, p);
      free(tmp);
    }
}

int read_agn(char *trainpath, char *testpath, dataset *train, dataset *test)
{
  FILE *stream = fopen(testpath, "r");
  from_file_stram(stream, test);
  fclose(stream);
  printf("Test loaded on memory!  [ %-10.10d records ]\n", (int)test->size);
  stream = fopen(trainpath, "r");
  from_file_stram(stream, train);
  fclose(stream);
  printf("Train loaded on memory! [ %-10.10d records ]\n", (int)train->size);
  return 0;
}