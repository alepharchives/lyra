#include <stdlib.h>
#include <string.h>
#include <gc.h>
#include <lyra.h>

static int varIndex = 0;

static int inter_trans_stmt_assign(Ast);
static int inter_trans_stmt_init(Ast);
static int inter_trans_stmt_declare(Ast);

static ICode inter_trans_exp(Ast, const char*);
static ICode inter_trans_number(Ast, const char*);
static ICode inter_trans_string(Ast,const char*);
static ICode inter_trans_boolean(Ast, const char*);
static ICode inter_trans_identifier(Ast, const char*);
static ICode inter_trans_binop(Ast, const char*);

static char* newvar();


int inter_translate_program(Ast ast)
{
    Ast current = ast;

    int status = 0;

    while(current != NULL) {

        switch(current->type) {
    /*        case STATEMENT_READ: inter_trans_stmt_read(current);break;
            case STATEMENT_PRINT: inter_trans_stmt_print(current);break;*/
            case STATEMENT_ASSIGN: status = inter_trans_stmt_assign(current);break;
            case STATEMENT_INIT: status = inter_trans_stmt_init(current);break;
            case STATEMENT_DECLARE: status = inter_trans_stmt_declare(current);break;
            default: fprintf(stderr, "Unknown Lyra Statement Type\n");return 0;
        }

        if(!status) {
            return 0;
        }

        current = current->next;
    }

    return 1;
}


static int inter_trans_stmt_assign(Ast ast)
{
    const char* name = ast->value.stmt_assign.key->value.identifier;
    SymTab st = symtab_lookup(symbolTable, name);
    const char* place = symtab_string_get(st);

    ICode rhs = inter_trans_exp(ast->value.stmt_assign.value, place);

    iCode = icode_append(iCode, rhs);

    return 1;
}

static int inter_trans_stmt_init(Ast ast)
{
    const char* name = ast->value.stmt_init.key->value.identifier;
    const char* place = newvar();
    symbolTable = symtab_define(symbolTable, name, L_STRING);
    symbolTable = symtab_string_set(symbolTable, place);

    ICode rhs = inter_trans_exp(ast->value.stmt_init.value, place);
    iCode = icode_append(iCode, rhs);

    return 1;

}

static int inter_trans_stmt_declare(Ast ast)
{
    /*what to do about types?*/
    Ast current = ast->value.stmt_declare.idlist;

    while(current != NULL) {
        const char* place = newvar();
        symbolTable = symtab_define(symbolTable, current->value.identifier, L_STRING);
        symbolTable = symtab_string_set(symbolTable, place);

        current = current->next;
    }

    return 1;
}


static ICode inter_trans_exp(Ast ast, const char* place)
{
    switch(ast->type) {
        case NUMBER: return inter_trans_number(ast, place);
        case STRING: return inter_trans_string(ast, place);
        case BOOLEAN: return inter_trans_boolean(ast, place);
        case IDENTIFIER: return inter_trans_identifier(ast, place);
        case OP_BINARY: return inter_trans_binop(ast, place);
        default: fprintf(stderr, "Unknown Lyra expression type\n");return 0;
    }
}

static ICode inter_trans_number(Ast ast, const char* place)
{
    ICode ic = icode_assign_new(I_ASSIGN_NUMBER);
    ic = icode_name_set(ic, place);
    ic = icode_number_set(ic, ast->value.number);
    return ic;
}

static ICode inter_trans_string(Ast ast, const char* place)
{
    ICode ic = icode_assign_new(I_ASSIGN_STRING);
    ic = icode_name_set(ic, place);
    ic = icode_string_set(ic, ast->value.string);
    return ic;
}

static ICode inter_trans_boolean(Ast ast, const char* place)
{
    ICode ic = icode_assign_new(I_ASSIGN_BOOLEAN);
    ic = icode_name_set(ic, place);
    ic = icode_boolean_set(ic, ast->value.boolean);
    return ic;
}

static ICode inter_trans_identifier(Ast ast, const char* place)
{
    ICode ic = icode_assign_new(I_ASSIGN_IDENTIFIER);
    ic = icode_name_set(ic, place);

    const char* name = ast->value.identifier;
    SymTab st = symtab_lookup(symbolTable, name);
    const char* var = symtab_string_get(st);
    ic = icode_identifier_set(ic, var);
    return ic;
}

static ICode inter_trans_binop(Ast ast, const char* place)
{
    printf("translating binary operation\n");
    const char* left = newvar();
    const char* right = newvar();
    ICode ic_left = inter_trans_exp(ast->value.binop.left, left);
    ICode ic_right = inter_trans_exp(ast->value.binop.right, right);
    int op = ast->value.binop.operator->value.operator;

    ICode ic = icode_assign_new(I_ASSIGN_BINOP);
    ic = icode_name_set(ic, place);
    ic = icode_binop_set(ic, op, left, right);

    ICode r = icode_append(ic_right, ic_left);
    r = icode_append(r, ic);

    return r;
}

static char* newvar()
{
    char* s = GC_malloc(sizeof(*s) * 255);
    memset(s, '\0', 255);

    snprintf(s, 255, "v%d", varIndex++);

    return s;
}
