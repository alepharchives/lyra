#include <stdio.h>
#include <stdlib.h>
#include <gc.h>
#include <ast.h>
#include <symtab.h>
#include <semantic.h>


static int op_binary_chk(Ast);
static int stmt_read_chk(Ast);
static int stmt_print_chk(Ast);
static int stmt_assign_chk(Ast);
static int stmt_init_chk(Ast);
static int stmt_declare_chk(Ast);

static LyraType semantic_type_identifer_get(Ast);
static LyraType semantic_type_binop_get(Ast);

int semantic_chk(Ast ast)
{
    if(ast == NULL) {
        return 1;
    }

    int status = 0;

    switch(ast->type) {
        /*case STATEMENT_READ: return stmt_read_chk(ast);
        case STATEMENT_PRINT: return stmt_print_chk(ast);
        case STATEMENT_ASSIGN: return stmt_assign_chk(ast);*/
        case STATEMENT_INIT: status = stmt_init_chk(ast);break;
        case STATEMENT_DECLARE: status = stmt_declare_chk(ast);break;
        default: fprintf(stderr, "Line %d: UNKNOWN AST STATEMENT TYPE: %d\n", ast->line, ast->type);return 0;
    }

    if(!status) {
        return 0;
    }

    return semantic_chk(ast->next);
}

int stmt_init_chk(Ast ast)
{
    Ast typeAst = ast->value.stmt_init.type;
    LyraType type = typeAst->value.type;

    Ast nameAst = ast->value.stmt_init.key;
    const char* name = nameAst->value.identifier;

    Ast valueAst = ast->value.stmt_init.value;
    LyraType valueType = semantic_type_get(valueAst);

    if(symtab_lookup(symbolTable, name) != NULL) {
        /* 
            TODO: More info with semantic error from existing 
            definition, also line number reference will be good 
         */
        fprintf(stderr, "Line %d: variable %s is already declared\n", nameAst->line, name);
        return 0;
    }


    if(type != valueType) {
        fprintf(stderr, "Line %d: variable %s has declaration and initialization of different types\n", nameAst->line, name);
        return 0;
    }

    symbolTable = symtab_define(symbolTable, name, type);

    switch(type) {
        case L_NUMBER: symbolTable = symtab_number_set(symbolTable, valueAst->value.number);break;
        case L_STRING: symbolTable = symtab_string_set(symbolTable, valueAst->value.string);break;
        case L_BOOLEAN: symbolTable = symtab_boolean_set(symbolTable, valueAst->value.boolean);break;
        default: fprintf(stderr, "Line %d: UNKNOWN LYRA TYPE: %d\n", typeAst->line, type);return 0;
    }

    return 1;
}

int stmt_declare_chk(Ast ast)
{
    LyraType type = ast->value.stmt_declare.type->value.type;

    Ast current = ast->value.stmt_declare.idlist;

    while(current != NULL) {
        const char* name = current->value.identifier;

        if(symtab_lookup(symbolTable, name) != NULL) {
            /* 
                TODO: More info with semantic error from existing 
                definition, also line number reference will be good 
             */
            fprintf(stderr, "Line %d: variable '%s' is already declared\n", current->line, name);
            return 0;
        }

        symbolTable = symtab_define(symbolTable, name, type);
        current = current->next;
    }

    return 1;
}


LyraType semantic_type_get(Ast ast)
{
    switch(ast->type) {
        case IDENTIFIER: return semantic_type_identifer_get(ast);
        case STRING: return L_STRING;
        case BOOLEAN: return L_BOOLEAN;
        case NUMBER: return L_NUMBER;
        case OP_BINARY: return semantic_type_binop_get(ast);
        default: fprintf(stderr, "Line %d: Invalid Expression Type\n", ast->line); return L_INVALID;
    }
}

LyraType semantic_type_identifer_get(Ast ast)
{
    SymTab st = symtab_lookup(symbolTable, ast->value.identifier);

    if(st == NULL) {
        fprintf(stderr, "Line %d: %s can not be used before it is declared\n", ast->line, ast->value.identifier);
        return L_INVALID;
    }

    return symtab_type_get(st);
}

LyraType semantic_type_binop_get(Ast ast)
{
    Ast opAst = ast->value.binop.operator;
    Ast leftAst = ast->value.binop.left;
    Ast rightAst = ast->value.binop.right;

    LyraType leftType = semantic_type_get(leftAst);
    LyraType rightType = semantic_type_get(rightAst);

    if(leftType != rightType) {
        fprintf(stderr, "Line %d: LHS and RHS of operator %c are not of same type\n", opAst->line, opAst->value.operator);
        return L_INVALID;
    }

    int operator = opAst->value.operator;

    if(operator == '+' || operator == '-' 
            || operator == '*' || operator == '/'
            || operator == '%') {
        if(leftType != L_NUMBER) { 
            fprintf(stderr, "Line %d: '%c' can only be used on NUMBERs\n", opAst->line, operator); 
            return L_INVALID;
        }
    }

    if(operator == '~' && leftType != L_STRING) {
        fprintf(stderr, "Line %d: '%c' can only be used on STRINGs\n", opAst->line, operator); 
        return L_INVALID;
    }

    return leftType;
}
