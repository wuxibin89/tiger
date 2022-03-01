#include "translate.h"

#include "frame.h"
#include "printtree.h"
#include "temp.h"
#include "tree.h"
#include "util.h"

// 需要填充标号的地点，用于条件转移
typedef struct patchList_ *patchList;

struct patchList_ {
  Temp_label *head;
  patchList tail;
};

struct Tr_access_ {
  Tr_level level;
  F_access access;
};

struct Tr_level_ {
  Tr_level parent;
  F_frame frame;
};

struct Cx {
  patchList trues;
  patchList falses;
  T_stm stm;
};

// Ex: 表达式
// Nx：无结果语句
// Cx：条件语句，表示为一个可能转移到两个标号之一的语句
struct Tr_exp_ {
  enum { Tr_ex, Tr_nx, Tr_cx } kind;
  union {
    T_exp ex;
    T_stm nx;
    struct Cx cx;
  } u;
};

static patchList PatchList(Temp_label *head, patchList tail) {
  patchList p = checked_malloc(sizeof(*p));
  p->head = head;
  p->tail = tail;
  return p;
}

static void doPatch(patchList tList, Temp_label label) {
  for (; tList; tList = tList->tail) {
    assert(tList->head);
    *(tList->head) = label;
  }
}

static patchList joinPatch(patchList first, patchList second) {
  if (!first) return second;
  for (; first->tail; first = first->tail) {
  }  // go to end of list
  first->tail = second;
  return first;
}

static Tr_exp Tr_Ex(T_exp ex) {
  Tr_exp p = checked_malloc(sizeof(*p));
  p->kind = Tr_ex;
  p->u.ex = ex;
  return p;
}

static Tr_exp Tr_Nx(T_stm nx) {
  Tr_exp p = checked_malloc(sizeof(*p));
  p->kind = Tr_nx;
  p->u.nx = nx;
  return p;
}

static Tr_exp Tr_Cx(patchList trues, patchList falses, T_stm stm) {
  Tr_exp p = checked_malloc(sizeof(*p));
  p->kind = Tr_cx;
  p->u.cx.trues = trues;
  p->u.cx.falses = falses;
  p->u.cx.stm = stm;
  return p;
}

static T_exp unEx(Tr_exp e) {
  switch (e->kind) {
    case Tr_ex:
      return e->u.ex;
    case Tr_nx:
      return T_Eseq(e->u.nx, T_Const(0));
    case Tr_cx: {
      T_exp r = T_Temp(Temp_newtemp());
      Temp_label t = Temp_newlabel(), f = Temp_newlabel();
      doPatch(e->u.cx.trues, t);
      doPatch(e->u.cx.falses, f);
      return T_Eseq(T_Move(r, T_Const(1)),
                    T_Eseq(e->u.cx.stm,
                           T_Eseq(T_Label(f), T_Eseq(T_Move(r, T_Const(0)),
                                                     T_Eseq(T_Label(t), r)))));
    }
  }
  assert(0);
}

static T_stm unNx(Tr_exp e) {
  switch (e->kind) {
    case Tr_ex:
      return T_Exp(e->u.ex);
    case Tr_nx:
      return e->u.nx;
    case Tr_cx:
      return e->u.cx.stm;
  }
  assert(0);
}

static struct Cx unCx(Tr_exp e) {
  switch (e->kind) {
    case Tr_ex: {
      // 真值标号和假值标号的目标地址一直要到非常晚的时候才能知道
      T_stm stm = T_Cjump(T_ne, e->u.ex, T_Const(0), NULL, NULL);
      patchList trues = PatchList(&(stm->u.CJUMP.t), NULL),
                falses = PatchList(&(stm->u.CJUMP.f), NULL);
      struct Cx cx = {trues, falses, stm};
      return cx;
    }
    case Tr_nx:
      assert(0);
    case Tr_cx:
      return e->u.cx;
  }
  assert(0);
}

Tr_access Tr_newAccess(Tr_level level, F_access access) {
  Tr_access p = checked_malloc(sizeof(*p));
  p->level = level;
  p->access = access;
  return p;
}

Tr_accessList Tr_AccessList(Tr_access head, Tr_accessList tail) {
  Tr_accessList p = checked_malloc(sizeof(*p));
  p->head = head;
  p->tail = tail;
  return p;
}

Tr_level Tr_outermost(void) {
  static Tr_level p = NULL;
  if (!p) {
    p = checked_malloc(sizeof(*p));
    p->parent = NULL;
    p->frame = NULL;
  }
  return p;
}

Tr_level Tr_newLevel(Tr_level parent, Temp_label name, U_boolList formals) {
  Tr_level p = checked_malloc(sizeof(*p));
  p->parent = parent;
  // NOTE: add static link as extra parameter which does escape
  p->frame = F_newFrame(name, U_BoolList(TRUE, formals));
  return p;
}

Tr_accessList Tr_formals(Tr_level level) {
  F_accessList formals = F_formals(level->frame);
  assert(formals && formals->head);

  Tr_accessList head = NULL, tail = NULL;
  // NOTE: skip static link
  for (formals = formals->tail; formals; formals = formals->tail) {
    Tr_accessList t = Tr_AccessList(Tr_newAccess(level, formals->head), NULL);
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

Tr_access Tr_allocLocal(Tr_level level, bool escape) {
  Tr_access p = checked_malloc(sizeof(*p));
  p->level = level;
  p->access = F_allocLocal(level->frame, escape);
  return p;
}

Tr_expList Tr_ExpList(Tr_exp head, Tr_expList tail) {
  Tr_expList p = checked_malloc(sizeof(*p));
  p->head = head;
  p->tail = tail;
  return p;
}

Tr_exp Tr_SimpleVar(Tr_access access, Tr_level level) {
  T_exp static_link = T_Const(F_wordSize);
  T_exp frame_ptr = T_Temp(F_FP());
  // 追踪静态链
  for (; level != access->level; level = level->parent) {
    frame_ptr = T_Mem(T_Binop(T_plus, frame_ptr, static_link));
  }

  return Tr_Ex(F_exp(access->access, frame_ptr));
}

Tr_exp Tr_FieldVar(Tr_exp record, unsigned offset) {
  return Tr_Ex(
      T_Mem(T_Binop(T_plus, unEx(record), T_Const(offset * F_wordSize))));
}

Tr_exp Tr_SubscriptVar(Tr_exp array, Tr_exp index) {
  // TODO: 增加越界检查
  return Tr_Ex(T_Mem(T_Binop(
      T_plus, unEx(array), T_Binop(T_mul, unEx(index), T_Const(F_wordSize)))));
}

// NOTE: nil is treated as a record with no memory space
Tr_exp Tr_NilExp() {
  static T_exp nil = NULL;

  if (!nil) {
    nil = T_Temp(Temp_newtemp());
    return Tr_Ex(T_Eseq(
        T_Move(nil,
               F_externalCall(String("malloc"), T_ExpList(T_Const(0), NULL))),
        nil));
  }

  return Tr_Ex(nil);
}

Tr_exp Tr_IntExp(int i) { return Tr_Ex(T_Const(i)); }

// NOTE：这里维护一个fraglist表示全局字符串, 为每个字符串生成label
static F_fragList global_string = NULL;

Tr_exp Tr_StringExp(string s) {
  Temp_label label = Temp_newlabel();

  // 字符串常量放到全局表中
  F_frag frag = F_StringFrag(label, s);
  global_string = F_FragList(frag, global_string);

  return Tr_Ex(T_Name(label));
}

// NOTE: 全局函数片段
static F_fragList global_proc = NULL;

void Tr_procEntryExit(Tr_level level, Tr_exp body, Tr_accessList formals) {
  F_frag frag = F_ProcFrag(unNx(body), level->frame);
  global_proc = F_FragList(frag, global_proc);
}

F_fragList Tr_getResult(void) {
  if (!global_string) {
    return global_proc;
  }

  F_fragList p = global_string;
  while (p->tail) p = p->tail;
  p->tail = global_proc;

  return global_string;
}

Tr_exp Tr_RecordExp(Tr_expList fields, unsigned n_fields) {
  T_exp r = T_Temp(Temp_newtemp());

  // 调用外部函数malloc创建记录
  T_stm stm = T_Seq(
      T_Move(r,
             F_externalCall(String("malloc"),
                            T_ExpList(T_Const(n_fields * F_wordSize), NULL))),
      NULL);

  unsigned i = 0;
  for (; fields; fields = fields->tail, ++i) {
    stm = T_Seq(stm, T_Move(T_Mem(T_Binop(T_plus, r, T_Const(i * F_wordSize))),
                            unEx(fields->head)));  // 元素偏移量
  }
  assert(i == n_fields);

  return Tr_Ex(T_Eseq(stm, r));
}

Tr_exp Tr_ArrayExp(Tr_exp len, Tr_exp val) {
  return Tr_Ex(F_externalCall(
      String("initArray"), T_ExpList(unEx(len), T_ExpList(unEx(val), NULL))));
}

Tr_exp Tr_OpExp(A_oper op, Tr_exp left, Tr_exp right) {
  switch (op) {
    // 算术操作
    case A_plusOp:
    case A_minusOp:
    case A_timesOp:
    case A_divideOp:
      return Tr_Ex(T_Binop((T_binOp)op, unEx(left), unEx(right)));
    // 比较操作
    case A_eqOp:
    case A_neqOp:
    case A_ltOp:
    case A_gtOp:
    case A_leOp:
    case A_geOp: {
      T_stm stm = T_Cjump((T_relOp)op, unEx(left), unEx(right), NULL, NULL);
      patchList trues = PatchList(&(stm->u.CJUMP.t), NULL),
                falses = PatchList(&(stm->u.CJUMP.f), NULL);
      return Tr_Cx(trues, falses, stm);
    }
  }
  assert(0);
}

Tr_exp Tr_AssignExp(Tr_exp var, Tr_exp val) {
  return Tr_Nx(T_Move(unEx(var), unEx(val)));
}

Tr_exp Tr_IfExp(Tr_exp test, Tr_exp then, Tr_exp elsee) {
  struct Cx test_cx = unCx(test);

  // 填充真假值标号
  Temp_label t = Temp_newlabel(), f = Temp_newlabel();
  doPatch(test_cx.trues, t);
  doPatch(test_cx.falses, f);

  // if-then表达式必须无值
  if (!elsee) {
    return Tr_Nx(
        T_Seq(test_cx.stm, T_Seq(T_Label(t), T_Seq(unNx(then), T_Label(f)))));
  }

  Temp_label z = Temp_newlabel();
  Temp_labelList l = Temp_LabelList(z, NULL);
  T_stm jump = T_Jump(T_Name(z), l);

  // if-then-else无值表达式
  if (then->kind == Tr_nx) {
    return Tr_Nx(T_Seq(
        test_cx.stm,
        T_Seq(T_Label(t),
              T_Seq(unNx(then),
                    T_Seq(jump, T_Seq(T_Label(f),
                                      T_Seq(unNx(elsee), T_Label(z))))))));
  }

  // if-then-else有值表达式
  T_exp r = T_Temp(Temp_newtemp());
  T_exp then_exp = unEx(then), elsee_exp = unEx(elsee);
  return Tr_Ex(T_Eseq(
      test_cx.stm,
      T_Eseq(T_Label(t),
             T_Eseq(T_Move(r, then_exp),
                    T_Eseq(jump, T_Eseq(T_Label(f),
                                        T_Eseq(T_Move(r, elsee_exp), r)))))));
}

Tr_exp Tr_WhileExp(Tr_exp test, Tr_exp body, Temp_label done) {
  struct Cx test_cx = unCx(test);

  Temp_label test_label = Temp_newlabel();
  Temp_labelList l = Temp_LabelList(test_label, NULL);
  T_stm jump_test = T_Jump(T_Name(test_label), l);

  // 填充真假值标号
  Temp_label body_label = Temp_newlabel();
  doPatch(test_cx.trues, body_label);
  doPatch(test_cx.falses, done);

  return Tr_Nx(
      T_Seq(T_Label(test_label),
            T_Seq(test_cx.stm,
                  T_Seq(T_Label(body_label),
                        T_Seq(unNx(body),
                              T_Seq(jump_test, T_Seq(T_Label(done), NULL)))))));
}

Tr_exp Tr_ForExp(Tr_exp var, Tr_exp lo, Tr_exp hi, Tr_exp body,
                 Temp_label done) {
  T_exp i = unEx(var);

  Temp_label body_label = Temp_newlabel();
  T_stm test = T_Cjump(A_leOp, i, unEx(hi), body_label, done);

  Temp_label test_label = Temp_newlabel();
  Temp_labelList l = Temp_LabelList(test_label, NULL);
  T_stm jump_test = T_Jump(T_Name(test_label), l);

  T_stm incr = T_Move(i, T_Binop(A_plusOp, i, T_Const(1)));
  return Tr_Nx(T_Seq(
      T_Label(test_label),
      T_Seq(test, T_Seq(T_Label(body_label),
                        T_Seq(unNx(body),
                              T_Seq(incr, T_Seq(jump_test, T_Seq(T_Label(done),
                                                                 NULL))))))));
}

Tr_exp Tr_Break(Temp_label done) {
  assert(done);
  return Tr_Nx(T_Label(done));
}

Tr_exp Tr_CallExp(Tr_level call_level, Tr_level fun_level, Temp_label fun_label,
                  Tr_expList args) {
  // 追踪静态链，作为额外参数
  T_exp static_link = T_Const(F_wordSize);
  T_exp frame_ptr = T_Temp(F_FP());
  for (; call_level != fun_level->parent; call_level = call_level->parent) {
    frame_ptr = T_Mem(T_Binop(T_plus, frame_ptr, static_link));
  }

  T_expList head = T_ExpList(frame_ptr, NULL), tail = head;
  for (; args; args = args->tail) {
    assert(args->head);
    tail->tail = T_ExpList(unEx(args->head), NULL);
    tail = tail->tail;
  }

  return Tr_Ex(T_Call(T_Name(fun_label), head));
}

void printTrExp(Tr_exp exp) {
  printStmList(stdout, T_StmList(unNx(exp), NULL));
}