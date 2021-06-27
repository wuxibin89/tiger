#include <stdio.h>
#include <stdlib.h>

#include "absyn.h"
#include "parse.h"
#include "prabsyn.h"

int main(int argc, char **argv) {
  if (argc != 2) {
    fprintf(stderr, "usage: a.out filename\n");
    exit(1);
  }

  A_exp exp = parse(argv[1]);
  pr_exp(stdout, exp, 4);

  return 0;
}