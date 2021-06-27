#include "semant.h"

#include <stddef.h>

#include "absyn.h"
#include "errormsg.h"
#include "parse.h"
#include "stdlib.h"
#include "symbol.h"

static struct S_symbol_ loopsym = {"<loop>", NULL};

struct expty expTy(Tr_exp exp, Ty_ty ty) {
  struct expty e;
  e.exp = exp;
  e.ty = ty;
  return e;
}

Ty_ty actual_ty(Ty_ty ty) {
  if (!ty) return ty;
  return (ty->kind == Ty_name) ? actual_ty(ty->u.name.ty) : ty;
}

bool assignable(Ty_ty t1, Ty_ty t2) {
  return (t1 == t2) || (t1->kind == Ty_record && t2->kind == Ty_nil);
}

bool comparable(Ty_ty t1, Ty_ty t2) {
  return (t1->kind == Ty_int && t2->kind == Ty_int) ||
         (t1->kind == Ty_string && t2->kind == Ty_string) ||
         (t1->kind == Ty_array && t2->kind == Ty_array) ||
         (t1->kind == Ty_record && t2->kind == Ty_nil) ||
         (t1->kind == Ty_nil && t1->kind == Ty_record);
}

// cycle reference check for name type
void cycle_check(S_table tenv, A_nametyList types) {
  bool *v = checked_malloc(sizeof(*v));
  S_table checked_types = S_empty();

  A_nametyList l;
  for (l = types; l; l = l->tail) {
    A_namety p = l->head;
    if (p->ty->kind == A_recordTy || p->ty->kind == A_arrayTy) {
      S_enter(checked_types, p->name, v);
      continue;
    }
    Ty_ty ty = S_look(tenv, p->name);
    assert(ty && ty->kind == Ty_name);
    if (S_look(checked_types, p->name)) continue;

    S_table visited = S_empty();
    while (true) {
      S_enter(visited, ty->u.name.sym, v);
      Ty_ty t = ty->u.name.ty;
      if (t->kind != Ty_name || S_look(checked_types, t->u.name.sym)) {
        break;
      }

      if (S_look(visited, t->u.name.sym)) {
        EM_error(p->ty->pos, "cycle reference type: %s", S_name(p->name));
      }

      ty = t;
    }

    while (visited->top) {
      binder b = TAB_pop(visited);
      S_enter(checked_types, b->key, b->value);
    }
  }
}

Ty_tyList makeFormalTyList(S_table tenv, A_fieldList params) {
  if (!params) return NULL;

  Ty_tyList head = NULL, tail = NULL;
  A_fieldList l;
  for (l = params; l; l = l->tail) {
    A_field p = l->head;
    Ty_ty ty = actual_ty(S_look(tenv, p->typ));
    if (!ty) {
      EM_error(p->pos, "undeclared type: %s", S_name(p->typ));
    }

    if (tail) {
      tail->tail = Ty_TyList(ty, NULL);
      tail = tail->tail;
      continue;
    }

    tail = Ty_TyList(ty, NULL);
    head = tail;
  }

  return head;
}

struct expty transVar(S_table venv, S_table tenv, A_var v) {
  switch (v->kind) {
    case A_simpleVar: {
      E_enventry x = S_look(venv, v->u.simple);
      if (x && x->kind == E_varEntry) {
        return expTy(NULL, actual_ty(x->u.var.ty));
      }
      EM_error(v->pos, "undefined variable %s", S_name(v->u.simple));
      return expTy(NULL, Ty_Int());
    }
    case A_fieldVar: {
      struct expty e = transVar(venv, tenv, v->u.field.var);
      if (e.ty->kind != Ty_record) {
        EM_error(v->pos, "mismatch type, expected: Ty_record, got: %s",
                 T_name(e.ty));
      }

      Ty_fieldList l;
      for (l = e.ty->u.record; l; l = l->tail) {
        Ty_field p = l->head;
        if (p->name == v->u.field.sym) {
          return expTy(NULL, actual_ty(p->ty));
        }
      }
      EM_error(v->pos, "undefined field: %s in record type",
               S_name(v->u.field.sym));
      return expTy(NULL, Ty_Int());
    }
    case A_subscriptVar: {
      struct expty e = transExp(venv, tenv, v->u.subscript.exp);
      if (e.ty->kind != Ty_int) {
        EM_error(v->pos, "mismatch type, expected: Ty_int, got: %s",
                 T_name(e.ty));
      }
      e = transVar(venv, tenv, v->u.subscript.var);
      if (e.ty->kind != Ty_array) {
        EM_error(v->pos, "mismatch type, expected: Ty_array, got: %s",
                 T_name(e.ty));
      }
      return expTy(NULL, actual_ty(e.ty->u.array));
    }
  }

  assert(0);
}

struct expty transCallExp(S_table venv, S_table tenv, A_exp e) {
  E_enventry f = S_look(venv, e->u.call.func);
  if (!f || f->kind != E_funEntry) {
    EM_error(e->pos, "undeclared function: %s", S_name(e->u.call.func));
  }
  Ty_tyList formals = f->u.fun.formals;
  A_expList args = e->u.call.args;
  for (; formals, args; formals = formals->tail, args = args->tail) {
    struct expty a = transExp(venv, tenv, args->head);
    if (a.ty != formals->head) {
      EM_error(e->pos,
               "mismatch type, expected: %s, got: %s, function call: %s",
               T_name(formals->head), T_name(a.ty), S_name(e->u.call.func));
    }
  }
  if (args || formals) {
    EM_error(e->pos, "mismatch args size in function call: %s",
             S_name(e->u.call.func));
  }
  return expTy(NULL, f->u.fun.result);
}

struct expty transOpExp(S_table venv, S_table tenv, A_exp e) {
  A_oper oper = e->u.op.oper;
  struct expty left = transExp(venv, tenv, e->u.op.left);
  struct expty right = transExp(venv, tenv, e->u.op.right);
  switch (oper) {
    case A_plusOp:
    case A_minusOp:
    case A_timesOp:
    case A_divideOp:
    case A_andOp:
    case A_orOp:
      if (!(left.ty->kind == Ty_int && right.ty->kind == Ty_int)) {
        EM_error(e->pos, "integer required, left: %s, right: %s",
                 T_name(left.ty), T_name(right.ty));
      }
      return expTy(NULL, Ty_Int());
    case A_eqOp:
    case A_neqOp:
      if (!comparable(left.ty, right.ty)) {
        EM_error(e->pos, "equality compare mismatch, left: %s, right: %s",
                 T_name(left.ty), T_name(right.ty));
      }
      return expTy(NULL, Ty_Int());
    case A_ltOp:
    case A_leOp:
    case A_gtOp:
    case A_geOp:
      if (!((left.ty->kind == Ty_int && right.ty->kind == Ty_int) ||
            (left.ty->kind == Ty_string && right.ty->kind == Ty_string))) {
        EM_error(e->pos, "integer or string required, left: %s, right: %s",
                 T_name(left.ty), T_name(right.ty));
      }
      return expTy(NULL, Ty_Int());
  }
  assert(0);
}

struct expty transRecordExp(S_table venv, S_table tenv, A_exp e) {
  Ty_ty ty = actual_ty(S_look(tenv, e->u.record.typ));
  if (!ty || ty->kind != Ty_record) {
    EM_error(e->pos, "record type: %s not defined", S_name(e->u.record.typ));
  }

  Ty_fieldList l = ty->u.record;
  A_efieldList f = e->u.record.fields;
  for (; l, f; l = l->tail, f = f->tail) {
    if (l->head->name != f->head->name) {
      EM_error(f->head->exp->pos,
               "mismatch field name, record: %s, expected: %s, got: %s",
               S_name(e->u.record.typ), S_name(l->head->name),
               S_name(f->head->name));
    }

    struct expty a = transExp(venv, tenv, f->head->exp);
    Ty_ty expected = actual_ty(l->head->ty);
    if (!assignable(expected, a.ty)) {
      EM_error(
          f->head->exp->pos,
          "mismatch field type, record: %s, field: %s, expected: %s, got: %s",
          S_name(e->u.record.typ), S_name(l->head->name), T_name(expected),
          T_name(a.ty));
    }
  }

  if (f || l) {
    EM_error(e->pos, "mismatch field size in record: %s",
             S_name(e->u.record.typ));
  }

  return expTy(NULL, ty);
}

struct expty transSeqExp(S_table venv, S_table tenv, A_exp e) {
  if (!e->u.seq) {
    return expTy(NULL, Ty_Void());
  }

  A_expList l = e->u.seq;
  // reverse order expression list: (e1, e2, e3) -> e3->e2->e1
  struct expty ety = transExp(venv, tenv, l->head);
  for (l = l->tail; l; l = l->tail) {
    transExp(venv, tenv, l->head);
  }

  return ety;
}

struct expty transAssignExp(S_table venv, S_table tenv, A_exp e) {
  struct expty vty = transVar(venv, tenv, e->u.assign.var);
  struct expty ety = transExp(venv, tenv, e->u.assign.exp);

  if (!assignable(vty.ty, ety.ty)) {
    EM_error(e->pos, "assign expression mismatch type, var: %s, exp: %s",
             T_name(vty.ty), T_name(ety.ty));
  }

  return expTy(NULL, Ty_Void());
}

struct expty transIfExp(S_table venv, S_table tenv, A_exp e) {
  struct expty test = transExp(venv, tenv, e->u.iff.test);
  if (test.ty->kind != Ty_int) {
    EM_error(e->pos, "if expression test must be integer, got: %s",
             T_name(test.ty));
  }

  struct expty then = transExp(venv, tenv, e->u.iff.then);
  if (!e->u.iff.elsee) {
    if (then.ty->kind != Ty_void) {
      EM_error(e->pos, "if-then expression must be void, got: %s",
               T_name(then.ty));
    }
    return then;
  }

  struct expty elsee = transExp(venv, tenv, e->u.iff.elsee);
  if (!assignable(then.ty, elsee.ty)) {
    EM_error(e->pos, "if expression mismatch type, then: %s, else: %s",
             T_name(then.ty), T_name(elsee.ty));
  }

  return then;
}

struct expty transWhileExp(S_table venv, S_table tenv, A_exp e) {
  struct expty test = transExp(venv, tenv, e->u.whilee.test);
  if (test.ty->kind != Ty_int) {
    EM_error(e->pos, "while expression test must be integer, got: %s",
             T_name(test.ty));
  }

  S_beginScope(venv);
  S_enter(venv, &loopsym, &loopsym);

  struct expty body = transExp(venv, tenv, e->u.whilee.body);
  if (body.ty->kind != Ty_void) {
    EM_error(e->pos, "while expression body must be void, got: %s",
             T_name(body.ty));
  }

  S_endScope(venv);
  return expTy(NULL, Ty_Void());
}

struct expty transForExp(S_table venv, S_table tenv, A_exp e) {
  struct expty lo = transExp(venv, tenv, e->u.forr.lo);
  struct expty hi = transExp(venv, tenv, e->u.forr.hi);
  if (lo.ty->kind != Ty_int || hi.ty->kind != Ty_int) {
    EM_error(e->pos, "for expression range must be integer, lo: %s, hi: %s",
             T_name(lo.ty), T_name(hi.ty));
  }

  S_beginScope(venv);
  // TODO: how to set var read only?
  S_enter(venv, e->u.forr.var, E_VarEntry(Ty_Int()));
  S_enter(venv, &loopsym, NULL);

  struct expty body = transExp(venv, tenv, e->u.forr.body);
  if (body.ty->kind != Ty_void) {
    EM_error(e->pos, "for expression body must be void, got: %s",
             T_name(body.ty));
  }

  S_endScope(venv);
  return expTy(NULL, Ty_Void());
}

struct expty transBreakExp(S_table venv, S_table tenv, A_exp e) {
  E_enventry loop = S_look(venv, &loopsym);
  if (!loop) {
    EM_error(e->pos, "break expression must be in for or while loop");
  }

  return expTy(NULL, Ty_Void());
}

struct expty transLetExp(S_table venv, S_table tenv, A_exp e) {
  S_beginScope(tenv);

  A_decList l = e->u.let.decs;
  for (; l; l = l->tail) {
    transDec(venv, tenv, l->head);
  }

  assert(e->u.let.body->kind == A_seqExp);
  struct expty res = transSeqExp(venv, tenv, e->u.let.body);

  S_endScope(tenv);
  return res;
}

struct expty transArrayExp(S_table venv, S_table tenv, A_exp e) {
  Ty_ty typ = actual_ty(S_look(tenv, e->u.array.typ));
  if (!typ) {
    EM_error(e->pos, "undeclared type: %s", S_name(e->u.array.typ));
  }

  if (typ->kind != Ty_array) {
    EM_error(e->pos, "array expression: %s must be array type, got: %s",
             S_name(e->u.array.typ), T_name(typ));
  }

  struct expty size = transExp(venv, tenv, e->u.array.size);
  if (size.ty->kind != Ty_int) {
    EM_error(e->pos, "array expression size must be integer, got: %s",
             T_name(size.ty));
  }

  struct expty init = transExp(venv, tenv, e->u.array.init);
  Ty_ty expected = actual_ty(typ->u.array);
  Ty_ty got = actual_ty(init.ty);
  if (expected != got) {
    EM_error(e->pos, "array expression mismatch type, expected: %s, got: %s",
             T_name(expected), T_name(got));
  }

  return expTy(NULL, typ);
}

struct expty transExp(S_table venv, S_table tenv, A_exp e) {
  switch (e->kind) {
    case A_varExp: {
      return transVar(venv, tenv, e->u.var);
    }
    case A_nilExp: {
      return expTy(NULL, Ty_Nil());
    }
    case A_intExp: {
      return expTy(NULL, Ty_Int());
    }
    case A_stringExp: {
      return expTy(NULL, Ty_String());
    }
    case A_callExp: {
      return transCallExp(venv, tenv, e);
    }
    case A_opExp: {
      return transOpExp(venv, tenv, e);
    }
    case A_recordExp: {
      return transRecordExp(venv, tenv, e);
    }
    case A_seqExp: {
      return transSeqExp(venv, tenv, e);
    }
    case A_assignExp: {
      return transAssignExp(venv, tenv, e);
    }
    case A_ifExp: {
      return transIfExp(venv, tenv, e);
    }
    case A_whileExp: {
      return transWhileExp(venv, tenv, e);
    }
    case A_forExp: {
      return transForExp(venv, tenv, e);
    }
    case A_breakExp: {
      return transBreakExp(venv, tenv, e);
    }
    case A_letExp: {
      return transLetExp(venv, tenv, e);
    }
    case A_arrayExp: {
      return transArrayExp(venv, tenv, e);
    }
  }
  assert(0);
}

void transDec(S_table venv, S_table tenv, A_dec d) {
  switch (d->kind) {
    case A_varDec: {
      struct expty e = transExp(venv, tenv, d->u.var.init);
      if (!d->u.var.typ && e.ty->kind == Ty_nil) {
        EM_error(d->pos, "nil expressions not constrained by record type: %s",
                 S_name(d->u.var.var));
      }

      Ty_ty ty = e.ty;
      if (d->u.var.typ) {
        ty = actual_ty(S_look(tenv, d->u.var.typ));
        if (!ty) {
          EM_error(d->pos, "undeclared type: %s", S_name(d->u.var.typ));
        }

        if (!assignable(ty, e.ty)) {
          EM_error(d->pos, "mismatch type, expected: %s, got: %s", T_name(ty),
                   T_name(e.ty));
        }
      }

      S_enter(venv, d->u.var.var, E_VarEntry(ty));
      return;
    }
    case A_typeDec: {
      A_nametyList l;
      S_table types = S_empty();
      for (l = d->u.type; l; l = l->tail) {
        A_namety p = l->head;
        if (S_look(types, p->name)) {
          EM_error(p->ty->pos, "redefine type: %s", S_name(p->name));
        }
        S_enter(types, p->name, &loopsym);

        S_enter(tenv, p->name, Ty_Name(p->name, NULL));
      }
      for (l = d->u.type; l; l = l->tail) {
        A_namety p = l->head;
        Ty_ty ty = S_look(tenv, p->name);
        assert(ty);
        ty->u.name.ty = transTy(tenv, p->ty);
      }

      // cycle reference check
      cycle_check(tenv, d->u.type);
      return;
    }
    case A_functionDec: {
      A_fundecList l;
      S_table types = S_empty();
      for (l = d->u.function; l; l = l->tail) {
        A_fundec p = l->head;
        if (S_look(types, p->name)) {
          EM_error(p->pos, "redefine function: %s", S_name(p->name));
        }
        S_enter(types, p->name, &loopsym);

        Ty_ty resultTy = Ty_Void();
        if (p->result) {
          resultTy = actual_ty(S_look(tenv, p->result));
          if (!resultTy) {
            EM_error(p->pos, "undeclared type: %s", S_name(p->result));
          }
        }
        Ty_tyList formalTys = makeFormalTyList(tenv, p->params);
        S_enter(venv, p->name, E_FunEntry(formalTys, resultTy));
      }
      for (l = d->u.function; l; l = l->tail) {
        A_fundec p = l->head;
        E_enventry f = S_look(venv, p->name);
        assert(f);
        S_beginScope(venv);
        A_fieldList l;
        Ty_tyList t;
        for (l = p->params, t = f->u.fun.formals; l; l = l->tail, t = t->tail) {
          S_enter(venv, l->head->name, E_VarEntry(t->head));
        }
        struct expty ty = transExp(venv, tenv, p->body);
        if (ty.ty != f->u.fun.result) {
          EM_error(d->pos, "mismatch return type, expected: %s, got: %s",
                   T_name(f->u.fun.result), T_name(ty.ty));
        }
        S_endScope(venv);
      }
      return;
    }
  }
  assert(0);
}

Ty_ty transTy(S_table tenv, A_ty a) {
  switch (a->kind) {
    case A_nameTy: {
      Ty_ty ty = S_look(tenv, a->u.name);
      if (!ty) {
        EM_error(a->pos, "undeclared type: %s", S_name(a->u.name));
      }
      return Ty_Name(a->u.name, ty);
    }
    case A_recordTy: {
      if (!a->u.record) return Ty_Record(NULL);  // empty fields
      Ty_fieldList head = NULL, tail = NULL;
      A_fieldList l;
      for (l = a->u.record; l; l = l->tail) {
        A_field p = l->head;
        Ty_ty ty = S_look(tenv, p->typ);
        if (!ty) {
          EM_error(p->pos, "undeclared type: %s", S_name(p->typ));
        }
        Ty_field field = Ty_Field(p->name, ty);

        if (tail) {
          tail->tail = Ty_FieldList(field, NULL);
          tail = tail->tail;
          continue;
        }

        tail = Ty_FieldList(field, NULL);
        head = tail;
      }
      return Ty_Record(head);
    }
    case A_arrayTy: {
      Ty_ty ty = S_look(tenv, a->u.array);
      if (!ty) {
        EM_error(a->pos, "undeclared type: %s", S_name(a->u.array));
      }
      return Ty_Array(ty);
    }
  }
  assert(0);
}

void SEM_transProg(A_exp exp) {
  S_table venv = E_base_venv();
  S_table tenv = E_base_venv();

  S_enter(tenv, S_Symbol("int"), Ty_Int());
  S_enter(tenv, S_Symbol("string"), Ty_String());
  builtin(venv, tenv);

  transExp(venv, tenv, exp);
}