#ifndef UTIL_H_
#define UTIL_H_

#include <stdbool.h>
#include <assert.h>

typedef char *string;

#define TRUE 1
#define FALSE 0

void *checked_malloc(int);

/* create a string in heap from a null terminated string */
string String(char *);

/* create a string in heap from a char array with length */
string NString(char *, size_t len);

typedef struct U_boolList_ *U_boolList;
struct U_boolList_ {bool head; U_boolList tail;};
U_boolList U_BoolList(bool head, U_boolList tail);

#endif // UTIL_H_