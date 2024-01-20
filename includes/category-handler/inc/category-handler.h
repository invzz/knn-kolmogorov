#ifndef _CATEGORY_HANDLER_H_
#define _CATEGORY_HANDLER_H_

#define HASHSIZE 101

typedef struct nlist_s { /* table entry: */
  struct nlist_s *next;  /* next entry in chain */
  char *name;            /* defined name */
  int defn;              /* replacement text */
} nlist;
nlist *lookup(char *s);
nlist *install(char *name);
void printCat();
char *get_klass(int klass);
int category_arrayCount();
char **get_klasses(int klass);
#endif