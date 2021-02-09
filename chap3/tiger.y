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
%nonassoc EQ NEQ
%nonassoc LT GT LE GE
%left PLUS MINUS
%left TIMES DIVIDE
%left UMINUS

%start program

%%

program: statement;

/* expression */

primary_expression:
  NIL
| INT
| STRING
| LPAREN expression RPAREN
| ID LBRACE type_fields_initializer RBRACE
;

type_fields_initializer:
  /* %empty */
| type_fields_initializer_list
;

type_fields_initializer_list:
  ID EQ expression
| type_fields_initializer_list COMMA ID EQ expression
;

postfix_expression:
  primary_expression
| ID
| ID LBRACK expression RBRACK
| postfix_expression LBRACK expression RBRACK
| postfix_expression LPAREN RPAREN
| postfix_expression LPAREN argument_expression_list RPAREN
| postfix_expression DOT ID
;

argument_expression_list:
  statement
| argument_expression_list COMMA statement
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

relational_expression:
  additive_expression
| relational_expression LT additive_expression
| relational_expression GT additive_expression
| relational_expression LE additive_expression
| relational_expression GE additive_expression
;

equality_expression:
  relational_expression
| equality_expression EQ relational_expression
| equality_expression NEQ relational_expression
;

logical_and_expression:
  equality_expression
| logical_and_expression AND equality_expression
;

logical_or_expression:
  logical_and_expression
| logical_or_expression OR logical_and_expression
;

assignment_expression:
  logical_or_expression
| unary_expression ASSIGN assignment_expression
;

expression:
  assignment_expression
| ID LBRACK expression RBRACK OF expression
| compound_statement
;

/* declaration */

declaration:
  /* %empty */
| init_declarator_list
;

init_declarator_list:
  init_declarator
| init_declarator_list init_declarator
;

init_declarator:
  type_declarator
| var_declarator
| function_declarator
;

type_declarator:
  TYPE ID EQ type_definition;

type_definition:
  ID
| LBRACE type_fields RBRACE
| ARRAY OF ID
;

type_fields:
  /* %empty */
| init_type_fields_list
;

init_type_fields_list:
  ID COLON ID
| init_type_fields_list COMMA ID COLON ID
;

var_declarator:
  VAR ID ASSIGN expression
| VAR ID COLON ID ASSIGN expression
;

function_declarator:
  FUNCTION ID LPAREN type_fields RPAREN EQ statement
| FUNCTION ID LPAREN type_fields RPAREN COLON ID EQ statement 
;

/* statement */

statement:
  expression_statement
| selection_statement
| iteration_statement
| jump_statement
| let_statement
;

compound_statement: LPAREN init_statement_list RPAREN;

init_statement_list:
  /* %empty */
| statement_list
;

statement_list:
  statement
| statement_list SEMICOLON statement
;

expression_statement: expression;

selection_statement:
  IF expression THEN statement
| IF expression THEN statement ELSE statement
;

iteration_statement:
  WHILE expression DO statement
| FOR ID ASSIGN expression TO expression DO statement
;

jump_statement: BREAK;

let_statement: LET declaration IN statement_list END;