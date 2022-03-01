#ifndef __SEMANT_H__
#define __SEMANT_H__

#include "absyn.h"
#include "env.h"
#include "translate.h"
#include "types.h"

struct expty {
  Tr_exp exp;
  Ty_ty ty;
};

struct expty expTy(Tr_exp exp, Ty_ty ty);

struct expty transVar(Tr_level level, Temp_label done, S_table venv, S_table tenv, A_var v);
struct expty transExp(Tr_level level, Temp_label done, S_table venv, S_table tenv, A_exp e);
Tr_exp transDec(Tr_level level, Temp_label done, S_table venv, S_table tenv, A_dec d);
Ty_ty transTy(S_table tenv, A_ty a);

F_fragList SEM_transProg(A_exp exp);

#endif  // __SEMANT_H__