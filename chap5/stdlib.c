#include "stdlib.h"

#include <stdio.h>
#include <string.h>

#include "errormsg.h"
#include "semant.h"

const char* __builtin =
    "let"
    "    function print(s: string) = ()\n"
    "    function flush() = ()\n"
    "    function getchar() : string = \"\"\n"
    "    function ord(s: string) : int = 0\n"
    "    function chr(i: int) : string = \"\"\n"
    "    function size(s: string) : int = 0\n"
    "    function substring(s: string, first: int, n: int) : string = \"\"\n"
    "    function concat(s1: string, s2: string) : string = \"\"\n"
    "    function not(i: int) : int = 0\n"
    "    function exit(i: int) = ()\n"
    "in\n"
    "end";

extern FILE* yyin;
extern int yyparse(void);
extern A_exp absyn_root;

void builtin(S_table venv, S_table tenv) {
  FILE* file = fmemopen((void*)__builtin, strlen(__builtin), "r");
  EM_reset_file(file);

  assert(yyparse() == 0);
  assert(absyn_root->kind == A_letExp);
  A_decList l = absyn_root->u.let.decs;
  for (; l; l = l->tail) {
    transDec(venv, tenv, l->head);
  }
}