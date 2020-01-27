%{
#include <math.h>
#define MAX_STR_CONST 2048
char string_buf[MAX_STR_CONST];
char *string_buf_ptr;
%}
%s expect
%x comment
%x str

%option noyywrap

%%
expect-floats        BEGIN(expect);

<expect>[0-9]+.[0-9]+      {
            printf( "found a float, = %f\n",
                    atof( yytext ) );
            }
<expect>\n           {
            /* that's the end of the line, so
             * we need another "expect-number"
             * before we'll recognize any more
             * numbers
             */
            BEGIN(INITIAL);
            }

[0-9]+      {
            printf( "found an integer, = %d\n",
                    atoi( yytext ) );
            }

"."         printf( "found a dot\n" );

"/*"           BEGIN(comment);
<comment>{
[^*]*          /* eat anything that's not a '*' */
"*"[^*/]*      /* eat up '*'s not followed by '/'s */
"*/"           BEGIN(INITIAL);
}

\"      {string_buf_ptr = string_buf; BEGIN(str);}
<str>{
\"      {BEGIN(INITIAL); *string_buf_ptr = '\0'; printf("c string: %s\n", string_buf);}
\n      {printf("error: unterminated string constant"); exit(-1);}
\\[0-7]{1,3} {
    int result; 
    (void) sscanf( yytext + 1, "%o", &result );
    if ( result > 0xff ) {
        printf("unexpected: %d\n", result);
        exit(-1);
    }
    *string_buf_ptr++ = result;
}
\\[0-9]+ {printf("bad escape sequence"); exit(-1);}
\\n  *string_buf_ptr++ = '\n';
\\t  *string_buf_ptr++ = '\t';
\\r  *string_buf_ptr++ = '\r';
\\b  *string_buf_ptr++ = '\b';
\\f  *string_buf_ptr++ = '\f';
\\(.|\n)  *string_buf_ptr++ = yytext[1];
[^\\\n\"]+        {
    memcpy(string_buf_ptr, yytext, yyleng);
    string_buf_ptr += yyleng;
}
}

[ \t\n]+          /* eat up whitespace */
%%

int main( int argc, char **argv )
{
++argv, --argc;  /* skip over program name */
if ( argc > 0 )
        yyin = fopen( argv[0], "r" );
else
        yyin = stdin;

yylex();
}