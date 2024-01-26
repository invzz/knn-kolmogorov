#include "agnewsloader.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define SEPARATOR ","
#define NEWLINE   "\n"
#ifdef _WIN32
#include <io.h>
#define F_OK   0
#define access _access
#else
#include <unistd.h>
#endif
void from_file_stram(FILE *stream, dataset *data)
{
  data->size                  = 0;
  int   line_count            = 0;
  int   current               = 0;
  char  line[KNN_BUFFER_SIZE] = {0};
  char *token;
  char *saveptr1;
  while(fgets(line, KNN_BUFFER_SIZE, stream))
    {
      if(line_count++ == 0) continue;
      data->size++;
      current                      = data->size - 1;
      token                        = strtok(line, SEPARATOR);
      data->samples[current].klass = atoi(token) - 1;
      token                        = strtok(NULL, NEWLINE);
      data->samples[current].text  = strdup(token);
    }
  return;
}

int count_lines(FILE *stream)
{
  int  line_count            = 0;
  char line[KNN_BUFFER_SIZE] = {0};
  while(fgets(line, KNN_BUFFER_SIZE, stream)) ++line_count;
  rewind(stream);
  return line_count;
}

dataset *get_dataset(char *filepath)
{
  FILE    *stream = fopen(filepath, "r");
  dataset *data   = NULL;
  if(access(filepath, F_OK) == 0)
    {
      data = allocateDataset(count_lines(stream) - 1);
      from_file_stram(stream, data);
      fclose(stream);
      printf(CRESET "Found " RED "[ %-50.50s ]" CRESET " :: " CYN "[ %-10.10d records ]\n" CRESET, filepath,
             (int)data->size);
    }
  else { printf("File %s does not exist\n", filepath); }

  return data;
}
