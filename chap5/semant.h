#ifndef __SEMANT_H__
#define __SEMANT_H__

#include "absyn.h"
#include "env.h"
#include "translate.h"
#include "types.h"

typedef void *Tr_exp;
struct expty {
  Tr_exp exp;
  Ty_ty ty;
};

struct expty expTy(Tr_exp exp, Ty_ty ty);

struct expty transVar(Tr_level level, S_table venv, S_table tenv, A_var v);
struct expty transExp(Tr_level level, S_table venv, S_table tenv, A_exp e);
void transDec(Tr_level level, S_table venv, S_table tenv, A_dec d);
Ty_ty transTy(S_table tenv, A_ty a);

void SEM_transProg(A_exp exp);

#endif  // __SEMANT_H__