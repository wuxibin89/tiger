#include "translate.h"

#include "frame.h"
#include "temp.h"
#include "util.h"

struct Tr_access_ {
  Tr_level level;
  F_access access;
};

struct Tr_level_ {
  Tr_level parent;
  F_frame frame;
};

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