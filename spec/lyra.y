%{
    #include <stdio.h>
    #include <stdlib.h>
    #include <ast.h>
    #include "../src/y.tab.h"

    Ast rootAst;

    void yyerror(const char *str)
    {
        fprintf(stderr, "%s at column %d, line %d\n", str, CURRENT_COLUMN, CURRENT_LINE);
    }

    int yywrap()
    {
        return 1;
    }

%}

%error-verbose

%union {
    int number;
    LyraBoolean boolean;
    LyraType type;
    int binop;
    char* identifier;
    char* string;
    void* ast;
}

%token T_TERMINATOR T_SEPARATOR
%token T_OP_ASSIGN
%token T_KEY_READ T_KEY_PRINT
%token <type> T_TYPE 
%token <boolean> T_BOOLEAN 
%token <identifier> T_IDENTIFIER 
%token <string> T_STRING 
%token <number> T_NUMBER
%token <binop> T_OP_BINARY

%%

program: statement_list {
            rootAst = $<ast>1;
        };

statement_list: statement statement_list {
                    $<ast>$ = ast_next_set($<ast>1, $<ast>2);
                }
                |
                statement { $<ast>$ = $<ast>1; }
                ;

statement: statement_type T_TERMINATOR { $<ast>$ = $<ast>1; };

statement_type: declaration { $<ast>$ = $<ast>1; }
                | initialization { $<ast>$ = $<ast>1; }
                | assignment { $<ast>$ = $<ast>1; }
                | console_out { $<ast>$ = $<ast>1; }
                | console_in { $<ast>$ = $<ast>1; }
                ;

declaration: T_TYPE identifier_list {
                 Ast type = ast_lyratype_new($<type>1);
                 $<ast>$ = ast_declare_new(type, $<ast>2);
             }
             ;

initialization: T_TYPE T_IDENTIFIER T_OP_ASSIGN expression {
                 Ast type = ast_lyratype_new($<type>1);
                 Ast id = ast_identifier_new($<identifier>2);
                 $<ast>$ = ast_init_new(type, id, $<ast>4);
             }
             ;

assignment: T_IDENTIFIER T_OP_ASSIGN expression {
                Ast id = ast_identifier_new($<identifier>1);
                $<ast>$ = ast_assign_new(id, $<ast>3);
            };

console_out: T_KEY_PRINT expression_list {
                 $<ast>$ = ast_print_new($<ast>2);
             };

console_in: T_KEY_READ identifier_list { 
                $<ast>$ = ast_read_new($<ast>2); 
            };

expression_list: expression T_SEPARATOR expression_list {
                     $<ast>$ = ast_next_set($<ast>1, $<ast>3);
                 }
                | expression {
                    $<ast>$ = $<ast>1;
                }
                ;

expression: factor T_OP_BINARY expression {
                Ast operator = ast_operator_new($<binop>2);
                $<ast>$ = ast_binaryop_new(operator, $<ast>1, $<ast>3);
            }
            | factor { $<ast>$ = $<ast>1; }
            ;

identifier_list: T_IDENTIFIER T_SEPARATOR identifier_list {
                     Ast id = ast_identifier_new($<identifier>1);
                     $<ast>$ = ast_next_set(id, $<ast>3);
                 }
                | T_IDENTIFIER { 
                    $<ast>$ = ast_identifier_new($<identifier>1);
                }
                ;


factor: T_NUMBER { $<ast>$ = ast_number_new($<number>1); }
        | T_BOOLEAN { $<ast>$ = ast_boolean_new($<boolean>1); }
        | T_STRING { $<ast>$ = ast_string_new($<string>1); }
        | T_IDENTIFIER { $<ast>$ = ast_identifier_new($<identifier>1); }
        ;
%%
