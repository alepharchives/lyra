#include <stdlib.h>
#include <string.h>
#include <gc.h>
#include <lyra.h>

static int varIndex = 0;

static int inter_trans_stmt_read(Ast);
static int inter_trans_stmt_print(Ast);
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
static LyraType op2type(int);


int inter_translate_program(Ast ast)
{
    Ast current = ast;

    int status = 0;

    while(current != NULL) {

        switch(current->type) {
            case STATEMENT_READ: status = inter_trans_stmt_read(current);break;
            case STATEMENT_PRINT: status = inter_trans_stmt_print(current);break;
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


/*
   A program like

   number a, b, c;

   stdin a, b, c;

   Will be translated into

   READ number v0
   READ number v1
   READ number v2

*/
static int inter_trans_stmt_read(Ast ast)
{
    Ast current = ast->value.stmt_read.idlist;

    while(current != NULL) {
        const char* var = current->value.identifier;
        SymTab st = symtab_lookup(symbolTable, var);
        const char* ivar = symtab_string_get(st);

        SymTab st1 = symtab_lookup(symbolTable, ivar);
        LyraType ltype = symtab_type_get(st1);

        ICode ic = icode_read_new(ltype);
        ic = icode_name_set(ic, ivar);

        iCode = icode_append(iCode, ic);

        current = current->next;
    }

    return 1;
}

/*
   An expression like 

   stdout 5+5;

   or

   stdout "abcde";

   will be converted to the following ICode fragments

   num v2 = 5;
   num v1 = 5;
   num v0 = v1 + v2;
   PRINT num v0;

   and

   str v0 = "abcde";
   PRINT str v0
*/

static int inter_trans_stmt_print(Ast ast)
{
    Ast current = ast->value.stmt_print.explist;

    while(current != NULL) {
        /* First build an expression ICode for the current
           expression, hence getting its type,
           */
        const char* ivar = newvar();

        ICode exp = inter_trans_exp(current, ivar);
        iCode = icode_append(iCode, exp);

        LyraType ltype = icode_ltype_get(exp);

        /* Then build a print ICode with the name
           and type of the just built expression
           ICode */

        ICode print = icode_print_new(ltype);
        print = icode_name_set(print, ivar);
        iCode = icode_append(iCode, print);

        current = current->next;
    }

    return 1;
}

static int inter_trans_stmt_assign(Ast ast)
{
    const char* var = ast->value.stmt_assign.key->value.identifier;
    SymTab st = symtab_lookup(symbolTable, var);
    const char* ivar = symtab_string_get(st);

    ICode rhs = inter_trans_exp(ast->value.stmt_assign.value, ivar);

    iCode = icode_append(iCode, rhs);

    return 1;
}

static int inter_trans_stmt_init(Ast ast)
{
    const char* var = ast->value.stmt_init.key->value.identifier;
    LyraType ltype = ast->value.stmt_init.type->value.type;

    const char* ivar = newvar();

    /* first name 'var' to 'ivar' in symbol table
       for use in the translation */
    symbolTable = symtab_define(symbolTable, var, L_STRING);
    symbolTable = symtab_string_set(symbolTable, ivar);

    /* then add a declaration for 'ivar' in symbol table
       so we can grab type information later in the 
       translation process */
    symbolTable = symtab_define(symbolTable, ivar, ltype);

    ICode rhs = inter_trans_exp(ast->value.stmt_init.value, ivar);
    iCode = icode_append(iCode, rhs);

    return 1;

}

static int inter_trans_stmt_declare(Ast ast)
{
    Ast current = ast->value.stmt_declare.idlist;
    LyraType ltype = ast->value.stmt_declare.type->value.type;

    while(current != NULL) {
        const char* ivar = newvar();
        symbolTable = symtab_define(symbolTable, current->value.identifier, L_STRING);
        symbolTable = symtab_string_set(symbolTable, ivar);
        symbolTable = symtab_define(symbolTable, ivar, ltype);

        current = current->next;
    }

    return 1;
}


static ICode inter_trans_exp(Ast ast, const char* ivar)
{
    switch(ast->type) {
        case NUMBER: return inter_trans_number(ast, ivar);
        case STRING: return inter_trans_string(ast, ivar);
        case BOOLEAN: return inter_trans_boolean(ast, ivar);
        case IDENTIFIER: return inter_trans_identifier(ast, ivar);
        case OP_BINARY: return inter_trans_binop(ast, ivar);
        default: fprintf(stderr, "Unknown Lyra expression type\n");return 0;
    }
}

static ICode inter_trans_number(Ast ast, const char* ivar)
{
    ICode ic = icode_assign_new(I_ASSIGN_NUMBER, L_NUMBER);
    ic = icode_name_set(ic, ivar);
    ic = icode_number_set(ic, ast->value.number);
    return ic;
}

static ICode inter_trans_string(Ast ast, const char* ivar)
{
    ICode ic = icode_assign_new(I_ASSIGN_STRING, L_STRING);
    ic = icode_name_set(ic, ivar);
    ic = icode_string_set(ic, ast->value.string);
    return ic;
}

static ICode inter_trans_boolean(Ast ast, const char* ivar)
{
    ICode ic = icode_assign_new(I_ASSIGN_BOOLEAN, L_BOOLEAN);
    ic = icode_name_set(ic, ivar);
    ic = icode_boolean_set(ic, ast->value.boolean);
    return ic;
}

static ICode inter_trans_identifier(Ast ast, const char* ivar)
{
    const char* var = ast->value.identifier;
    SymTab st = symtab_lookup(symbolTable, var);

    const char* ivar2 = symtab_string_get(st);

    SymTab st2 = symtab_lookup(symbolTable, ivar2);
    LyraType ltype = symtab_type_get(st2);

    ICode ic = icode_assign_new(I_ASSIGN_IDENTIFIER, ltype);

    ic = icode_name_set(ic, ivar);
    ic = icode_identifier_set(ic, ivar2);

    return ic;
}

static ICode inter_trans_binop(Ast ast, const char* ivar)
{
    const char* left = newvar();
    const char* right = newvar();
    ICode ic_left = inter_trans_exp(ast->value.binop.left, left);
    ICode ic_right = inter_trans_exp(ast->value.binop.right, right);
    int op = ast->value.binop.operator->value.operator;

    LyraType ltype = op2type(op);

    ICode ic = icode_assign_new(I_ASSIGN_BINOP, ltype);
    ic = icode_name_set(ic, ivar);
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

static LyraType op2type(int op)
{
    switch(op) {
        case '+':
        case '-':
        case '*':
        case '/':
        case '%': return L_NUMBER;
        case '~': return L_STRING;
        default: return L_INVALID;
    }
}
