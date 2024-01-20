/**
 * @file data-handler.c
 * @brief Implementation of data getter functions.
 *
 * This file contains the implementation of functions for reading data from a
 * file and manipulating the data.
 */

#include "data-handler.h"
#include <assert.h>
#include <category-handler.h>
#include <fcntl.h>
#include <json-c/json.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
extern char *category_array[HASHSIZE];
/**
 * @brief Removes the first and last characters of a string.
 *
 * @param str The string to modify.
 */
void topntail(char *str) {
  if (str == NULL) {
    return;
  }
  size_t len = strlen(str);
  if (len < 2)
    return; // or whatever you want to do with short strings
  memmove(str, str + 1, len - 2);
  str[len - 2] = 0;
}

/**
 * @brief Fixes the text fields of a sample by removing surrounding quotes if
 * present.
 *
 * @param sample The sample to fix.
 */
void fixtext(t_sample *sample) {
  if (sample == NULL)
    return;
  if (*(sample->category) == '"')
    topntail(sample->category);
  if (*(sample->headline) == '"')
    topntail(sample->headline);
  if (*(sample->short_description) == '"')
    topntail(sample->short_description);
  if (*(sample->authors) == '"')
    topntail(sample->authors);
  if (*(sample->link) == '"')
    topntail(sample->link);
  if (*(sample->pubDate) == '"')
    topntail(sample->pubDate);
}

/**
 * @brief Reads a line from a file and parses it as a JSON object.
 *
 * @param fd The file descriptor.
 * @return The parsed JSON object.
 */
json_object *read_line(FILE *fd) {
  char *line = NULL;
  size_t len = 0;
  ssize_t read;
  json_object *jobj = NULL;

  if ((read = getline(&line, &len, fd)) != -1)
    jobj = json_tokener_parse(line);

  return jobj;
}

/**
 * @brief Reads data from a file and populates the data structure.
 *
 * @param fd The file descriptor.
 * @param data The data structure to populate.
 */
void read_file(FILE *fd, dataset *data) {
  while (1) {
    json_object *ret_json;

    Point *point = alloc_point();
    t_sample *sample = point->data;

    json_object *json_object = read_line(fd);
    json_object_object_get_ex(json_object, "category", &ret_json);
    sample->category = strdup(json_object_to_json_string_ext(
        ret_json, JSON_C_TO_STRING_NOSLASHESCAPE));
    json_object_object_get_ex(json_object, "headline", &ret_json);
    sample->headline = strdup(json_object_to_json_string_ext(
        ret_json, JSON_C_TO_STRING_NOSLASHESCAPE));
    json_object_object_get_ex(json_object, "short_description", &ret_json);
    sample->short_description = strdup(json_object_to_json_string(ret_json));
    json_object_object_get_ex(json_object, "authors", &ret_json);
    sample->authors = strdup(json_object_to_json_string_ext(
        ret_json, JSON_C_TO_STRING_NOSLASHESCAPE));
    json_object_object_get_ex(json_object, "link", &ret_json);
    sample->link = strdup(json_object_to_json_string_ext(
        ret_json, JSON_C_TO_STRING_NOSLASHESCAPE));
    json_object_object_get_ex(json_object, "date", &ret_json);
    sample->pubDate = strdup(json_object_to_json_string_ext(
        ret_json, JSON_C_TO_STRING_NOSLASHESCAPE));
    fixtext(sample);
    point->data = sample;

    data->size++;

    if (data->capacity <= sizeof(data) * data->size) {
      data->capacity = sizeof(data) * data->size * 2;
      data->points = realloc(data->points, data->capacity);
    }
    data->points[data->size - 1] = point;
    // log data
    printf("\rreading samples: %ld", data->size);
    fflush(stdout);
    if (json_object == NULL) {
      break;
    }
    install(sample->category);
    int klass = lookup(sample->category)->defn;
    point->klass = klass;
  }
  printf("\nData loaded on memory!\n\n");
}

/**
 * @brief Prints the category of a sample.
 *
 * @param data The data structure.
 * @param index The index of the sample.
 */
void dataset_top(dataset *data, size_t howmany) {
  printf("%-50.50s ", "Category");
  printf("%-10.10s\n", "klass");
  printf("--------------------------------------------------------------\n");
  for (int i = 0; i < howmany; i++) {
    printf("%-50.30s ", data->points[i]->data->category);
    printf("%-2.2d", data->points[i]->klass);
    printf("\n");
  }
  printf("--------------------------------------------------------------\n");
}

/**
 * @brief Reads data from a file and returns a data structure.
 *
 * @param filename The name of the file to read.
 * @return The data structure containing the read data.
 */
dataset *gedataset_from_file(char *filename) {
  dataset *data = malloc(sizeof(dataset));
  FILE *fd = fopen(filename, "r");
  read_file(fd, data);
  fclose(fd);
  return data;
}

Point *alloc_point() {
  Point *point = malloc(sizeof(Point));
  point->data = malloc(sizeof(t_sample));
  return point;
}

int get_klass_count() { return category_arrayCount(); }
char *get_klass_name(int klass) { return get_klass(klass); }
char **get_klass_array(int klass) {
  // allocate array
  char **array = malloc(sizeof(char *) * get_klass_count());
  //  deep copy get_klass_array to array
  for (int i = 0; i < get_klass_count(); i++) {
    array[i] = get_klass(i);
  }
  return array;
}