#include "prog1.h"

#include <stdio.h>
#include <string.h>

#include <vector>

#include "slp.h"
#include "util.h"

A_stm prog(void) {
  return A_CompoundStm(
      A_AssignStm("a", A_OpExp(A_NumExp(5), A_plus, A_NumExp(3))),
      A_CompoundStm(
          A_AssignStm("b",
                      A_EseqExp(A_PrintStm(A_PairExpList(
                                    A_IdExp("a"),
                                    A_LastExpList(A_OpExp(A_IdExp("a"), A_minus,
                                                          A_NumExp(1))))),
                                A_OpExp(A_NumExp(10), A_times, A_IdExp("a")))),
          A_PrintStm(A_LastExpList(A_IdExp("b")))));
}

// DFS or BFS?
int maxargs(A_exp exp) {
  if (exp->kind == A_exp_::A_opExp) {
    return std::max(maxargs(exp->u.op.left), maxargs(exp->u.op.right));
  } else if (exp->kind == A_exp_::A_eseqExp) {
    return std::max(maxargs(exp->u.eseq.stm), maxargs(exp->u.eseq.exp));
  }

  return 0;
}

int maxargs(A_stm stm) {
  int max_args = 0;
  if (stm->kind == A_stm_::A_compoundStm) {
    max_args = std::max(max_args, maxargs(stm->u.compound.stm1));
    max_args = std::max(max_args, maxargs(stm->u.compound.stm2));
  } else if (stm->kind == A_stm_::A_assignStm) {
    max_args = std::max(max_args, maxargs(stm->u.assign.exp));
  } else if (stm->kind == A_stm_::A_printStm) {
    int args = 1;
    A_expList exps = stm->u.print.exps;
    while (exps->kind != A_expList_::A_lastExpList) {
      max_args = std::max(max_args, maxargs(exps->u.pair.head));
      ++args;
      exps = exps->u.pair.tail;
    }
    // last expression
    max_args = std::max(max_args, maxargs(exps->u.last));
    max_args = std::max(max_args, args);
  }

  return max_args;
}

Table_ Table(string id, int value, struct table *tail) {
  Table_ t = (Table_)checked_malloc(sizeof(*t));
  t->id = id;
  t->value = value;
  t->tail = tail;
  return t;
}

Table_ interpStm(A_stm stm, Table_ *t) {
  if (stm->kind == A_stm_::A_compoundStm) {
    interpStm(stm->u.compound.stm1, t);
    interpStm(stm->u.compound.stm2, t);
  } else if (stm->kind == A_stm_::A_assignStm) {
    IntAndTable it = interpExp(stm->u.assign.exp, t);
    *t = Table(stm->u.assign.id, it.i, it.t);
  } else if (stm->kind == A_stm_::A_printStm) {
    A_expList exps = stm->u.print.exps;
    while (exps->kind != A_expList_::A_lastExpList) {
      IntAndTable it = interpExp(exps->u.pair.head, t);
      printf("%d\n", it.i);
      exps = exps->u.pair.tail;
    }
    // last expression
    IntAndTable it = interpExp(exps->u.last, t);
    printf("%d\n", it.i);
  }

  return *t;
}

IntAndTable interpExp(A_exp exp, Table_ *t) {
  if (exp->kind == A_exp_::A_numExp) {
    return IntAndTable(exp->u.num, *t);
  } else if (exp->kind == A_exp_::A_idExp) {
    Table_ n = *t;
    while (n) {
      if (strcmp(exp->u.id, n->id) == 0) {
        return IntAndTable(n->value, *t);
      }
    }
    assert(n);
  } else if (exp->kind == A_exp_::A_opExp) {
    IntAndTable left = interpExp(exp->u.op.left, t);
    assert(left.t == *t);
    IntAndTable right = interpExp(exp->u.op.right, t);
    assert(right.t == *t);

    if (exp->u.op.oper == A_plus) {
      return IntAndTable(left.i + right.i, *t);
    } else if (exp->u.op.oper == A_minus) {
      return IntAndTable(left.i - right.i, *t);
    } else if (exp->u.op.oper == A_times) {
      return IntAndTable(left.i * right.i, *t);
    } else if (exp->u.op.oper == A_div) {
      return IntAndTable(left.i / right.i, *t);
    } else {
      assert(0);
    }
  } else if (exp->kind == A_exp_::A_eseqExp) {
    interpStm(exp->u.eseq.stm, t);
    return interpExp(exp->u.eseq.exp, t);
  }

  assert(0);
}

void interp(A_stm stm) {
  Table_ t = NULL;
  interpStm(stm, &t);
  printf("======debug info======\n");
  while (t) {
    printf("%s: %d\n", t->id, t->value);
    t = t->tail;
  }
}