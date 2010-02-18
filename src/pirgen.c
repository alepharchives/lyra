#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <lyra.h>


void pirgen_type(LyraType ltype)
{
    switch(ltype) {
        case L_NUMBER: printf("$I");return;
        case L_STRING: printf("$S");return;
        case L_BOOLEAN: printf("$I");return;
        default: printf("LINVALID ");return;
    }
}

void pirgen_assign(ICode ic)
{
    pirgen_type(ic->ltype);
    printf("%s = ", ic->name+1);

    if(ic->atype == A_INVALID) {
        printf("INVALID ASSIGNMENT\n");
        return;
    }

    if(ic->atype == A_IDENTIFIER) {
        pirgen_type(ic->value.identifier.ltype);
        printf("%s\n", ic->value.identifier+1);
        return;
    }

    if(ic->atype == A_BINOP) {
        pirgen_type(ic->value.binop.left.ltype);
        printf("%s ", ic->value.binop.left.value+1);

        printf("%c ", ic->value.binop.op);

        pirgen_type(ic->value.binop.right.ltype);
        printf("%s\n", ic->value.binop.right.value+1);

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
            printf("1\n");
        } else {
            printf("0\n");
        }
    }
}

void pirgen_print(ICode ic)
{
    printf("say ");
    pirgen_type(ic->ltype);
    printf("%s\n", ic->name+1);
}

void pirgen_read(ICode ic)
{
    printf("get ");
    pirgen_type(ic->ltype);
    printf("%s\n", ic->name+1);
}

void pirgen_head(ICode ic, int i)
{
    if(ic == NULL) {
        printf(".end\n");
        return;
    }

    switch(ic->type) {
        case I_ASSIGN: pirgen_assign(ic);break;
        case I_PRINT: pirgen_print(ic);break;
        case I_READ: pirgen_read(ic);break;
        default: fprintf(stderr, "Unknown Intermediate Code type\n");return;
    }

    i += 1;
    pirgen_head(ic->next, i);
}

void pirgen(ICode ic)
{
    int lnum = 0;
    printf(".sub 'main' :main\n");
    pirgen_head(ic, lnum);
}
