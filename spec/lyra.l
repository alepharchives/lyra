%{
    #include <stdio.h>
    #include <stdlib.h>
    #include <string.h>
    #include "../include/lyra.h"
    #include "y.tab.h"
%}
%%
;                        return T_TERMINATOR;
,                        return T_SEPARATOR;
=                        return T_OP_ASSIGN;
\+|\-|\*|\/|\%|\~        return T_OP_BINARY;
stdout                   return T_KEY_PRINT;
stdin                    return T_KEY_READ;
number                   yylval.type=C_NUMBER;return T_TYPE;
string                   yylval.type=C_STRING;return T_TYPE;
boolean                  yylval.type=C_BOOLEAN;return T_TYPE;
true                     yylval.boolean=C_TRUE;return T_BOOLEAN;
false                    yylval.boolean=C_FALSE;return T_BOOLEAN;
[a-zA-Z_][a-zA-Z_0-9]*   yylval.identifier=strdup(yytext);return T_IDENTIFIER;
\".*\"                   yylval.string=strdup(yytext);return T_STRING;
(\-|\+)?[0-9]+           yylval.number=atoi(yytext);return T_NUMBER;
[ \t\n]                  ;
.                        return (int) yytext;
%%
