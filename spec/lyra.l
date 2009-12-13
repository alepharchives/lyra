%{
    #include <stdio.h>
    #include <stdlib.h>
    #include <string.h>
    #include <ast.h>
    #include "../src/y.tab.h"

    #define COLPOS_INC() COLPOS += strnlen(yytext, 255)
    #define NEWLINE() LINENUM += 1; COLPOS = 0

    int LINENUM = 1;
    int COLPOS = 0;
%}
%%
;                        COLPOS_INC(); return T_TERMINATOR;
,                        COLPOS_INC(); return T_SEPARATOR;
=                        COLPOS_INC(); return T_OP_ASSIGN;
\+|\-|\*|\/|\%|\~        COLPOS_INC(); yylval.binop= yytext[0]; return T_OP_BINARY;
stdout                   COLPOS_INC(); return T_KEY_PRINT;
stdin                    COLPOS_INC(); return T_KEY_READ;
number                   COLPOS_INC(); yylval.type=L_NUMBER;return T_TYPE;
string                   COLPOS_INC(); yylval.type=L_STRING;return T_TYPE;
boolean                  COLPOS_INC(); yylval.type=L_BOOLEAN;return T_TYPE;
true                     COLPOS_INC(); yylval.boolean=L_TRUE;return T_BOOLEAN;
false                    COLPOS_INC(); yylval.boolean=L_FALSE;return T_BOOLEAN;
[a-zA-Z_][a-zA-Z_0-9]*   COLPOS_INC(); yylval.identifier=strdup(yytext);return T_IDENTIFIER;
\"([^\"]|\\\")*\"        COLPOS_INC(); yylval.string=strdup(yytext);return T_STRING;
[0-9]+                   COLPOS_INC(); yylval.number=atoi(yytext);return T_NUMBER;
\n                       NEWLINE();
[ \t]                    COLPOS_INC();
.                        COLPOS_INC(); return yytext[0];
%%
