/*
 * util.c - commonly used utility functions.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "util.h"
void *checked_malloc(int len)
{void *p = malloc(len);
 if (!p) {
    fprintf(stderr,"\nRan out of memory!\n");
    exit(1);
 }
 return p;
}

string String(char *s)
{
 string p = (string)checked_malloc(strlen(s)+1);
 strcpy(p,s);
 return p;
}

string NString(char *s, size_t len) {
   string p = (string)checked_malloc(len + 1);
   memcpy(p, s, len);
   p[len] = '\0';
   return p;
}

U_boolList U_BoolList(bool head, U_boolList tail)
{ U_boolList list = (U_boolList)checked_malloc(sizeof(*list));
  list->head = head;
  list->tail = tail;
  return list;
}
