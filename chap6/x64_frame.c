#include "frame.h"

unsigned WORD_SIZE = 8;

struct F_frame_ {
  Temp_label name;
  F_accessList formals;
  unsigned locals;
};

struct F_access_ {
  enum { inFrame, inReg } kind;
  union {
    int offset;     // InFrame
    Temp_temp reg;  // InReg
  } u;
};

F_access InFrame(int offset) {
  F_access p = checked_malloc(sizeof(*p));
  p->kind = inFrame;
  p->u.offset = offset;
  return p;
}

F_access InReg(Temp_temp reg) {
  F_access p = checked_malloc(sizeof(*p));
  p->kind = inReg;
  p->u.reg = reg;
  return p;
}

F_accessList F_AccessList(F_access head, F_accessList tail) {
  F_accessList p = checked_malloc(sizeof(*p));
  p->head = head;
  p->tail = tail;
  return p;
}

F_frame F_newFrame(Temp_label name, U_boolList formals) {
  F_frame p = checked_malloc(sizeof(*p));
  p->name = name;
  p->formals = NULL;
  p->locals = 0;

  F_accessList head = NULL, tail = NULL;
  for (int offset = 0; formals; formals = formals->tail, offset += WORD_SIZE) {
    F_access access = (formals->head) ? InFrame(offset) : InReg(Temp_newtemp());
    F_accessList t = F_AccessList(access, NULL);
    if (!head) {
      head = t;
      tail = t;
    } else {
      tail->tail = t;
      tail = t;
    }
  }
  p->formals = head;

  return p;
}

Temp_label F_name(F_frame f) { return f->name; }

F_accessList F_formals(F_frame f) { return f->formals; }

F_access F_allocLocal(F_frame f, bool escape) {
  assert(f);
  F_access frame = InFrame(-f->locals++ * WORD_SIZE);
  F_access reg = InReg(Temp_newtemp());
  return escape ? frame : reg;
}
