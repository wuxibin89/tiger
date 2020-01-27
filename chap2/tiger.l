%{
#include <string.h>
#include "util.h"
#include "tokens.h"
#include "errormsg.h"

/* max lenght of string literal */
#define MAX_STR_CONST 2048
char string_buf[MAX_STR_CONST];
char *string_buf_ptr;

int charPos=1;

void adjust(void)
{
 EM_tokPos=charPos;
 charPos+=yyleng;
}

%}

%option noyywrap
%x comment
%x string

%%
 /* reserved words */
type        {adjust(); return TYPE;}
var         {adjust(); return VAR;}
function    {adjust(); return FUNCTION;}
array       {adjust(); return ARRAY;}
of          {adjust(); return OF;}
nil         {adjust(); return NIL;}
if          {adjust(); return IF;}
then        {adjust(); return THEN;}
else        {adjust(); return ELSE;}
for  	    {adjust(); return FOR;}
to          {adjust(); return TO;}
while       {adjust(); return WHILE;}
do          {adjust(); return DO;}
break       {adjust(); return BREAK;}
let         {adjust(); return LET;}
in          {adjust(); return IN;}
end         {adjust(); return END;}

 /* operators */
","	        {adjust(); return COMMA;}
":"         {adjust(); return COLON;}
";"         {adjust(); return SEMICOLON;}
"("         {adjust(); return LPAREN;}
")"         {adjust(); return RPAREN;}
"["         {adjust(); return LBRACK;}
"]"         {adjust(); return RBRACK;}
"{"         {adjust(); return LBRACE;}
"}"         {adjust(); return RBRACE;}
"."         {adjust(); return DOT;}
"+"         {adjust(); return PLUS;}
"-"         {adjust(); return MINUS;}
"*"         {adjust(); return TIMES;}
"/"         {adjust(); return DIVIDE;}
"="         {adjust(); return EQ;}
"<>"        {adjust(); return NEQ;}
"<"         {adjust(); return LT;}
"<="        {adjust(); return LE;}
">"         {adjust(); return GT;}
">="        {adjust(); return GE;}
"&"         {adjust(); return AND;}
"|"         {adjust(); return OR;}
":="        {adjust(); return ASSIGN;}

 /* whitespace */
[ \t]+	    {adjust();}
\n	        {adjust(); EM_newline();}

 /* comment */
"/*"        {adjust(); BEGIN(comment);}
<comment>{
[^*]*       {adjust();} /* eat anything that's not a '*' */
"*"[^*/]*   {adjust();} /* eat up '*'s not followed by '/'s */
"*/"        {adjust(); BEGIN(INITIAL);}
}

 /* integeral literal */
[0-9]+	    {adjust(); yylval.ival = atoi(yytext); return INT;}

 /* string literal */
\"          {adjust(); string_buf_ptr = string_buf; BEGIN(string);}
<string>{
\"          {
    adjust(); 
    *string_buf_ptr = '\0'; yylval.sval = String(string_buf);
    BEGIN(INITIAL); return STRING;
}

\n          {adjust(); EM_error(EM_tokPos, "unterminated string literal");}

\\[0-7]{1,3} {
    adjust();
    int result; 
    (void) sscanf(yytext + 1, "%o", &result);
    if (result > 0xff) {
        EM_error(EM_tokPos, "bad escape");
    }
    *string_buf_ptr++ = result;
}

\\[0-9]+    {adjust(); EM_error(EM_tokPos, "bad escape");}

\\n         {adjust(); *string_buf_ptr++ = '\n';}
\\t         {adjust(); *string_buf_ptr++ = '\t';}
\\r         {adjust(); *string_buf_ptr++ = '\r';}
\\b         {adjust(); *string_buf_ptr++ = '\b';}
\\f         {adjust(); *string_buf_ptr++ = '\f';}
\\(.|\n)    {adjust(); *string_buf_ptr++ = yytext[1];}

[^\\\n\"]+  {
    adjust();
    memcpy(string_buf_ptr, yytext, yyleng);
    string_buf_ptr += yyleng;
}
}

 /* identifier */
[a-zA-Z][a-zA-Z0-9_]*   {adjust(); yylval.sval = NString(yytext, yyleng); return ID; }

.	        {adjust(); EM_error(EM_tokPos,"illegal token");}
