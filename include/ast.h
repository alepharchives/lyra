#ifndef _AST_H
#define _AST_H

typedef struct _Ast* Ast;

typedef enum {
    L_INVALID,
    L_NUMBER,
    L_BOOLEAN,
    L_STRING
} LyraType;

typedef enum {
    L_FALSE,
    L_TRUE
} LyraBoolean;

typedef enum {
    IDENTIFIER,
    STRING,
    BOOLEAN,
    NUMBER,
    TYPE,
    OPERATOR,
    OP_BINARY,
    STATEMENT_READ,
    STATEMENT_PRINT,
    STATEMENT_ASSIGN,
    STATEMENT_INIT,
    STATEMENT_DECLARE
} AstType;

struct _Ast {
    AstType type;

    union {
        char* identifier;
        char* string;
        int number;
        int operator;
        LyraBoolean boolean;
        LyraType type;

        struct {
            Ast head;
            Ast tail;
        } idlist;

        struct {
            Ast head;
            Ast tail;
        } explist;

        struct {
            Ast operator;
            Ast left;
            Ast right;
        } binop;

        struct {
            Ast idlist;
        } stmt_read;

        struct {
            Ast explist;
        } stmt_print;

        struct {
            Ast key;
            Ast value;
        } stmt_assign;

        struct {
            Ast type;
            Ast key;
            Ast value;
        } stmt_init;

        struct {
            Ast type;
            Ast idlist;
        } stmt_declare;
    } value;

    int line;
    Ast next;
};



extern Ast rootAst;
extern int CURRENT_LINE;
extern int CURRENT_COLUMN;

Ast ast_identifier_new(const char*);
Ast ast_string_new(const char*);
Ast ast_boolean_new(int);
Ast ast_number_new(int);
Ast ast_lyratype_new(LyraType);
Ast ast_operator_new(int);

Ast ast_binaryop_new(Ast,Ast,Ast);
Ast ast_read_new(Ast);
Ast ast_print_new(Ast);
Ast ast_assign_new(Ast, Ast);
Ast ast_init_new(Ast,Ast,Ast);
Ast ast_declare_new(Ast,Ast);
Ast ast_next_set(Ast,Ast);

void ast_printf(Ast,int);

#endif
