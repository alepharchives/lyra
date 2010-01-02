#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <lyra.h>

ICode iCode;

struct _ICode {
    ICodeType type;
    ICodeAssignType atype;
    LyraType ltype;
    char* name;
    union {
        char* identifier;
        char* string;
        int number;
        LyraBoolean boolean;

        struct {
            char* left;
            int op;
            char* right;
        } binop;
    } value;

    ICode next;
    ICode tail;
};


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

ICode icode_identifier_set(ICode ic, const char *id)
{
    ic->value.identifier = strndup(id, 255);
    return ic;
}

ICode icode_binop_set(ICode ic, int op, const char* left, const char* right)
{
    ic->value.binop.left = strndup(left, 255);
    ic->value.binop.op = op;
    ic->value.binop.right = strndup(right, 255);
    return ic;
}

LyraType icode_ltype_get(ICode ic)
{
    return ic->ltype;
}

void icode_printf(ICode ic, int i)
{
    if(ic == NULL) {
        printf("EOF\n");
        return;
    }

    printf("%d: ", i);
    switch(ic->ltype) {
        case L_NUMBER: printf("num ");break;
        case L_STRING: printf("str ");break;
        case L_BOOLEAN: printf("bool ");break;
        default: printf("LINVALID ");return;
    }

    switch(ic->type) {
        case I_ASSIGN_STRING: printf("%s = %s\n", ic->name, ic->value.string);break;
        case I_ASSIGN_NUMBER: printf("%s = %d\n", ic->name, ic->value.number);break;
        case I_ASSIGN_BOOLEAN: printf("%s = %d\n", ic->name, ic->value.boolean);break;
        case I_ASSIGN_IDENTIFIER: printf("%s = %s\n", ic->name, ic->value.identifier);break;
        case I_ASSIGN_BINOP: printf("%s = %s %c %s\n", ic->name,
                                     ic->value.binop.left,
                                     ic->value.binop.op,
                                     ic->value.binop.right);break;
        case I_PRINT: printf("PRINT %s\n", ic->name);break;
        case I_READ: printf("READ %s\n", ic->name);break;
        default: fprintf(stderr, "Unknown Intermediate Code type\n");return;
    }

    i += 1;
    icode_printf(ic->next, i);
}
