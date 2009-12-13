%{
    #include <stdio.h>
    #include <stdlib.h>
    #include <string.h>
    #include <ast.h>
    #include "../src/y.tab.h"

    #define NEW_POSITION() CURRENT_COLUMN += strnlen(yytext, 255)
    #define NEW_LINE() CURRENT_LINE += 1; CURRENT_COLUMN = 0

    int CURRENT_LINE = 1;
    int CURRENT_COLUMN = 0;
%}
%%
;                        NEW_POSITION(); return T_TERMINATOR;
,                        NEW_POSITION(); return T_SEPARATOR;
=                        NEW_POSITION(); return T_OP_ASSIGN;
\+|\-|\*|\/|\%|\~        NEW_POSITION(); yylval.binop= yytext[0]; return T_OP_BINARY;
stdout                   NEW_POSITION(); return T_KEY_PRINT;
stdin                    NEW_POSITION(); return T_KEY_READ;
number                   NEW_POSITION(); yylval.type=L_NUMBER;return T_TYPE;
string                   NEW_POSITION(); yylval.type=L_STRING;return T_TYPE;
boolean                  NEW_POSITION(); yylval.type=L_BOOLEAN;return T_TYPE;
true                     NEW_POSITION(); yylval.boolean=L_TRUE;return T_BOOLEAN;
false                    NEW_POSITION(); yylval.boolean=L_FALSE;return T_BOOLEAN;
[a-zA-Z_][a-zA-Z_0-9]*   NEW_POSITION(); yylval.identifier=strdup(yytext);return T_IDENTIFIER;
\"([^\"]|\\\")*\"        NEW_POSITION(); yylval.string=strdup(yytext);return T_STRING;
[0-9]+                   NEW_POSITION(); yylval.number=atoi(yytext);return T_NUMBER;
\n                       NEW_LINE();
[ \t]                    NEW_POSITION();
.                        NEW_POSITION(); return yytext[0];
%%
