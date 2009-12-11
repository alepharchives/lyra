#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <gc.h>
#include <ast.h>

static void ast_indent(int);
static void ast_type_printf(Ast);
static void ast_value_printf(Ast,int);
static void ast_identifier_printf(Ast);
static void ast_string_printf(Ast);
static void ast_number_printf(Ast);
static void ast_boolean_printf(Ast);
static void ast_binop_printf(Ast,int);
static void ast_stmt_read_printf(Ast,int);
static void ast_stmt_print_printf(Ast,int);
static void ast_stmt_assign_printf(Ast,int);
static void ast_stmt_init_printf(Ast,int);
static void ast_stmt_declare_printf(Ast,int);

struct _Ast {
    AstType type;

    union {
        char* identifier;
        char* string;
        LyraBoolean boolean;
        int number;

        struct {
            Ast head;
            Ast tail;
        } idlist;

        struct {
            Ast head;
            Ast tail;
        } explist;

        struct {
            int operator;
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
            char* key;
            Ast value;
        } stmt_assign;

        struct {
            LyraType type;
            char* key;
            Ast value;
        } stmt_init;

        struct {
            LyraType type;
            Ast idlist;
        } stmt_declare;
    } value;

    int lineno;
    Ast next;
};


Ast ast_identifier_new(const char* s)
{
    AST_NEW(IDENTIFIER, ast);
    ast->value.identifier = strndup(s, 255);
    return ast;
}

Ast ast_string_new(const char* s)
{
    AST_NEW(STRING, ast);
    ast->value.string = strndup(s, 255);
    return ast;
}

Ast ast_boolean_new(int i)
{
    AST_NEW(BOOLEAN, ast);
    ast->value.boolean = i;
    return ast;
}

Ast ast_number_new(int i)
{
    AST_NEW(NUMBER, ast);
    ast->value.number = i;
    return ast;
}

Ast ast_idlist_new(const char* value, Ast next)
{
    Ast ast = ast_identifier_new(value);
    ast->next = next;
    return ast;
}

Ast ast_binaryop_new(int binop, Ast left, Ast right)
{
    AST_NEW(OP_BINARY, ast);
    ast->value.binop.operator = binop;
    ast->value.binop.left = left;
    ast->value.binop.right = right;
    return ast;
}

Ast ast_read_new(Ast idlist)
{
    AST_NEW(STATEMENT_READ, ast);
    ast->value.stmt_read.idlist = idlist;
    return ast;
}

Ast ast_print_new(Ast explist)
{
    AST_NEW(STATEMENT_PRINT, ast);
    ast->value.stmt_print.explist = explist;
    return ast;
}

Ast ast_assign_new(const char* key, Ast value)
{
    AST_NEW(STATEMENT_ASSIGN, ast);
    ast->value.stmt_assign.key = strndup(key, 255);
    ast->value.stmt_assign.value = value;
    return ast;
}

Ast ast_init_new(int type, const char* key, Ast value)
{
    AST_NEW(STATEMENT_INIT, ast);
    ast->value.stmt_init.type = type;
    ast->value.stmt_init.key = strndup(key, 255);
    ast->value.stmt_init.value = value;
    return ast;
}

Ast ast_declare_new(int type, Ast idlist)
{
    AST_NEW(STATEMENT_DECLARE, ast);
    ast->value.stmt_declare.type = type;
    ast->value.stmt_declare.idlist = idlist;
    return ast;
}

Ast ast_next_set(Ast ast, Ast next)
{
    ast->next = next;
    return ast;
}

void ast_printf(Ast ast, int indent)
{
    if(ast == NULL) {
        return;
    }

    ast_indent(indent);
    ast_type_printf(ast);
    ast_value_printf(ast, indent+1);
    ast_printf(ast->next, indent);
}

/* private functions */

void ast_indent(int n)
{
    int i;

    for(i=0; i < n; i++) {
        printf(" ");
    }

    printf("-");
}

void ast_type_printf(Ast ast)
{
    switch(ast->type) {
        case IDENTIFIER: printf("IDENTIFIER ");break;
        case STRING: printf("STRING ");break;
        case BOOLEAN: printf("BOOLEAN ");break;
        case NUMBER: printf("NUMBER ");break;
        case OP_BINARY: printf("OP_BINARY ");break;
        case STATEMENT_READ: printf("STMT_READ ");break;
        case STATEMENT_PRINT: printf("STMT_PRINT ");break;
        case STATEMENT_ASSIGN: printf("STMT_ASSIGN ");break;
        case STATEMENT_INIT: printf("STMT_INIT ");break;
        case STATEMENT_DECLARE: printf("STMT_DECLARE ");break;
        default: fprintf(stderr, "UNKNOWN AST TYPE\n");exit(1);
    }
}

void ast_value_printf(Ast ast, int indent)
{
    switch(ast->type) {
        case IDENTIFIER: ast_identifier_printf(ast);break;
        case STRING: ast_string_printf(ast);break;
        case NUMBER: ast_number_printf(ast);break;
        case BOOLEAN: ast_boolean_printf(ast);break;
        case OP_BINARY: ast_binop_printf(ast, indent);break;
        case STATEMENT_READ: ast_stmt_read_printf(ast, indent);break;
        case STATEMENT_PRINT: ast_stmt_print_printf(ast, indent);break;
        case STATEMENT_ASSIGN: ast_stmt_assign_printf(ast, indent);break;
        case STATEMENT_INIT: ast_stmt_init_printf(ast, indent);break;
        case STATEMENT_DECLARE: ast_stmt_declare_printf(ast, indent);break;
        default: fprintf(stderr, "UNKNOWN AST TYPE\n");exit(1);
    }
}

void ast_identifier_printf(Ast ast)
{
    printf(" [%s]\n", ast->value.identifier);
}

void ast_string_printf(Ast ast)
{
    printf(" [%s]\n", ast->value.string);
}

void ast_number_printf(Ast ast)
{
    printf(" [%d]\n", ast->value.number);
}

void ast_boolean_printf(Ast ast)
{
    if(ast->value.boolean == L_TRUE) {
        printf(" [true]\n");
    } else {
        printf(" [false]\n");
    }
}

void ast_binop_printf(Ast ast, int indent)
{
    printf(" [%c]\n", ast->value.binop.operator);
    ast_printf(ast->value.binop.left, indent);
    ast_printf(ast->value.binop.right, indent);
}

void ast_stmt_read_printf(Ast ast, int indent)
{
    printf("\n");
    ast_printf(ast->value.stmt_read.idlist, indent);
}

void ast_stmt_print_printf(Ast ast, int indent)
{
    printf("\n");
    ast_printf(ast->value.stmt_print.explist, indent);
}

void ast_stmt_assign_printf(Ast ast, int indent)
{
    printf(" [%s]\n", ast->value.stmt_assign.key);
    ast_printf(ast->value.stmt_assign.value, indent);
}

void ast_stmt_init_printf(Ast ast, int indent)
{
    const char* type;

    switch(ast->value.stmt_init.type) {
        case L_NUMBER: type = "number";break;
        case L_BOOLEAN: type = "boolean"; break;
        case L_STRING: type = "string";break;
        default: fprintf(stderr, "UNKNOWN TYPE IN DECLARATION\n");exit(1);
    }

    printf(" [%s %s]\n", type, ast->value.stmt_init.key);
    ast_printf(ast->value.stmt_init.value, indent);
}
void ast_stmt_declare_printf(Ast ast, int indent)
{
    const char* type;

    switch(ast->value.stmt_declare.type) {
        case L_NUMBER: type = "number";break;
        case L_BOOLEAN: type = "boolean"; break;
        case L_STRING: type = "string";break;
        default: fprintf(stderr, "UNKNOWN TYPE IN DECLARATION\n");exit(1);
    }

    printf(" [%s]\n", type);
    ast_printf(ast->value.stmt_declare.idlist, indent);
}
