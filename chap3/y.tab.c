#ifndef lint
static const char yysccsid[] = "@(#)yaccpar	1.9 (Berkeley) 02/21/93";
#endif

#define YYBYACC 1
#define YYMAJOR 1
#define YYMINOR 9
#define YYPATCH 20130304

#define YYEMPTY        (-1)
#define yyclearin      (yychar = YYEMPTY)
#define yyerrok        (yyerrflag = 0)
#define YYRECOVERING() (yyerrflag != 0)

#define YYPREFIX "yy"

#define YYPURE 0

#line 2 "tiger.y"
#include <stdio.h>
#include "util.h"
#include "errormsg.h"

int yylex(void); /* function prototype */

void yyerror(char *s)
{
 EM_error(EM_tokPos, "%s", s);
}
#line 15 "tiger.y"
#ifdef YYSTYPE
#undef  YYSTYPE_IS_DECLARED
#define YYSTYPE_IS_DECLARED 1
#endif
#ifndef YYSTYPE_IS_DECLARED
#define YYSTYPE_IS_DECLARED 1
typedef union {
	int pos;
	int ival;
	string sval;
	} YYSTYPE;
#endif /* !YYSTYPE_IS_DECLARED */
#line 43 "y.tab.c"

/* compatibility with bison */
#ifdef YYPARSE_PARAM
/* compatibility with FreeBSD */
# ifdef YYPARSE_PARAM_TYPE
#  define YYPARSE_DECL() yyparse(YYPARSE_PARAM_TYPE YYPARSE_PARAM)
# else
#  define YYPARSE_DECL() yyparse(void *YYPARSE_PARAM)
# endif
#else
# define YYPARSE_DECL() yyparse(void)
#endif

/* Parameters sent to lex. */
#ifdef YYLEX_PARAM
# define YYLEX_DECL() yylex(void *YYLEX_PARAM)
# define YYLEX yylex(YYLEX_PARAM)
#else
# define YYLEX_DECL() yylex(void)
# define YYLEX yylex()
#endif

/* Parameters sent to yyerror. */
#ifndef YYERROR_DECL
#define YYERROR_DECL() yyerror(const char *s)
#endif
#ifndef YYERROR_CALL
#define YYERROR_CALL(msg) yyerror(msg)
#endif

extern int YYPARSE_DECL();

#define ID 257
#define STRING 258
#define INT 259
#define COMMA 260
#define COLON 261
#define SEMICOLON 262
#define LPAREN 263
#define RPAREN 264
#define LBRACK 265
#define RBRACK 266
#define LBRACE 267
#define RBRACE 268
#define DOT 269
#define PLUS 270
#define MINUS 271
#define TIMES 272
#define DIVIDE 273
#define EQ 274
#define NEQ 275
#define LT 276
#define LE 277
#define GT 278
#define GE 279
#define AND 280
#define OR 281
#define ASSIGN 282
#define ARRAY 283
#define IF 284
#define THEN 285
#define ELSE 286
#define WHILE 287
#define FOR 288
#define TO 289
#define DO 290
#define LET 291
#define IN 292
#define END 293
#define OF 294
#define BREAK 295
#define NIL 296
#define FUNCTION 297
#define VAR 298
#define TYPE 299
#define UMINUS 300
#define YYERRCODE 256
static const short yylhs[] = {                           -1,
    0,    2,    2,    2,    2,    2,    4,    4,    5,    5,
    6,    6,    6,    6,    6,    6,    6,    7,    7,    8,
    8,    9,    9,    9,   10,   10,   10,   11,   11,   11,
   11,   11,   12,   12,   12,   13,   13,   14,   14,   15,
   15,    3,    3,    3,   17,   17,   18,   18,   19,   19,
   19,   20,   23,   23,   23,   24,   24,   25,   25,   21,
   21,   22,   22,    1,    1,    1,    1,    1,   16,   31,
   31,   32,   32,   26,   27,   27,   28,   28,   29,   30,
};
static const short yylen[] = {                            2,
    1,    1,    1,    1,    3,    4,    0,    1,    3,    5,
    1,    1,    4,    4,    3,    4,    3,    1,    3,    1,
    2,    1,    3,    3,    1,    3,    3,    1,    3,    3,
    3,    3,    1,    3,    3,    1,    3,    1,    3,    1,
    3,    1,    6,    1,    0,    1,    1,    2,    1,    1,
    1,    4,    1,    3,    3,    0,    1,    3,    5,    4,
    6,    7,    9,    1,    1,    1,    1,    1,    3,    0,
    1,    1,    3,    1,    4,    6,    4,    8,    1,    5,
};
static const short yydefred[] = {                         0,
    0,    4,    3,    0,    0,    0,    0,    0,    0,   79,
    2,    0,    1,   11,   74,    0,    0,    0,    0,    0,
    0,    0,    0,   42,   44,   64,   65,   66,   67,   68,
    0,    0,   72,    0,    0,    0,    0,    0,   21,    0,
    0,    0,    0,    0,    0,    0,    0,   47,   49,   50,
   51,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    5,   69,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,   48,   15,   18,    0,    0,   17,   41,
   23,   24,   22,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    6,    0,   73,    0,    0,
   77,    0,    0,    0,    0,    0,    0,    0,   16,   14,
    0,    9,    0,   13,    0,    0,    0,    0,    0,    0,
   60,   53,    0,    0,   52,   80,   19,   43,    0,   76,
    0,    0,    0,    0,    0,    0,    0,   10,    0,   58,
    0,    0,    0,   61,   54,   55,   78,    0,   62,    0,
    0,   59,   63,
};
static const short yydgoto[] = {                         12,
   33,   14,   15,   70,   71,   16,   87,   17,   18,   19,
   20,   21,   22,   23,   24,   25,   46,   47,   48,   49,
   50,   51,  135,  128,  129,   26,   27,   28,   29,   30,
   35,   36,
};
static const short yysindex[] = {                      -153,
 -215,    0,    0, -153, -237, -170, -170, -257, -224,    0,
    0,    0,    0,    0,    0, -260, -274, -230, -213, -182,
 -197, -265, -244,    0,    0,    0,    0,    0,    0,    0,
 -170, -227,    0, -225, -218, -235, -175, -170,    0, -236,
 -205, -229, -166, -157, -155, -189, -224,    0,    0,    0,
    0, -240, -170, -150, -237, -237, -237, -237, -237, -237,
 -237, -237, -237, -237, -237, -237, -237, -158, -165, -151,
 -149,    0,    0, -153, -170, -225, -153, -153, -170, -144,
 -254, -145, -153,    0,    0,    0, -231, -143,    0,    0,
    0,    0,    0, -230, -230, -213, -213, -213, -213, -182,
 -182, -197, -265, -169, -170,    0, -133,    0, -136, -154,
    0, -152, -121, -118, -170, -251, -258, -153,    0,    0,
 -170,    0, -134,    0, -153, -170, -120, -119, -116, -135,
    0,    0, -121, -146,    0,    0,    0,    0, -170,    0,
 -141, -107, -249, -104, -170, -113, -101,    0, -153,    0,
 -100, -153, -103,    0,    0,    0,    0, -115,    0,  -97,
 -153,    0,    0,
};
static const short yyrindex[] = {                         0,
   41,    0,    0, -102,    0,    0,    0,    0, -128,    0,
    0,    0,    0,    0,    0,   81,  121,  161,  284,  484,
  606,  686,  766,    0,    0,    0,    0,    0,    0,    0,
    0,  -99,    0, -235,    0,  -98,   41,    0,    0,    0,
    0,    0,    0,    0,    0,    0, -127,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  -96,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  201,  241,  324,  364,  404,  444,  524,
  566,  646,  726,    1,    0,    0,    0,    0,    0,  244,
    0,    0,  -94,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0, -228,    0,
    0,    0,  -95,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,
};
static const short yygindex[] = {                         0,
    2,    0,    7,    0,    0,    0,    0,    5,   25,   53,
   34,  101,  104,    0,  113,    0,    0,    0,  127,    0,
    0,    0,    0,   42,    0,    0,    0,    0,    0,    0,
    0,   93,
};
#define YYTABLESIZE 1065
static const short yytable[] = {                         42,
   13,   13,   52,   74,   53,  132,  114,   55,   54,   39,
   34,  151,   40,   41,   66,  133,    1,    2,    3,   37,
    2,    3,    4,   85,  152,   38,   74,  115,  118,   69,
    5,  134,  119,    5,  136,   57,   67,   68,   72,   57,
   12,   56,   57,    6,   76,   73,    7,    8,   77,   31,
    9,   32,   79,   86,   10,   11,   58,   59,   11,   88,
   91,   92,   93,   93,   93,   93,   93,   93,   93,   93,
   93,   93,   43,   44,   45,  108,   64,   65,  110,  111,
   20,  109,   94,   95,   78,  112,    1,    2,    3,   75,
   80,   32,    4,   60,   61,   62,   63,  100,  101,   81,
    5,   82,   83,    1,    2,    3,   89,  104,  105,    4,
  107,  122,   96,   97,   98,   99,  106,    5,  113,  137,
   22,  131,  120,  123,  121,   11,  140,  138,  116,  124,
    6,  125,  141,    7,    8,  127,  126,    9,  130,  139,
  142,   10,   11,  144,  143,  148,  145,  147,  149,  150,
  157,  154,  153,  159,  155,  156,  158,  160,  161,  162,
   25,   70,  163,   45,   46,   71,  102,   90,    7,   56,
  103,    8,   56,   84,  146,  117,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
   26,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
   27,    0,    0,   75,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
   13,    0,   13,   13,   13,   13,   13,    0,   13,   13,
   13,   13,   13,   13,   13,   13,   13,   13,   13,   13,
   13,   13,   13,   28,    0,   13,   13,    0,    0,   13,
   13,    0,   13,   13,    0,    0,    0,   13,   13,   13,
   12,    0,   12,   12,   12,    0,   12,    0,   12,   12,
   12,   12,   12,   12,   12,   12,   12,   12,   12,   12,
   12,   12,   12,   29,    0,   12,   12,    0,    0,   12,
   12,    0,   12,   12,    0,    0,    0,   12,   12,   12,
   20,    0,   20,    0,   20,    0,   20,    0,   20,    0,
   20,   20,   20,   20,   20,   20,   20,   20,   20,   20,
   20,   20,   20,   31,    0,   20,   20,    0,    0,   20,
   20,    0,   20,   20,    0,    0,    0,   20,   20,   20,
   22,    0,   22,    0,   22,    0,   22,    0,   22,    0,
   22,   22,   22,   22,   22,   22,   22,   22,   22,   22,
   22,   22,    0,   30,    0,   22,   22,    0,    0,   22,
   22,    0,   22,   22,    0,    0,    0,   22,   22,   22,
   25,    0,   25,    0,   25,    0,   25,    0,   25,    0,
   25,   25,    0,    0,   25,   25,   25,   25,   25,   25,
   25,   25,    0,   32,    0,   25,   25,    0,    0,   25,
   25,    0,   25,   25,    0,    0,    0,   25,   25,   25,
   26,    0,   26,    0,   26,    0,   26,    0,   26,    0,
   26,   26,    0,    0,   26,   26,   26,   26,   26,   26,
   26,   26,    0,   33,    0,   26,   26,    0,    0,   26,
   26,    0,   26,   26,    0,    0,    0,   26,   26,   26,
   27,    0,   27,   75,   27,   75,   27,   75,   27,    0,
   27,   27,    0,    0,   27,   27,   27,   27,   27,   27,
   27,   27,    0,   34,    0,   27,   27,    0,    0,   27,
   27,    0,   27,   27,    0,   75,   75,   27,   27,   27,
   75,   75,   75,   28,    0,   28,    0,   28,    0,   28,
    0,   28,    0,    0,    0,    0,    0,   28,   28,   28,
   28,   28,   28,   28,   28,   35,    0,    0,   28,   28,
    0,    0,   28,   28,    0,   28,   28,    0,    0,    0,
   28,   28,   28,   29,    0,   29,    0,   29,    0,   29,
    0,   29,    0,    0,    0,    0,    0,   29,   29,   29,
   29,   29,   29,   29,   29,   36,    0,    0,   29,   29,
    0,    0,   29,   29,    0,   29,   29,    0,    0,    0,
   29,   29,   29,   31,    0,   31,    0,   31,    0,   31,
    0,   31,    0,    0,    0,    0,    0,   31,   31,   31,
   31,   31,   31,   31,   31,   37,    0,    0,   31,   31,
    0,    0,   31,   31,    0,   31,   31,    0,    0,    0,
   31,   31,   31,   30,    0,   30,    0,   30,    0,   30,
    0,   30,    0,    0,    0,    0,    0,   30,   30,   30,
   30,   30,   30,   30,   30,   38,    0,    0,   30,   30,
    0,    0,   30,   30,    0,   30,   30,    0,    0,    0,
   30,   30,   30,   32,    0,   32,    0,   32,    0,   32,
    0,   32,    0,    0,    0,    0,    0,   32,   32,   32,
   32,   32,   32,   32,   32,   39,    0,    0,   32,   32,
    0,    0,   32,   32,    0,   32,   32,    0,    0,    0,
   32,   32,   32,   33,    0,   33,    0,   33,    0,   33,
    0,   33,    0,    0,    0,    0,    0,   33,   33,    0,
    0,    0,    0,   33,   33,   40,    0,    0,   33,   33,
    0,    0,   33,   33,    0,   33,   33,    0,    0,    0,
   33,   33,   33,   34,    0,   34,    0,   34,    0,   34,
    0,   34,    0,    0,    0,    0,    0,   34,   34,    0,
    0,    0,    0,   34,   34,    0,    0,    0,   34,   34,
    0,    0,   34,   34,    0,   34,   34,    0,    0,    0,
   34,   34,   34,    0,    0,   35,    0,   35,    0,   35,
    0,   35,    0,   35,    0,    0,    0,    0,    0,   35,
   35,    0,    0,    0,    0,   35,   35,    0,    0,    0,
   35,   35,    0,    0,   35,   35,    0,   35,   35,    0,
    0,    0,   35,   35,   35,   36,    0,   36,    0,   36,
    0,   36,    0,   36,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,   36,   36,    0,    0,    0,
   36,   36,    0,    0,   36,   36,    0,   36,   36,    0,
    0,    0,   36,   36,   36,   37,    0,   37,    0,   37,
    0,   37,    0,   37,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,   37,   37,    0,    0,    0,
   37,   37,    0,    0,   37,   37,    0,   37,   37,    0,
    0,    0,   37,   37,   37,   38,    0,   38,    0,   38,
    0,   38,    0,   38,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,   38,    0,    0,    0,
   38,   38,    0,    0,   38,   38,    0,   38,   38,    0,
    0,    0,   38,   38,   38,   39,    0,   39,    0,   39,
    0,   39,    0,   39,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,   39,    0,    0,    0,
   39,   39,    0,    0,   39,   39,    0,   39,   39,    0,
    0,    0,   39,   39,   39,   40,    0,   40,    0,   40,
    0,   40,    0,   40,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
   40,   40,    0,    0,   40,   40,    0,   40,   40,    0,
    0,    0,   40,   40,   40,
};
static const short yycheck[] = {                        257,
    0,    0,  263,  262,  265,  257,  261,  282,  269,    5,
    4,  261,    6,    7,  280,  267,  257,  258,  259,  257,
  258,  259,  263,  264,  274,  263,  262,  282,  260,  257,
  271,  283,  264,  271,  293,  264,  281,   31,  264,  268,
    0,  272,  273,  284,   38,  264,  287,  288,  285,  265,
  291,  267,  282,   52,  295,  296,  270,  271,  296,   53,
   56,   57,   58,   59,   60,   61,   62,   63,   64,   65,
   66,   67,  297,  298,  299,   74,  274,  275,   77,   78,
    0,   75,   58,   59,  290,   79,  257,  258,  259,  265,
  257,  267,  263,  276,  277,  278,  279,   64,   65,  257,
  271,  257,  292,  257,  258,  259,  257,  266,  274,  263,
  260,  105,   60,   61,   62,   63,  268,  271,  263,  118,
    0,  115,  266,  257,  294,  296,  125,  121,  274,  266,
  284,  286,  126,  287,  288,  257,  289,  291,  257,  274,
  261,  295,  296,  260,  264,  139,  282,  294,  290,  257,
  149,  145,  257,  152,  268,  257,  257,  261,  274,  257,
    0,  264,  161,  292,  292,  264,   66,   55,  268,  264,
   67,  268,  268,   47,  133,   83,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
    0,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
    0,   -1,   -1,    0,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  260,   -1,  262,  263,  264,  265,  266,   -1,  268,  269,
  270,  271,  272,  273,  274,  275,  276,  277,  278,  279,
  280,  281,  282,    0,   -1,  285,  286,   -1,   -1,  289,
  290,   -1,  292,  293,   -1,   -1,   -1,  297,  298,  299,
  260,   -1,  262,  263,  264,   -1,  266,   -1,  268,  269,
  270,  271,  272,  273,  274,  275,  276,  277,  278,  279,
  280,  281,  282,    0,   -1,  285,  286,   -1,   -1,  289,
  290,   -1,  292,  293,   -1,   -1,   -1,  297,  298,  299,
  260,   -1,  262,   -1,  264,   -1,  266,   -1,  268,   -1,
  270,  271,  272,  273,  274,  275,  276,  277,  278,  279,
  280,  281,  282,    0,   -1,  285,  286,   -1,   -1,  289,
  290,   -1,  292,  293,   -1,   -1,   -1,  297,  298,  299,
  260,   -1,  262,   -1,  264,   -1,  266,   -1,  268,   -1,
  270,  271,  272,  273,  274,  275,  276,  277,  278,  279,
  280,  281,   -1,    0,   -1,  285,  286,   -1,   -1,  289,
  290,   -1,  292,  293,   -1,   -1,   -1,  297,  298,  299,
  260,   -1,  262,   -1,  264,   -1,  266,   -1,  268,   -1,
  270,  271,   -1,   -1,  274,  275,  276,  277,  278,  279,
  280,  281,   -1,    0,   -1,  285,  286,   -1,   -1,  289,
  290,   -1,  292,  293,   -1,   -1,   -1,  297,  298,  299,
  260,   -1,  262,   -1,  264,   -1,  266,   -1,  268,   -1,
  270,  271,   -1,   -1,  274,  275,  276,  277,  278,  279,
  280,  281,   -1,    0,   -1,  285,  286,   -1,   -1,  289,
  290,   -1,  292,  293,   -1,   -1,   -1,  297,  298,  299,
  260,   -1,  262,  260,  264,  262,  266,  264,  268,   -1,
  270,  271,   -1,   -1,  274,  275,  276,  277,  278,  279,
  280,  281,   -1,    0,   -1,  285,  286,   -1,   -1,  289,
  290,   -1,  292,  293,   -1,  292,  293,  297,  298,  299,
  297,  298,  299,  260,   -1,  262,   -1,  264,   -1,  266,
   -1,  268,   -1,   -1,   -1,   -1,   -1,  274,  275,  276,
  277,  278,  279,  280,  281,    0,   -1,   -1,  285,  286,
   -1,   -1,  289,  290,   -1,  292,  293,   -1,   -1,   -1,
  297,  298,  299,  260,   -1,  262,   -1,  264,   -1,  266,
   -1,  268,   -1,   -1,   -1,   -1,   -1,  274,  275,  276,
  277,  278,  279,  280,  281,    0,   -1,   -1,  285,  286,
   -1,   -1,  289,  290,   -1,  292,  293,   -1,   -1,   -1,
  297,  298,  299,  260,   -1,  262,   -1,  264,   -1,  266,
   -1,  268,   -1,   -1,   -1,   -1,   -1,  274,  275,  276,
  277,  278,  279,  280,  281,    0,   -1,   -1,  285,  286,
   -1,   -1,  289,  290,   -1,  292,  293,   -1,   -1,   -1,
  297,  298,  299,  260,   -1,  262,   -1,  264,   -1,  266,
   -1,  268,   -1,   -1,   -1,   -1,   -1,  274,  275,  276,
  277,  278,  279,  280,  281,    0,   -1,   -1,  285,  286,
   -1,   -1,  289,  290,   -1,  292,  293,   -1,   -1,   -1,
  297,  298,  299,  260,   -1,  262,   -1,  264,   -1,  266,
   -1,  268,   -1,   -1,   -1,   -1,   -1,  274,  275,  276,
  277,  278,  279,  280,  281,    0,   -1,   -1,  285,  286,
   -1,   -1,  289,  290,   -1,  292,  293,   -1,   -1,   -1,
  297,  298,  299,  260,   -1,  262,   -1,  264,   -1,  266,
   -1,  268,   -1,   -1,   -1,   -1,   -1,  274,  275,   -1,
   -1,   -1,   -1,  280,  281,    0,   -1,   -1,  285,  286,
   -1,   -1,  289,  290,   -1,  292,  293,   -1,   -1,   -1,
  297,  298,  299,  260,   -1,  262,   -1,  264,   -1,  266,
   -1,  268,   -1,   -1,   -1,   -1,   -1,  274,  275,   -1,
   -1,   -1,   -1,  280,  281,   -1,   -1,   -1,  285,  286,
   -1,   -1,  289,  290,   -1,  292,  293,   -1,   -1,   -1,
  297,  298,  299,   -1,   -1,  260,   -1,  262,   -1,  264,
   -1,  266,   -1,  268,   -1,   -1,   -1,   -1,   -1,  274,
  275,   -1,   -1,   -1,   -1,  280,  281,   -1,   -1,   -1,
  285,  286,   -1,   -1,  289,  290,   -1,  292,  293,   -1,
   -1,   -1,  297,  298,  299,  260,   -1,  262,   -1,  264,
   -1,  266,   -1,  268,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  280,  281,   -1,   -1,   -1,
  285,  286,   -1,   -1,  289,  290,   -1,  292,  293,   -1,
   -1,   -1,  297,  298,  299,  260,   -1,  262,   -1,  264,
   -1,  266,   -1,  268,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  280,  281,   -1,   -1,   -1,
  285,  286,   -1,   -1,  289,  290,   -1,  292,  293,   -1,
   -1,   -1,  297,  298,  299,  260,   -1,  262,   -1,  264,
   -1,  266,   -1,  268,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  281,   -1,   -1,   -1,
  285,  286,   -1,   -1,  289,  290,   -1,  292,  293,   -1,
   -1,   -1,  297,  298,  299,  260,   -1,  262,   -1,  264,
   -1,  266,   -1,  268,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  281,   -1,   -1,   -1,
  285,  286,   -1,   -1,  289,  290,   -1,  292,  293,   -1,
   -1,   -1,  297,  298,  299,  260,   -1,  262,   -1,  264,
   -1,  266,   -1,  268,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
  285,  286,   -1,   -1,  289,  290,   -1,  292,  293,   -1,
   -1,   -1,  297,  298,  299,
};
#define YYFINAL 12
#ifndef YYDEBUG
#define YYDEBUG 0
#endif
#define YYMAXTOKEN 300
#if YYDEBUG
static const char *yyname[] = {

"end-of-file",0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,"ID","STRING","INT","COMMA",
"COLON","SEMICOLON","LPAREN","RPAREN","LBRACK","RBRACK","LBRACE","RBRACE","DOT",
"PLUS","MINUS","TIMES","DIVIDE","EQ","NEQ","LT","LE","GT","GE","AND","OR",
"ASSIGN","ARRAY","IF","THEN","ELSE","WHILE","FOR","TO","DO","LET","IN","END",
"OF","BREAK","NIL","FUNCTION","VAR","TYPE","UMINUS",
};
static const char *yyrule[] = {
"$accept : program",
"program : statement",
"primary_expression : NIL",
"primary_expression : INT",
"primary_expression : STRING",
"primary_expression : LPAREN expression RPAREN",
"primary_expression : ID LBRACE type_fields_initializer RBRACE",
"type_fields_initializer :",
"type_fields_initializer : type_fields_initializer_list",
"type_fields_initializer_list : ID EQ expression",
"type_fields_initializer_list : type_fields_initializer_list COMMA ID EQ expression",
"postfix_expression : primary_expression",
"postfix_expression : ID",
"postfix_expression : ID LBRACK expression RBRACK",
"postfix_expression : postfix_expression LBRACK expression RBRACK",
"postfix_expression : postfix_expression LPAREN RPAREN",
"postfix_expression : postfix_expression LPAREN argument_expression_list RPAREN",
"postfix_expression : postfix_expression DOT ID",
"argument_expression_list : statement",
"argument_expression_list : argument_expression_list COMMA statement",
"unary_expression : postfix_expression",
"unary_expression : MINUS unary_expression",
"multiplicative_expression : unary_expression",
"multiplicative_expression : multiplicative_expression TIMES unary_expression",
"multiplicative_expression : multiplicative_expression DIVIDE unary_expression",
"additive_expression : multiplicative_expression",
"additive_expression : additive_expression PLUS multiplicative_expression",
"additive_expression : additive_expression MINUS multiplicative_expression",
"relational_expression : additive_expression",
"relational_expression : relational_expression LT additive_expression",
"relational_expression : relational_expression GT additive_expression",
"relational_expression : relational_expression LE additive_expression",
"relational_expression : relational_expression GE additive_expression",
"equality_expression : relational_expression",
"equality_expression : equality_expression EQ relational_expression",
"equality_expression : equality_expression NEQ relational_expression",
"logical_and_expression : equality_expression",
"logical_and_expression : logical_and_expression AND equality_expression",
"logical_or_expression : logical_and_expression",
"logical_or_expression : logical_or_expression OR logical_and_expression",
"assignment_expression : logical_or_expression",
"assignment_expression : unary_expression ASSIGN assignment_expression",
"expression : assignment_expression",
"expression : ID LBRACK expression RBRACK OF expression",
"expression : compound_statement",
"declaration :",
"declaration : init_declarator_list",
"init_declarator_list : init_declarator",
"init_declarator_list : init_declarator_list init_declarator",
"init_declarator : type_declarator",
"init_declarator : var_declarator",
"init_declarator : function_declarator",
"type_declarator : TYPE ID EQ type_definition",
"type_definition : ID",
"type_definition : LBRACE type_fields RBRACE",
"type_definition : ARRAY OF ID",
"type_fields :",
"type_fields : init_type_fields_list",
"init_type_fields_list : ID COLON ID",
"init_type_fields_list : init_type_fields_list COMMA ID COLON ID",
"var_declarator : VAR ID ASSIGN expression",
"var_declarator : VAR ID COLON ID ASSIGN expression",
"function_declarator : FUNCTION ID LPAREN type_fields RPAREN EQ statement",
"function_declarator : FUNCTION ID LPAREN type_fields RPAREN COLON ID EQ statement",
"statement : expression_statement",
"statement : selection_statement",
"statement : iteration_statement",
"statement : jump_statement",
"statement : let_statement",
"compound_statement : LPAREN init_statement_list RPAREN",
"init_statement_list :",
"init_statement_list : statement_list",
"statement_list : statement",
"statement_list : statement_list SEMICOLON statement",
"expression_statement : expression",
"selection_statement : IF expression THEN statement",
"selection_statement : IF expression THEN statement ELSE statement",
"iteration_statement : WHILE expression DO statement",
"iteration_statement : FOR ID ASSIGN expression TO expression DO statement",
"jump_statement : BREAK",
"let_statement : LET declaration IN statement_list END",

};
#endif

int      yydebug;
int      yynerrs;

int      yyerrflag;
int      yychar;
YYSTYPE  yyval;
YYSTYPE  yylval;

/* define the initial stack-sizes */
#ifdef YYSTACKSIZE
#undef YYMAXDEPTH
#define YYMAXDEPTH  YYSTACKSIZE
#else
#ifdef YYMAXDEPTH
#define YYSTACKSIZE YYMAXDEPTH
#else
#define YYSTACKSIZE 10000
#define YYMAXDEPTH  500
#endif
#endif

#define YYINITSTACKSIZE 500

typedef struct {
    unsigned stacksize;
    short    *s_base;
    short    *s_mark;
    short    *s_last;
    YYSTYPE  *l_base;
    YYSTYPE  *l_mark;
} YYSTACKDATA;
/* variables for the parser stack */
static YYSTACKDATA yystack;

#if YYDEBUG
#include <stdio.h>		/* needed for printf */
#endif

#include <stdlib.h>	/* needed for malloc, etc */
#include <string.h>	/* needed for memset */

/* allocate initial stack or double stack size, up to YYMAXDEPTH */
static int yygrowstack(YYSTACKDATA *data)
{
    int i;
    unsigned newsize;
    short *newss;
    YYSTYPE *newvs;

    if ((newsize = data->stacksize) == 0)
        newsize = YYINITSTACKSIZE;
    else if (newsize >= YYMAXDEPTH)
        return -1;
    else if ((newsize *= 2) > YYMAXDEPTH)
        newsize = YYMAXDEPTH;

    i = (int) (data->s_mark - data->s_base);
    newss = (short *)realloc(data->s_base, newsize * sizeof(*newss));
    if (newss == 0)
        return -1;

    data->s_base = newss;
    data->s_mark = newss + i;

    newvs = (YYSTYPE *)realloc(data->l_base, newsize * sizeof(*newvs));
    if (newvs == 0)
        return -1;

    data->l_base = newvs;
    data->l_mark = newvs + i;

    data->stacksize = newsize;
    data->s_last = data->s_base + newsize - 1;
    return 0;
}

#if YYPURE || defined(YY_NO_LEAKS)
static void yyfreestack(YYSTACKDATA *data)
{
    free(data->s_base);
    free(data->l_base);
    memset(data, 0, sizeof(*data));
}
#else
#define yyfreestack(data) /* nothing */
#endif

#define YYABORT  goto yyabort
#define YYREJECT goto yyabort
#define YYACCEPT goto yyaccept
#define YYERROR  goto yyerrlab

int
YYPARSE_DECL()
{
    int yym, yyn, yystate;
#if YYDEBUG
    const char *yys;

    if ((yys = getenv("YYDEBUG")) != 0)
    {
        yyn = *yys;
        if (yyn >= '0' && yyn <= '9')
            yydebug = yyn - '0';
    }
#endif

    yynerrs = 0;
    yyerrflag = 0;
    yychar = YYEMPTY;
    yystate = 0;

#if YYPURE
    memset(&yystack, 0, sizeof(yystack));
#endif

    if (yystack.s_base == NULL && yygrowstack(&yystack)) goto yyoverflow;
    yystack.s_mark = yystack.s_base;
    yystack.l_mark = yystack.l_base;
    yystate = 0;
    *yystack.s_mark = 0;

yyloop:
    if ((yyn = yydefred[yystate]) != 0) goto yyreduce;
    if (yychar < 0)
    {
        if ((yychar = YYLEX) < 0) yychar = 0;
#if YYDEBUG
        if (yydebug)
        {
            yys = 0;
            if (yychar <= YYMAXTOKEN) yys = yyname[yychar];
            if (!yys) yys = "illegal-symbol";
            printf("%sdebug: state %d, reading %d (%s)\n",
                    YYPREFIX, yystate, yychar, yys);
        }
#endif
    }
    if ((yyn = yysindex[yystate]) && (yyn += yychar) >= 0 &&
            yyn <= YYTABLESIZE && yycheck[yyn] == yychar)
    {
#if YYDEBUG
        if (yydebug)
            printf("%sdebug: state %d, shifting to state %d\n",
                    YYPREFIX, yystate, yytable[yyn]);
#endif
        if (yystack.s_mark >= yystack.s_last && yygrowstack(&yystack))
        {
            goto yyoverflow;
        }
        yystate = yytable[yyn];
        *++yystack.s_mark = yytable[yyn];
        *++yystack.l_mark = yylval;
        yychar = YYEMPTY;
        if (yyerrflag > 0)  --yyerrflag;
        goto yyloop;
    }
    if ((yyn = yyrindex[yystate]) && (yyn += yychar) >= 0 &&
            yyn <= YYTABLESIZE && yycheck[yyn] == yychar)
    {
        yyn = yytable[yyn];
        goto yyreduce;
    }
    if (yyerrflag) goto yyinrecovery;

    yyerror("syntax error");

    goto yyerrlab;

yyerrlab:
    ++yynerrs;

yyinrecovery:
    if (yyerrflag < 3)
    {
        yyerrflag = 3;
        for (;;)
        {
            if ((yyn = yysindex[*yystack.s_mark]) && (yyn += YYERRCODE) >= 0 &&
                    yyn <= YYTABLESIZE && yycheck[yyn] == YYERRCODE)
            {
#if YYDEBUG
                if (yydebug)
                    printf("%sdebug: state %d, error recovery shifting\
 to state %d\n", YYPREFIX, *yystack.s_mark, yytable[yyn]);
#endif
                if (yystack.s_mark >= yystack.s_last && yygrowstack(&yystack))
                {
                    goto yyoverflow;
                }
                yystate = yytable[yyn];
                *++yystack.s_mark = yytable[yyn];
                *++yystack.l_mark = yylval;
                goto yyloop;
            }
            else
            {
#if YYDEBUG
                if (yydebug)
                    printf("%sdebug: error recovery discarding state %d\n",
                            YYPREFIX, *yystack.s_mark);
#endif
                if (yystack.s_mark <= yystack.s_base) goto yyabort;
                --yystack.s_mark;
                --yystack.l_mark;
            }
        }
    }
    else
    {
        if (yychar == 0) goto yyabort;
#if YYDEBUG
        if (yydebug)
        {
            yys = 0;
            if (yychar <= YYMAXTOKEN) yys = yyname[yychar];
            if (!yys) yys = "illegal-symbol";
            printf("%sdebug: state %d, error recovery discards token %d (%s)\n",
                    YYPREFIX, yystate, yychar, yys);
        }
#endif
        yychar = YYEMPTY;
        goto yyloop;
    }

yyreduce:
#if YYDEBUG
    if (yydebug)
        printf("%sdebug: state %d, reducing by rule %d (%s)\n",
                YYPREFIX, yystate, yyn, yyrule[yyn]);
#endif
    yym = yylen[yyn];
    if (yym)
        yyval = yystack.l_mark[1-yym];
    else
        memset(&yyval, 0, sizeof yyval);
    switch (yyn)
    {
    }
    yystack.s_mark -= yym;
    yystate = *yystack.s_mark;
    yystack.l_mark -= yym;
    yym = yylhs[yyn];
    if (yystate == 0 && yym == 0)
    {
#if YYDEBUG
        if (yydebug)
            printf("%sdebug: after reduction, shifting from state 0 to\
 state %d\n", YYPREFIX, YYFINAL);
#endif
        yystate = YYFINAL;
        *++yystack.s_mark = YYFINAL;
        *++yystack.l_mark = yyval;
        if (yychar < 0)
        {
            if ((yychar = YYLEX) < 0) yychar = 0;
#if YYDEBUG
            if (yydebug)
            {
                yys = 0;
                if (yychar <= YYMAXTOKEN) yys = yyname[yychar];
                if (!yys) yys = "illegal-symbol";
                printf("%sdebug: state %d, reading %d (%s)\n",
                        YYPREFIX, YYFINAL, yychar, yys);
            }
#endif
        }
        if (yychar == 0) goto yyaccept;
        goto yyloop;
    }
    if ((yyn = yygindex[yym]) && (yyn += yystate) >= 0 &&
            yyn <= YYTABLESIZE && yycheck[yyn] == yystate)
        yystate = yytable[yyn];
    else
        yystate = yydgoto[yym];
#if YYDEBUG
    if (yydebug)
        printf("%sdebug: after reduction, shifting from state %d \
to state %d\n", YYPREFIX, *yystack.s_mark, yystate);
#endif
    if (yystack.s_mark >= yystack.s_last && yygrowstack(&yystack))
    {
        goto yyoverflow;
    }
    *++yystack.s_mark = (short) yystate;
    *++yystack.l_mark = yyval;
    goto yyloop;

yyoverflow:
    yyerror("yacc stack overflow");

yyabort:
    yyfreestack(&yystack);
    return (1);

yyaccept:
    yyfreestack(&yystack);
    return (0);
}
