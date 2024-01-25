#include "models.h"
#define read_file(filepath, destination)                                                                               \
                                                                                                                       \
  {                                                                                                                    \
    FILE    *stream     = fopen(filepath, "r");                                                                        \
    int      test_count = count_lines(stream);                                                                         \
    dataset *data       = allocateDataset(test_count - 1);                                                             \
    from_file_stram(stream, data);                                                                                     \
    fclose(stream);                                                                                                    \
    printf(#destination " loaded on memory into" #destination "  [ %-10.10d records ]\n", (int)data->size);            \
    destination = data;                                                                                                \
  }
static char *klasses[] = {"world", "sports", "business", "tech/science"};

dataset *get_dataset(char *path);
