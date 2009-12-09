#ifndef _AST_H
#define _AST_H

#define AST_NEW(EnumType, a) Ast a = GC_malloc(sizeof(*a)); \
                                 if(a == NULL) { \
                                     return NULL; \
                                 } \
                                 a->type = EnumType; \
                                 a->next = NULL; \
                                 a->lineno = 0; \
                                 if(1) \


typedef enum {
    IDENTIFIER,
    STRING,
    BOOLEAN,
    NUMBER,
    OP_BINARY,
    STATEMENT_READ,
    STATEMENT_PRINT,
    STATEMENT_ASSIGN,
    STATEMENT_INIT,
    STATEMENT_DECLARE
} AstType;

typedef struct _Ast* Ast;

Ast ast_identifier_new(const char*);
Ast ast_string_new(const char*);
Ast ast_boolean_new(int);
Ast ast_number_new(int);
Ast ast_idlist_new(const char*, Ast);
Ast ast_binaryop_new(int,Ast,Ast);
Ast ast_explist_new(Ast,Ast);
Ast ast_read_new(Ast);
Ast ast_print_new(Ast);
Ast ast_assign_new(const char*, Ast);
Ast ast_init_new(int,const char*,Ast);
Ast ast_declare_new(int,Ast);
Ast ast_next_set(Ast,Ast);

void ast_printf(Ast);

#endif
