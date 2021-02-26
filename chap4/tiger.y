%{
#include <stdio.h>
#include "util.h"
#include "symbol.h" 
#include "errormsg.h"
#include "absyn.h"

int yylex(void); /* function prototype */

A_exp absyn_root;

void yyerror(char *s)
{
 EM_error(EM_tokPos, "%s", s);
}
%}


%union {
	int pos;
	int ival;
	string sval;

	A_var var;
  A_dec dec;
	A_exp exp;
  A_ty  ty;

  A_decList decList;
  A_expList expList;
  A_field field;
  A_fieldList fieldList;
  A_fundec fundec;
  A_fundecList fundecList;
  A_namety namety;
  A_nametyList nametyList;
  A_efield efield;
  A_efieldList efieldList;
}

%token <sval> ID STRING
%token <ival> INT

%token 
  COMMA COLON SEMICOLON LPAREN RPAREN LBRACK RBRACK 
  LBRACE RBRACE DOT 
  PLUS MINUS TIMES DIVIDE EQ NEQ LT LE GT GE
  AND OR ASSIGN
  ARRAY IF THEN ELSE WHILE FOR TO DO LET IN END OF 
  BREAK NIL
  FUNCTION VAR TYPE 

%type <decList> decs
%type <dec> dec vardec
%type <nametyList> tydecs
%type <fundecList> fundecs
%type <namety> tydec
%type <ty> ty
%type <fieldList> tyfields tyfield_list
%type <field> tyfield
%type <fundec> fundec
%type <var> lvalue
%type <efieldList> efields efield_list
%type <efield> efield
%type <exp> exp program call_exp op_exp record_exp array_exp if_exp while_exp for_exp let_exp 
%type <expList> expseq expseq_list args arg_list
/* et cetera */

%nonassoc LOW
%nonassoc TYPE FUNCTION
%nonassoc ID
%nonassoc LBRACK
%nonassoc ASSIGN
%left OR
%left AND
%nonassoc EQ NEQ
%nonassoc LT GT LE GE
%left PLUS MINUS
%left TIMES DIVIDE
%left UMINUS
%nonassoc DO
%nonassoc OF
%nonassoc THEN
%nonassoc ELSE

%start program

%%

program: exp              { absyn_root = $1; }

/* declaration */

decs:
                    { $$ = NULL; }
| dec decs                { $$ = A_DecList($1, $2); }
;

dec:
  tydecs                  { $$ = A_TypeDec(EM_tokPos, $1); }
| vardec                  { $$ = $1; }
| fundecs                 { $$ = A_FunctionDec(EM_tokPos, $1); }
;

tydecs:
  tydec    %prec LOW      { $$ = A_NametyList($1, NULL); }
| tydec tydecs            { $$ = A_NametyList($1, $2); }
;

tydec: TYPE ID EQ ty      { $$ = A_Namety(S_Symbol($2), $4); }

ty:
  ID                      { $$ = A_NameTy(EM_tokPos, S_Symbol($1)); }
| LBRACE tyfields RBRACE  { $$ = A_RecordTy(EM_tokPos, $2); }
| ARRAY OF ID             { $$ = A_ArrayTy(EM_tokPos, S_Symbol($3)); }
;

tyfields:
                    { $$ = NULL; }
| tyfield_list            { $$ = $1; }
;

tyfield_list:
  tyfield                         { $$ = A_FieldList($1, NULL); }
| tyfield_list COMMA tyfield      { $$ = A_FieldList($3, $1); }
;

tyfield: ID COLON ID              { $$ = A_Field(EM_tokPos, S_Symbol($1), S_Symbol($3)); }

vardec:
  VAR ID ASSIGN exp               { $$ = A_VarDec(EM_tokPos, S_Symbol($2), NULL, $4); }
| VAR ID COLON ID ASSIGN exp      { $$ = A_VarDec(EM_tokPos, S_Symbol($2), S_Symbol($4), $6); }
;

fundecs:
  fundec    %prec LOW             { $$ = A_FundecList($1, NULL); }
| fundec fundecs                  { $$ = A_FundecList($1, $2); }
;

fundec:
  FUNCTION ID LPAREN tyfields RPAREN EQ exp             { $$ = A_Fundec(EM_tokPos, S_Symbol($2), $4, NULL, $7); }
| FUNCTION ID LPAREN tyfields RPAREN COLON ID EQ exp    { $$ = A_Fundec(EM_tokPos, S_Symbol($2), $4, S_Symbol($7), $9); }
;

/* expression */

lvalue:
  ID                                  { $$ = A_SimpleVar(EM_tokPos, S_Symbol($1)); }
| lvalue DOT ID                       { $$ = A_FieldVar(EM_tokPos, $1, S_Symbol($3)); }
| ID LBRACK exp RBRACK                { $$ = A_SubscriptVar(EM_tokPos, A_SimpleVar(EM_tokPos, S_Symbol($1)), $3); }
| lvalue LBRACK exp RBRACK            { $$ = A_SubscriptVar(EM_tokPos, $1, $3); }
;

exp:
  NIL                                 { $$ = A_NilExp(EM_tokPos); }
| INT                                 { $$ = A_IntExp(EM_tokPos, $1); }
| STRING                              { $$ = A_StringExp(EM_tokPos, $1); }
| lvalue                              { $$ = A_VarExp(EM_tokPos, $1); }
| lvalue ASSIGN exp                   { $$ = A_AssignExp(EM_tokPos, $1, $3); }
| LPAREN expseq RPAREN                { $$ = A_SeqExp(EM_tokPos, $2); }
| MINUS exp  %prec UMINUS             { $$ = A_OpExp(EM_tokPos, A_minusOp, A_IntExp(EM_tokPos, 0), $2); }
| BREAK                               { $$ = A_BreakExp(EM_tokPos); }
| call_exp                            { $$ = $1; }
| op_exp                              { $$ = $1; }
| record_exp                          { $$ = $1; }
| array_exp                           { $$ = $1; }
| if_exp                              { $$ = $1; }
| while_exp                           { $$ = $1; }
| for_exp                             { $$ = $1; }
| let_exp                             { $$ = $1; }
;

expseq:
                                { $$ = NULL; }
| expseq_list                         { $$ = $1; }
;

expseq_list:
  exp                                 { $$ = A_ExpList($1, NULL); }
| expseq_list SEMICOLON exp           { $$ = A_ExpList($3, $1); }
;

call_exp: ID LPAREN args RPAREN       { $$ = A_CallExp(EM_tokPos, S_Symbol($1), $3); }

args:
                                { $$ = NULL; }
| arg_list                            { $$ = $1; }
;

arg_list:
  exp                                 { $$ = A_ExpList($1, NULL); }
| arg_list COMMA exp                  { $$ = A_ExpList($3, $1); }
;

op_exp: 
  exp PLUS exp      { $$ = A_OpExp(EM_tokPos, A_plusOp, $1, $3); }
| exp MINUS exp     { $$ = A_OpExp(EM_tokPos, A_minusOp, $1, $3); }
| exp TIMES exp     { $$ = A_OpExp(EM_tokPos, A_timesOp, $1, $3); }
| exp DIVIDE exp    { $$ = A_OpExp(EM_tokPos, A_divideOp, $1, $3); }
| exp LT exp        { $$ = A_OpExp(EM_tokPos, A_ltOp, $1, $3); }
| exp GT exp        { $$ = A_OpExp(EM_tokPos, A_gtOp, $1, $3); }
| exp LE exp        { $$ = A_OpExp(EM_tokPos, A_leOp, $1, $3); }
| exp GE exp        { $$ = A_OpExp(EM_tokPos, A_geOp, $1, $3); }
| exp EQ exp        { $$ = A_OpExp(EM_tokPos, A_eqOp, $1, $3); }
| exp NEQ exp       { $$ = A_OpExp(EM_tokPos, A_neqOp, $1, $3); }
| exp AND exp       { $$ = A_OpExp(EM_tokPos, A_andOp, $1, $3); }
| exp OR exp        { $$ = A_OpExp(EM_tokPos, A_orOp, $1, $3); }
;

record_exp: ID LBRACE efields RBRACE    { $$ = A_RecordExp(EM_tokPos, S_Symbol($1), $3); }

efields:
                                  { $$ = NULL; }
| efield_list                           { $$ = $1; }
;

efield_list:
  efield                                { $$ = A_EfieldList($1, NULL); }
| efield_list COMMA efield              { $$ = A_EfieldList($3, $1); }
;

efield: ID EQ exp                       { $$ = A_Efield(S_Symbol($1), $3); }

array_exp: ID LBRACK exp RBRACK OF exp  { $$ = A_ArrayExp(EM_tokPos, S_Symbol($1), $3, $6); }

if_exp:
  IF exp THEN exp                       { $$ = A_IfExp(EM_tokPos, $2, $4, NULL); }
| IF exp THEN exp ELSE exp              { $$ = A_IfExp(EM_tokPos, $2, $4, $6); }
;

while_exp: WHILE exp DO exp             { $$ = A_WhileExp(EM_tokPos, $2, $4); }

for_exp: FOR ID ASSIGN exp TO exp DO exp { $$ = A_ForExp(EM_tokPos, S_Symbol($2), $4, $6, $8); }

let_exp: LET decs IN expseq END         { $$ = A_LetExp(EM_tokPos, $2, A_SeqExp(EM_tokPos, $4)); }
