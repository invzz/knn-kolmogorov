/**
 * @file data-handler.h
 * @brief Header file for data handling functions.
 */

#ifndef _DATA_HANDLER_H_
#define _DATA_HANDLER_H_

#ifndef BUF_SIZE
#define BUF_SIZE 65536
#define SMALL_BUF_SIZE 512
#endif

#include "models.h"

/**
 * @brief Moves the specified data set to the top of the list.
 *
 * @param data The data set.
 * @param index The index of the data set to move to the top.
 */
void dataset_top(dataset *data, size_t index);

/**
 * @brief Allocates memory for a new Point structure.
 *
 * @return A pointer to the allocated Point structure.
 */
Point *alloc_point();

/**
 * @brief Reads a data set from a file and returns it.
 *
 * @param filename The name of the file to read the data set from.
 * @return A pointer to the read data set.
 */
dataset *gedataset_from_file(char *filename);

/**
 * @brief Returns the name of the class with the specified class ID.
 *
 * @param klass The class ID.
 * @return The name of the class.
 */
char *get_klass_name(int klass);

/**
 * @brief Returns the number of classes.
 *
 * @return The number of classes.
 */
int get_klass_count();

char **get_klass_array();

#endif
