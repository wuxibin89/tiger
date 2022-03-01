/*
 * parse.c - Parse source file.
 */

#include <stdio.h>
#include <stdlib.h>

#include "parse.h"
#include "semant.h"

int main(int argc, char **argv) {
  if (argc != 2) {
    fprintf(stderr, "usage: a.out filename\n");
    exit(1);
  }

  A_exp exp = parse(argv[1]);
  F_fragList fragList = SEM_transProg(exp);

  return 0;
}