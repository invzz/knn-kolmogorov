#include "agnewsloader.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define SEPARATOR ","
#define NEWLINE   "\n"

void from_file_stram(FILE *stream, dataset *data)
{
  data->size         = 0;
  int   line_count   = 0;
  int   current      = 0;
  char  line[BUFSIZ] = {0};
  char *token;
  char *saveptr1;
  while(fgets(line, BUFSIZ, stream))
    {
      if(line_count++ == 0) continue;
      data->size++;
      current                      = data->size - 1;
      token                        = strtok_r(line, SEPARATOR, &saveptr1);
      data->samples[current].klass = atoi(token) - 1;
      token                        = strtok_r(NULL, SEPARATOR, &saveptr1);
      data->samples[current].text  = strdup(token);
    }
  return;
}

int count_lines(FILE *stream)
{
  int  line_count   = 0;
  char line[BUFSIZ] = {0};
  while(fgets(line, BUFSIZ, stream)) ++line_count;
  rewind(stream);
  return line_count;
}

dataset *get_dataset(char *filepath)
{
  FILE    *stream     = fopen(filepath, "r");
  int      test_count = count_lines(stream);
  dataset *data       = allocateDataset(test_count - 1);
  from_file_stram(stream, data);
  fclose(stream);
  printf("test"
         " loaded on memory into"
         "test"
         "  [ %-10.10d records ]\n",
         (int)data->size);
  return data;
}
