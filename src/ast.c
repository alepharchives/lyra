#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <gc.h>
#include <ast.h>

struct _Ast {
    AstType type;

    union {
        char* identifier;
        char* string;
        int boolean;
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
            int type;
            char* key;
            Ast value;
        } stmt_init;

        struct {
            int type;
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

Ast ast_explist_new(Ast head, Ast tail)
{
    head->next = tail;
    return head;
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
