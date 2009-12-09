%{
%}

%union {
    int number;
    int boolean;
    int type;
    char* identifier;
    char* string;
}

%token T_TERMINATOR T_SEPARATOR
%token T_OP_ASSIGN T_OP_BINARY
%token T_KEY_READ T_KEY_PRINT
%token <type> T_TYPE 
%token <boolean> T_BOOLEAN 
%token <identifier> T_IDENTIFIER 
%token <string> T_STRING 
%token <number> T_NUMBER
%%

program: statement_list;

statement_list: statement statement_list
                |
                statement
                ;

statement: statement_type T_TERMINATOR;

statement_type: declaration
                | assignment
                | console_out
                | console_in
                ;

declaration: T_TYPE T_IDENTIFIER T_OP_ASSIGN expression
             | 
             T_TYPE T_IDENTIFIER
             ;

assignment: T_IDENTIFIER T_OP_ASSIGN expression;

console_out: T_KEY_PRINT expression_list;

console_in: T_KEY_READ identifier_list;

expression_list: expression T_SEPARATOR expression_list
                | expression
                ;

identifier_list: T_IDENTIFIER T_SEPARATOR identifier_list
                | T_IDENTIFIER
                ;

expression: factor T_OP_BINARY expression
            | factor
            ;

factor: T_NUMBER 
        | T_BOOLEAN
        | T_STRING
        | T_IDENTIFIER
        ;

%%
