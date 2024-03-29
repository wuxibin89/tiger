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

// 函数形式参数是否逃逸列表
U_boolList makeFormalEscape(A_fieldList params) {
  U_boolList head = NULL, tail = NULL;
  for (; params; params = params->tail) {
    U_boolList t = U_BoolList(params->head->escape, NULL);
    if (!head) {
      head = t;
      tail = t;
    } else {
      tail->tail = t;
      tail = t;
    }
  }
  return head;
}

struct expty transVar(Tr_level level, Temp_label done, S_table venv,
                      S_table tenv, A_var v) {
  switch (v->kind) {
    case A_simpleVar: {
      E_enventry x = S_look(venv, v->u.simple);
      if (x && x->kind == E_varEntry) {
        return expTy(Tr_SimpleVar(x->u.var.access, level),
                     actual_ty(x->u.var.ty));
      }
      EM_error(v->pos, "undefined variable %s", S_name(v->u.simple));
      return expTy(NULL, Ty_Int());
    }
    case A_fieldVar: {
      struct expty e = transVar(level, done, venv, tenv, v->u.field.var);
      if (e.ty->kind != Ty_record) {
        EM_error(v->pos, "mismatch type, expected: Ty_record, got: %s",
                 T_name(e.ty));
      }

      Ty_fieldList l;
      unsigned offset;
      for (l = e.ty->u.record, offset = 0; l; l = l->tail, ++offset) {
        Ty_field p = l->head;
        if (p->name == v->u.field.sym) {
          return expTy(Tr_FieldVar(e.exp, offset), actual_ty(p->ty));
        }
      }
      EM_error(v->pos, "undefined field: %s in record type",
               S_name(v->u.field.sym));
      return expTy(NULL, Ty_Int());
    }
    case A_subscriptVar: {
      struct expty index =
          transExp(level, done, venv, tenv, v->u.subscript.exp);
      if (index.ty->kind != Ty_int) {
        EM_error(v->pos, "mismatch type, expected: Ty_int, got: %s",
                 T_name(index.ty));
      }
      struct expty array =
          transVar(level, done, venv, tenv, v->u.subscript.var);
      if (array.ty->kind != Ty_array) {
        EM_error(v->pos, "mismatch type, expected: Ty_array, got: %s",
                 T_name(array.ty));
      }
      return expTy(Tr_SubscriptVar(array.exp, index.exp),
                   actual_ty(array.ty->u.array));
    }
  }

  assert(0);
}

struct expty transCallExp(Tr_level level, Temp_label done, S_table venv,
                          S_table tenv, A_exp e) {
  E_enventry f = S_look(venv, e->u.call.func);
  if (!f || f->kind != E_funEntry) {
    EM_error(e->pos, "undeclared function: %s", S_name(e->u.call.func));
  }

  Ty_tyList formals = f->u.fun.formals;
  A_expList args = e->u.call.args;
  Tr_expList exp_head = NULL, exp_tail = NULL;

  for (; formals, args; formals = formals->tail, args = args->tail) {
    struct expty a = transExp(level, done, venv, tenv, args->head);
    if (a.ty != formals->head) {
      EM_error(e->pos,
               "mismatch type, expected: %s, got: %s, function call: %s",
               T_name(formals->head), T_name(a.ty), S_name(e->u.call.func));
    }

    if (!exp_tail) {
      exp_head = Tr_ExpList(a.exp, NULL);
      exp_tail = exp_head;
    } else {
      exp_tail->tail = Tr_ExpList(a.exp, NULL);
      exp_tail = exp_tail->tail;
    }
  }

  if (args || formals) {
    EM_error(e->pos, "mismatch args size in function call: %s",
             S_name(e->u.call.func));
  }

  return expTy(Tr_CallExp(level, f->u.fun.level, f->u.fun.label, exp_head),
               f->u.fun.result);
}

struct expty transOpExp(Tr_level level, Temp_label done, S_table venv,
                        S_table tenv, A_exp e) {
  A_oper oper = e->u.op.oper;
  struct expty left = transExp(level, done, venv, tenv, e->u.op.left);
  struct expty right = transExp(level, done, venv, tenv, e->u.op.right);
  switch (oper) {
    case A_plusOp:
    case A_minusOp:
    case A_timesOp:
    case A_divideOp:
      if (!(left.ty->kind == Ty_int && right.ty->kind == Ty_int)) {
        EM_error(e->pos, "integer required, left: %s, right: %s",
                 T_name(left.ty), T_name(right.ty));
      }
      return expTy(Tr_OpExp(oper, left.exp, right.exp), Ty_Int());
    case A_eqOp:
    case A_neqOp:
      if (!comparable(left.ty, right.ty)) {
        EM_error(e->pos, "equality compare mismatch, left: %s, right: %s",
                 T_name(left.ty), T_name(right.ty));
      }
      return expTy(Tr_OpExp(oper, left.exp, right.exp), Ty_Int());
    case A_ltOp:
    case A_leOp:
    case A_gtOp:
    case A_geOp:
      if (!((left.ty->kind == Ty_int && right.ty->kind == Ty_int) ||
            (left.ty->kind == Ty_string && right.ty->kind == Ty_string))) {
        EM_error(e->pos, "integer or string required, left: %s, right: %s",
                 T_name(left.ty), T_name(right.ty));
      }
      return expTy(Tr_OpExp(oper, left.exp, right.exp), Ty_Int());
  }
  assert(0);
}

struct expty transRecordExp(Tr_level level, Temp_label done, S_table venv,
                            S_table tenv, A_exp e) {
  Ty_ty ty = actual_ty(S_look(tenv, e->u.record.typ));
  if (!ty || ty->kind != Ty_record) {
    EM_error(e->pos, "record type: %s not defined", S_name(e->u.record.typ));
  }

  Ty_fieldList l = ty->u.record;
  A_efieldList f = e->u.record.fields;

  unsigned n_fields = 0;
  Tr_expList fields = NULL;
  for (; l, f; l = l->tail, f = f->tail, ++n_fields) {
    if (l->head->name != f->head->name) {
      EM_error(f->head->exp->pos,
               "mismatch field name, record: %s, expected: %s, got: %s",
               S_name(e->u.record.typ), S_name(l->head->name),
               S_name(f->head->name));
    }

    struct expty a = transExp(level, done, venv, tenv, f->head->exp);
    Ty_ty expected = actual_ty(l->head->ty);
    if (!assignable(expected, a.ty)) {
      EM_error(
          f->head->exp->pos,
          "mismatch field type, record: %s, field: %s, expected: %s, got: %s",
          S_name(e->u.record.typ), S_name(l->head->name), T_name(expected),
          T_name(a.ty));
    }

    fields = Tr_ExpList(a.exp, fields);
  }

  if (f || l) {
    EM_error(e->pos, "mismatch field size in record: %s",
             S_name(e->u.record.typ));
  }

  return expTy(Tr_RecordExp(fields, n_fields), ty);
}

struct expty transSeqExp(Tr_level level, Temp_label done, S_table venv,
                         S_table tenv, A_exp e) {
  if (!e->u.seq) {
    return expTy(Tr_IntExp(0), Ty_Void());
  }

  A_expList l = e->u.seq;
  // reverse order expression list: (e1, e2, e3) -> e3->e2->e1
  struct expty ety = transExp(level, done, venv, tenv, l->head);
  for (l = l->tail; l; l = l->tail) {
    transExp(level, done, venv, tenv, l->head);
  }

  return ety;
}

struct expty transAssignExp(Tr_level level, Temp_label done, S_table venv,
                            S_table tenv, A_exp e) {
  struct expty vty = transVar(level, done, venv, tenv, e->u.assign.var);
  struct expty ety = transExp(level, done, venv, tenv, e->u.assign.exp);

  if (!assignable(vty.ty, ety.ty)) {
    EM_error(e->pos, "assign expression mismatch type, var: %s, exp: %s",
             T_name(vty.ty), T_name(ety.ty));
  }

  return expTy(Tr_AssignExp(vty.exp, ety.exp), Ty_Void());
}

struct expty transIfExp(Tr_level level, Temp_label done, S_table venv,
                        S_table tenv, A_exp e) {
  struct expty test = transExp(level, done, venv, tenv, e->u.iff.test);
  if (test.ty->kind != Ty_int) {
    EM_error(e->pos, "if expression test must be integer, got: %s",
             T_name(test.ty));
  }

  struct expty then = transExp(level, done, venv, tenv, e->u.iff.then);
  if (!e->u.iff.elsee) {
    if (then.ty->kind != Ty_void) {
      EM_error(e->pos, "if-then expression must be void, got: %s",
               T_name(then.ty));
    }
    return expTy(Tr_IfExp(test.exp, then.exp, NULL), then.ty);
  }

  struct expty elsee = transExp(level, done, venv, tenv, e->u.iff.elsee);
  if (!assignable(then.ty, elsee.ty)) {
    EM_error(e->pos, "if expression mismatch type, then: %s, else: %s",
             T_name(then.ty), T_name(elsee.ty));
  }

  return expTy(Tr_IfExp(test.exp, then.exp, elsee.exp), then.ty);
}

struct expty transWhileExp(Tr_level level, Temp_label done, S_table venv,
                           S_table tenv, A_exp e) {
  struct expty test = transExp(level, done, venv, tenv, e->u.whilee.test);
  if (test.ty->kind != Ty_int) {
    EM_error(e->pos, "while expression test must be integer, got: %s",
             T_name(test.ty));
  }

  S_beginScope(venv);
  S_enter(venv, &loopsym, &loopsym);

  // 循环结束标号
  done = Temp_newlabel();

  struct expty body = transExp(level, done, venv, tenv, e->u.whilee.body);
  if (body.ty->kind != Ty_void) {
    EM_error(e->pos, "while expression body must be void, got: %s",
             T_name(body.ty));
  }

  S_endScope(venv);
  return expTy(Tr_WhileExp(test.exp, body.exp, done), Ty_Void());
}

struct expty transForExp(Tr_level level, Temp_label done, S_table venv,
                         S_table tenv, A_exp e) {
  struct expty lo = transExp(level, done, venv, tenv, e->u.forr.lo);
  struct expty hi = transExp(level, done, venv, tenv, e->u.forr.hi);
  if (lo.ty->kind != Ty_int || hi.ty->kind != Ty_int) {
    EM_error(e->pos, "for expression range must be integer, lo: %s, hi: %s",
             T_name(lo.ty), T_name(hi.ty));
  }

  S_beginScope(venv);
  // TODO: how to set var read only?
  Tr_access access = Tr_allocLocal(level, e->u.forr.escape);
  S_enter(venv, e->u.forr.var, E_VarEntry(access, Ty_Int()));
  S_enter(venv, &loopsym, NULL);

  // 循环结束标号
  done = Temp_newlabel();

  struct expty body = transExp(level, done, venv, tenv, e->u.forr.body);
  if (body.ty->kind != Ty_void) {
    EM_error(e->pos, "for expression body must be void, got: %s",
             T_name(body.ty));
  }

  S_endScope(venv);

  Tr_exp var = Tr_SimpleVar(access, level);
  return expTy(Tr_ForExp(var, lo.exp, hi.exp, body.exp, done), Ty_Void());
}

struct expty transBreakExp(Tr_level level, Temp_label done, S_table venv,
                           S_table tenv, A_exp e) {
  E_enventry loop = S_look(venv, &loopsym);
  if (!loop) {
    EM_error(e->pos, "break expression must be in for or while loop");
  }

  return expTy(Tr_Break(done), Ty_Void());
}

struct expty transLetExp(Tr_level level, Temp_label done, S_table venv,
                         S_table tenv, A_exp e) {
  S_beginScope(tenv);

  A_decList l = e->u.let.decs;
  for (; l; l = l->tail) {
    transDec(level, done, venv, tenv, l->head);
  }

  assert(e->u.let.body->kind == A_seqExp);
  struct expty res = transSeqExp(level, done, venv, tenv, e->u.let.body);

  S_endScope(tenv);
  return res;
}

struct expty transArrayExp(Tr_level level, Temp_label done, S_table venv,
                           S_table tenv, A_exp e) {
  Ty_ty typ = actual_ty(S_look(tenv, e->u.array.typ));
  if (!typ) {
    EM_error(e->pos, "undeclared type: %s", S_name(e->u.array.typ));
  }

  if (typ->kind != Ty_array) {
    EM_error(e->pos, "array expression: %s must be array type, got: %s",
             S_name(e->u.array.typ), T_name(typ));
  }

  struct expty size = transExp(level, done, venv, tenv, e->u.array.size);
  if (size.ty->kind != Ty_int) {
    EM_error(e->pos, "array expression size must be integer, got: %s",
             T_name(size.ty));
  }

  struct expty init = transExp(level, done, venv, tenv, e->u.array.init);
  Ty_ty expected = actual_ty(typ->u.array);
  Ty_ty got = actual_ty(init.ty);
  if (expected != got) {
    EM_error(e->pos, "array expression mismatch type, expected: %s, got: %s",
             T_name(expected), T_name(got));
  }

  return expTy(Tr_ArrayExp(size.exp, init.exp), typ);
}

struct expty transExp(Tr_level level, Temp_label done, S_table venv,
                      S_table tenv, A_exp e) {
  switch (e->kind) {
    case A_varExp: {
      return transVar(level, done, venv, tenv, e->u.var);
    }
    case A_nilExp: {
      return expTy(Tr_NilExp(), Ty_Nil());
    }
    case A_intExp: {
      return expTy(Tr_IntExp(e->u.intt), Ty_Int());
    }
    case A_stringExp: {
      return expTy(Tr_StringExp(e->u.stringg), Ty_String());
    }
    case A_callExp: {
      return transCallExp(level, done, venv, tenv, e);
    }
    case A_opExp: {
      return transOpExp(level, done, venv, tenv, e);
    }
    case A_recordExp: {
      return transRecordExp(level, done, venv, tenv, e);
    }
    case A_seqExp: {
      return transSeqExp(level, done, venv, tenv, e);
    }
    case A_assignExp: {
      return transAssignExp(level, done, venv, tenv, e);
    }
    case A_ifExp: {
      return transIfExp(level, done, venv, tenv, e);
    }
    case A_whileExp: {
      return transWhileExp(level, done, venv, tenv, e);
    }
    case A_forExp: {
      return transForExp(level, done, venv, tenv, e);
    }
    case A_breakExp: {
      return transBreakExp(level, done, venv, tenv, e);
    }
    case A_letExp: {
      return transLetExp(level, done, venv, tenv, e);
    }
    case A_arrayExp: {
      return transArrayExp(level, done, venv, tenv, e);
    }
  }
  assert(0);
}

Tr_exp transDec(Tr_level level, Temp_label done, S_table venv, S_table tenv,
                A_dec d) {
  switch (d->kind) {
    case A_varDec: {
      struct expty e = transExp(level, done, venv, tenv, d->u.var.init);
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

      Tr_access access = Tr_allocLocal(level, d->u.var.escape);
      E_enventry entry = E_VarEntry(access, ty);
      S_enter(venv, d->u.var.var, entry);

      // 赋值表达式
      return Tr_AssignExp(Tr_SimpleVar(access, level), e.exp);
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
      return Tr_IntExp(0);
    }
    case A_functionDec: {
      A_fundecList l;
      S_table types = S_empty();

      // NOTE: 处理递归函数
      // 第一遍：收集每个函数的信息（函数名、形式参数、返回值类型），将函数加入到值环境中
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

        // 栈帧：创建新嵌套层
        Temp_label label = Temp_newlabel();
        U_boolList formals = makeFormalEscape(p->params);
        Tr_level new_level = Tr_newLevel(level, label, formals);
        S_enter(venv, p->name,
                E_FunEntry(new_level, label, formalTys, resultTy));
      }

      // 第二遍：处理每个函数体，将形式参数加入值环境中
      for (l = d->u.function; l; l = l->tail) {
        A_fundec p = l->head;
        E_enventry f = S_look(venv, p->name);
        assert(f);

        // 注意：此处使用函数嵌套层
        Tr_level level = f->u.fun.level;

        // 处理形式参数
        S_beginScope(venv);
        A_fieldList l;
        Ty_tyList t;
        for (l = p->params, t = f->u.fun.formals; l; l = l->tail, t = t->tail) {
          E_enventry entry =
              E_VarEntry(Tr_allocLocal(level, l->head->escape), t->head);
          S_enter(venv, l->head->name, entry);
        }

        // 处理函数体
        struct expty ty = transExp(level, done, venv, tenv, p->body);
        if (ty.ty != f->u.fun.result) {
          EM_error(d->pos, "mismatch return type, expected: %s, got: %s",
                   T_name(f->u.fun.result), T_name(ty.ty));
        }

        Tr_procEntryExit(level, ty.exp, Tr_formals(level));

        S_endScope(venv);
      }
      return Tr_IntExp(0);
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

F_fragList SEM_transProg(A_exp exp) {
  S_table venv = E_base_venv();
  S_table tenv = E_base_venv();

  S_enter(tenv, S_Symbol("int"), Ty_Int());
  S_enter(tenv, S_Symbol("string"), Ty_String());
  builtin(venv, tenv);

  struct expty tr_exp =
      transExp(Tr_newLevel(Tr_outermost(), Temp_newlabel(), NULL), NULL, venv,
               tenv, exp);

  printTrExp(tr_exp.exp);

  return Tr_getResult();
}