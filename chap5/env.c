#include "env.h"

E_enventry E_VarEntry(Tr_access access, Ty_ty ty) {
  E_enventry p = checked_malloc(sizeof(*p));
  p->kind = E_varEntry;
  p->u.var.access = access;
  p->u.var.ty = ty;
  return p;
}

E_enventry E_FunEntry(Tr_level level, Temp_label label, Ty_tyList formals,
                      Ty_ty result) {
  E_enventry p = checked_malloc(sizeof(*p));
  p->kind = E_funEntry;
  p->u.fun.level = level;
  p->u.fun.label = label;
  p->u.fun.formals = formals;
  p->u.fun.result = result;
  return p;
}

S_table E_base_tenv(void) { return S_empty(); }

S_table E_base_venv(void) { return S_empty(); }