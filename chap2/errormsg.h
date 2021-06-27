#ifndef ERRORMSG_H_
#define ERRORMSG_H_

#include <stdio.h>

#include "util.h"

extern bool EM_anyErrors;

void EM_newline(void);

extern int EM_tokPos;

void EM_error(int, string, ...);
void EM_impossible(string, ...);
void EM_reset(string filename);
void EM_reset_file(FILE *file);

#endif  // ERRORMSG_H_