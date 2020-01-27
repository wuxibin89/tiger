#include "slp.h"
#include "util.h"
#include "prog1.h"
#include <vector>

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
  if(exp->kind == A_exp_::A_opExp) {
      return std::max(maxargs(exp->u.op.left), maxargs(exp->u.op.right));
  } else if(exp->kind == A_exp_::A_eseqExp) {
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

void interp(A_stm stm) {}