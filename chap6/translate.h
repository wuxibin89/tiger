#ifndef TRANSLATE_H_
#define TRANSLATE_H_

#include "absyn.h"
#include "frame.h"
#include "temp.h"
#include "util.h"

// 语义分析阶段，每个函数创建一个新的嵌套层
typedef struct Tr_level_* Tr_level;

// 由变量的层次level和它的F_access组成（需要知道静态链相关信息）
typedef struct Tr_access_* Tr_access;

// 已转化为中间代码(IR)的表达式
typedef struct Tr_exp_* Tr_exp;

typedef struct Tr_accessList_* Tr_accessList;
struct Tr_accessList_ {
  Tr_access head;
  Tr_accessList tail;
};

Tr_accessList Tr_AccessList(Tr_access head, Tr_accessList tail);

typedef struct Tr_expList_* Tr_expList;
struct Tr_expList_ {
  Tr_exp head;
  Tr_expList tail;
};

Tr_expList Tr_ExpList(Tr_exp head, Tr_expList tail);

/// 与frame模块交互接口

Tr_level Tr_outermost(void);
Tr_level Tr_newLevel(Tr_level parent, Temp_label name, U_boolList formals);
Tr_accessList Tr_formals(Tr_level level);
Tr_access Tr_allocLocal(Tr_level level, bool escape);

/// 将函数片段保存至片段表
/// @param level 嵌套层
/// @param body 函数体
/// @param formals 函数参数
void Tr_procEntryExit(Tr_level level, Tr_exp body, Tr_accessList formals);

/// 获取全局片段表(Fragment)：字符串常量片段+函数片段
F_fragList Tr_getResult(void);

/// 以下IR对应抽象语法树，参见：chap4/absyn.h，与semant模块交互接口

/// 简单变量
/// @param access 从Tr_allocLocal得到的
/// @param level 变量使用所在函数的level（可能与变量定义不在同一level）
Tr_exp Tr_SimpleVar(Tr_access access, Tr_level level);

/// 记录域
/// @param record 记录表达式
/// @param offset 记录域偏移量
Tr_exp Tr_FieldVar(Tr_exp record, unsigned offset);

/// 数组下标
/// @param array 数组表达式
/// @param index 下标表达式
Tr_exp Tr_SubscriptVar(Tr_exp array, Tr_exp index);

/// nil常量
Tr_exp Tr_NilExp();

/// 整型常量
/// @param i 常量值
Tr_exp Tr_IntExp(int i);

/// 字符串常量
/// @param s 常量值
Tr_exp Tr_StringExp(string s);

// TODO: 字符串变量？

/// 算术操作
/// @param op 算术和比较操作符
/// @param left 左表达式
/// @param right 右表达式
Tr_exp Tr_OpExp(A_oper op, Tr_exp left, Tr_exp right);

/// 记录元素创建
/// @param fields 记录元素表达式
/// @param n_fields 记录元素数量
Tr_exp Tr_RecordExp(Tr_expList fields, unsigned n_fields);

/// 数组创建
/// @param len 数组长度
/// @param val 数组元素初始值
Tr_exp Tr_ArrayExp(Tr_exp len, Tr_exp val);

/// 赋值表达式
/// @param var 左值
/// @param val 右值
Tr_exp Tr_AssignExp(Tr_exp var, Tr_exp val);

/// if表达式
/// @param test 条件表达式
/// @param then 真值分支
/// @param elsee 假值分支
Tr_exp Tr_IfExp(Tr_exp test, Tr_exp then, Tr_exp elsee);

/// while表达式
/// @param test 循环条件
/// @param body 循环体
/// @param done 循环结束标号
Tr_exp Tr_WhileExp(Tr_exp test, Tr_exp body, Temp_label done);

/// for表达式
/// @param var 循环变量
/// @param lo 循环下界
/// @param hi 循环上界
/// @param body 循环体
/// @param done 循环结束标号
Tr_exp Tr_ForExp(Tr_exp var, Tr_exp lo, Tr_exp hi, Tr_exp body,
                 Temp_label done);

/// 函数调用
/// @param call_level 函数调用嵌套层
/// @param fun_level 函数定义嵌套层
/// @param fun_label 函数标号
/// @param args 入参
Tr_exp Tr_CallExp(Tr_level call_level, Tr_level fun_level, Temp_label fun_label,
                  Tr_expList args);

/// break表达式
/// @param done 循环结束标号
Tr_exp Tr_Break(Temp_label done);

/// 打印IR
void printTrExp(Tr_exp exp);

#endif  // TRANSLATE_H_