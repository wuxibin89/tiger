#ifndef FRAME_H_
#define FRAME_H_

#include "temp.h"
#include "tree.h"
#include "util.h"

extern const int F_wordSize;

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

/// 片段(fragment)：有两种类型的片段
/// 1. 字符串常量的汇编语言伪指令
/// 2. 函数描述
/// - 栈帧：包含局部变量和参数的机器相关信息
/// - 函数体：从procEntryExit1返回的结果
typedef struct F_frag_* F_frag;

struct F_frag_ {
  enum { F_stringFrag, F_procFrag } kind;

  union {
    struct {
      Temp_label label;
      string str;
    } stringg;
    struct {
      T_stm body;
      F_frame frame;
    } proc;
  } u;
};

/// 字符串常量片段
/// @param label 常量标号
/// @param str 常量值
F_frag F_StringFrag(Temp_label label, string str);

/// 函数片段
/// @param body 见F_procEntryExit1
/// @param frame 一个栈帧描述符，它包含有关局部变量和参数的机器相关信息
F_frag F_ProcFrag(T_stm body, F_frame frame);

typedef struct F_fragList_* F_fragList;
struct F_fragList_ {
  F_frag head;
  F_fragList tail;
};
F_fragList F_FragList(F_frag head, F_fragList tail);

/// 帧指针寄存器
Temp_temp F_FP(void);

// 返回值寄存器
Temp_temp F_RV(void);

/// @param acc 变量定义
/// @param framePtr 变量定义函数的帧地址
T_exp F_exp(F_access acc, T_exp framePtr);

/// @param s 外部函数名
/// @param args 外部函数参数
T_exp F_externalCall(string s, T_expList args);


/// 视角移位
/// TODO: ???
/// @param frame 栈帧描述符
/// @param stm
T_stm F_procEntryExit1(F_frame frame, T_stm stm);

#endif  // FRAME_H_