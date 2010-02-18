#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <lyra.h>

ICode iCode;



ICode icode_new()
{
    return NULL;
}

ICode icode_assign_new(ICodeAssignType atype, LyraType ltype)
{
    ICODE_NEW(ic, I_ASSIGN, ltype);
    ic->atype = atype;
    return ic;
}

ICode icode_print_new(LyraType ltype)
{
    ICODE_NEW(ic, I_PRINT, ltype);
    return ic;
}

ICode icode_read_new(LyraType ltype)
{
    ICODE_NEW(ic, I_READ, ltype);
    return ic;
}

ICode icode_append(ICode head, ICode new)
{
    if(head == NULL) {
        return new;
    }

    if(new == NULL) {
        return head;
    }

    if(head->next == NULL) {
        head->next = new;
        head->tail = new;
        return head;
    }

    head->tail->next = new;

    if(new->tail == NULL) {
        head->tail = new;
    } else {
        head->tail = new->tail;
    }

    return head;
}

ICode icode_name_set(ICode ic, const char* name)
{
    ic->name = strndup(name, 255);
    return ic;
}

ICode icode_number_set(ICode ic, int i)
{
    ic->value.number = i;
    return ic;
}

ICode icode_string_set(ICode ic, const char* s)
{
    ic->value.string = strndup(s, 255);
    return ic;
}

ICode icode_boolean_set(ICode ic, LyraBoolean b)
{
    ic->value.boolean = b;
    return ic;
}

ICode icode_identifier_set(ICode ic, const char *id, LyraType ltype)
{
    ic->value.identifier.value = strndup(id, 255);
    ic->value.identifier.ltype = ltype;
    return ic;
}

ICode icode_binop_set(ICode ic, int op, const char* left, const char* right, LyraType ltype)
{
    ic->value.binop.left.value = strndup(left, 255);
    ic->value.binop.left.ltype = ltype;
    ic->value.binop.op = op;
    ic->value.binop.right.value = strndup(right, 255);
    ic->value.binop.right.ltype = ltype;
    return ic;
}

LyraType icode_ltype_get(ICode ic)
{
    return ic->ltype;
}

void icode_printf_type(ICode ic)
{
    switch(ic->ltype) {
        case L_NUMBER: printf("num ");return;
        case L_STRING: printf("str ");return;
        case L_BOOLEAN: printf("bool ");return;
        default: printf("LINVALID ");return;
    }
}

void icode_printf_assign(ICode ic)
{
    icode_printf_type(ic);
    printf("%s = ", ic->name);

    if(ic->atype == A_INVALID) {
        printf("INVALID ASSIGNMENT\n");
        return;
    }

    if(ic->atype == A_IDENTIFIER) {
        printf("%s\n", ic->value.identifier);
        return;
    }

    if(ic->atype == A_BINOP) {
        printf("%s %c %s\n", ic->value.binop.left,
                ic->value.binop.op,
                ic->value.binop.right);
        return;
    }

    /* All are now A_CONSTANT, so we dip into
       ltype to check what to print out
    */
    if(ic->ltype == L_NUMBER) {
        printf("%d\n", ic->value.number);
        return;
    }

    if(ic->ltype == L_STRING) {
        printf("%s\n", ic->value.string);
        return;
    }

    if(ic->ltype == L_BOOLEAN) {
        if(ic->value.boolean) {
            printf("true\n");
        } else {
            printf("false\n");
        }
    }
}

void icode_printf_print(ICode ic)
{
    printf("PRINT ");
    icode_printf_type(ic);
    printf("%s\n", ic->name);
}

void icode_printf_read(ICode ic)
{
    printf("READ ");
    icode_printf_type(ic);
    printf("%s\n", ic->name);
}

void icode_printf(ICode ic, int i)
{
    if(ic == NULL) {
        printf("EOF\n");
        return;
    }

    printf("%3d: ", i);

    switch(ic->type) {
        case I_ASSIGN: icode_printf_assign(ic);break;
        case I_PRINT: icode_printf_print(ic);break;
        case I_READ: icode_printf_read(ic);break;
        default: fprintf(stderr, "Unknown Intermediate Code type\n");return;
    }

    i += 1;
    icode_printf(ic->next, i);
}
