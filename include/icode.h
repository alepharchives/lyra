#ifndef _ICODE_H
#define _ICODE_H

#include <gc.h>
#include <ast.h>

#define  ICODE_NEW(ic, ictype, lytype) ICode ic = GC_malloc(sizeof(*ic)); \
                             if(ic == NULL) { \
                                 return NULL; \
                             } \
                             ic->type = ictype; \
                             ic->atype = A_INVALID; \
                             ic->ltype = lytype; \
                             ic->tail = NULL; \
                             ic->next = NULL; \

typedef struct _ICode* ICode;

extern ICode iCode;

typedef enum {
    I_ASSIGN,
    I_PRINT,
    I_READ
} ICodeType;

typedef enum {
    A_CONSTANT,
    A_IDENTIFIER,
    A_BINOP,
    A_INVALID
} ICodeAssignType;


ICode icode_new();
ICode icode_assign_new(ICodeType, LyraType);
ICode icode_print_new(LyraType);
ICode icode_read_new(LyraType);
ICode icode_append(ICode,ICode);
ICode icode_name_set(ICode, const char *);
ICode icode_number_set(ICode, int);
ICode icode_string_set(ICode, const char*);
ICode icode_boolean_set(ICode, LyraBoolean);
ICode icode_identifier_set(ICode, const char*);
ICode icode_binop_set(ICode, int, const char*, const char*);

LyraType icode_ltype_get(ICode);

void icode_printf(ICode, int);

#endif
