#ifndef FRAME_H_
#define FRAME_H_

#include "temp.h"
#include "util.h"

// 栈帧：形式参数和分配的局部变量信息
typedef struct F_frame_* F_frame;

// 描述可以存放在栈中或寄存器中形式参数和局部变量
typedef struct F_access_* F_access;

typedef struct F_accessList_* F_accessList;
struct F_accessList_ {
  F_access head;
  F_accessList tail;
};

F_accessList F_AccessList(F_access head, F_accessList tail);

F_frame F_newFrame(Temp_label name, U_boolList formals);
Temp_label F_name(F_frame f);
F_accessList F_formals(F_frame f);
F_access F_allocLocal(F_frame f, bool escape);

#endif  // FRAME_H_