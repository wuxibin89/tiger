%{
#include <stdio.h>
#include "util.h"
#include "errormsg.h"

int yylex(void); /* function prototype */

void yyerror(char *s)
{
 EM_error(EM_tokPos, "%s", s);
}
%}

%union {
	int pos;
	int ival;
	string sval;
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

%left OR
%left AND
%nonassoc LT GT LE GE
%nonassoc EQ NEQ
%left PLUS MINUS
%left TIMES DIVIDE
%left UMINUS

%start program

%%

program:
  /* empty */
| statement
;

/* ==================== expression ==================== */
primary_expression:
  ID
| NIL
| INT
| STRING
| LPAREN RPAREN
| LPAREN expression RPAREN
;

array_expression:
  ID LBRACK expression RBRACK;

postfix_expression:
  primary_expression
| array_expression
| postfix_expression DOT ID
| postfix_expression LBRACK expression RBRACK
| postfix_expression LPAREN RPAREN
| postfix_expression LPAREN argument_list RPAREN
;

argument_list:
  expression
| argument_list COMMA expression
;

unary_expression:
  postfix_expression
| MINUS unary_expression %prec UMINUS
;

multiplicative_expression:
  unary_expression
| multiplicative_expression TIMES unary_expression
| multiplicative_expression DIVIDE unary_expression
;

additive_expression:
  multiplicative_expression
| additive_expression PLUS multiplicative_expression
| additive_expression MINUS multiplicative_expression
;

equality_expression:
  additive_expression
| equality_expression EQ additive_expression
| equality_expression NEQ additive_expression
;

relational_expression:
  equality_expression
| relational_expression LT equality_expression
| relational_expression GT equality_expression
| relational_expression LE equality_expression
| relational_expression GE equality_expression
;

AND_expression:
  relational_expression
| AND_expression AND relational_expression
;

inclusive_OR_expression:
  AND_expression
| inclusive_OR_expression OR AND_expression
;

array_creation: array_expression OF value_expression;

struct_creation: 
  ID LBRACE RBRACE
| ID LBRACE field_list RBRACE
;

field_list:
  ID EQ expression
| field_list COMMA ID EQ expression
;

value_expression:
  inclusive_OR_expression
| array_creation
| struct_creation
;

assignment_expression: 
  value_expression
| unary_expression ASSIGN value_expression
;

expression:
  statement
| assignment_expression
| expression SEMICOLON statement
| expression SEMICOLON assignment_expression
;

/* ==================== declaration ==================== */
declaration:
  type_declaration
| var_declaration
| function_declaration
;

declaration_list:
  declaration
| declaration_list declaration
;

type_declaration: TYPE ID EQ type_definition;

type_definition:
  ID
| LBRACE RBRACE
| LBRACE type_list RBRACE
| ARRAY OF ID
;

type_list:
  ID COLON ID
| type_list COMMA ID COLON ID
;

var_declaration:
  VAR ID ASSIGN expression
| VAR ID COLON ID ASSIGN expression
;

function_declaration:
  FUNCTION ID LPAREN RPAREN EQ expression
| FUNCTION ID LPAREN RPAREN COLON ID EQ expression
| FUNCTION ID LPAREN type_list RPAREN EQ expression
| FUNCTION ID LPAREN type_list RPAREN COLON ID EQ expression

/* ==================== statement ==================== */
statement:
  BREAK
| IF expression THEN expression
| IF expression THEN expression ELSE expression
| WHILE expression DO expression
| FOR ID ASSIGN expression TO expression DO expression
| LET declaration_list IN expression END
| LET declaration_list IN END
;

%%