#ifndef PROG1_H_
#define PROG1_H_

#include "slp.h"

A_stm prog(void);
int maxargs(A_exp);
int maxargs(A_stm);

typedef struct table *Table_;
struct table {
    string id;
    int value;
    Table_ tail;
};

Table_ Table(string id, int value, struct table *tail);

struct IntAndTable {
    IntAndTable(int iv, Table_ tv): i(iv), t(tv) {}
    int i;
    Table_ t;
};

Table_ interpStm(A_stm stm, Table_ *t);
IntAndTable interpExp(A_exp exp, Table_ *t);
void interp(A_stm);

#endif // PROG1_H_