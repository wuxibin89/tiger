#include "env.h"


E_enventry E_VarEntry(Ty_ty ty) {
    E_enventry p = checked_malloc(sizeof(*p));
    p->kind = E_varEntry;
    p->u.var.ty = ty;
    return p;
}

E_enventry E_FunEntry(Ty_tyList formals, Ty_ty result) {
    E_enventry p = checked_malloc(sizeof(*p));
    p->kind = E_funEntry;
    p->u.fun.formals = formals;
    p->u.fun.result = result;
    return p;
}

S_table E_base_tenv(void) {
    return S_empty();
}

S_table E_base_venv(void) {
    return S_empty();
}