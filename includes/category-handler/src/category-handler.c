#include "category-handler.h"
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

static nlist *hashtab[HASHSIZE]; /* pointer table */
static int category_count = 0;
static char *category_array[HASHSIZE];

/* hash: form hash value for string s */
unsigned hash(char *s) {
  unsigned hashval;
  for (hashval = 0; *s != '\0'; s++)
    hashval = *s + 31 * hashval;
  return hashval % HASHSIZE;
}

/* lookup: look for s in hashtab */
nlist *lookup(char *s) {
  nlist *np;
  for (np = hashtab[hash(s)]; np != NULL; np = np->next)
    if (strcmp(s, np->name) == 0)
      return np; /* found */
  return NULL;   /* not found */
}

/* install: put (name, defn) in hashtab */
nlist *install(char *name) {
  nlist *np;
  unsigned hashval;
  if ((np = lookup(name)) == NULL) { /* not found */
    category_count++;
    category_array[category_count] = strdup(name);
    np = (nlist *)malloc(sizeof(*np));
    memset(np, 0, sizeof(*np));
    if (np == NULL || (np->name = strdup(name)) == NULL)
      return NULL;
    hashval = hash(name);
    np->next = hashtab[hashval];
    np->defn = category_count;
    hashtab[hashval] = np;
  }

  return np;
}

void printCat() {
  int i = 1;
  int j = 0;

  nlist *np;
  for (i = 0; i < HASHSIZE; i++) {
    for (np = hashtab[i]; np != NULL; np = np->next) {
      printf("[CATEGORY %-2.2d ]  %-30.30s %-6.6d\n", ++j, np->name, np->defn);
    }
  }
}

// get the number of keys in the dictionary
int category_arrayCount() { return category_count; }

// gets the name corresponting to the key klass
char *get_klass(int klass) { return category_array[klass]; }

char **get_klasses(int klass) { category_array; }