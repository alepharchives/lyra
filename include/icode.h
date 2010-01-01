#ifndef _ICODE_H
#define _ICODE_H

#include <gc.h>
#include <ast.h>

#define  ICODE_NEW(ic, type, ltype) ICode ic = GC_malloc(sizeof(*ic)); \
                             if(ic == NULL) { \
                                 return NULL; \
                             } \
                             ic->type = type; \
                             ic->ltype = ltype; \
                             ic->tail = NULL; \
                             ic->next = NULL; \

typedef struct _ICode* ICode;

extern ICode iCode;

typedef enum {
    I_ASSIGN_STRING,
    I_ASSIGN_NUMBER,
    I_ASSIGN_BOOLEAN,
    I_ASSIGN_IDENTIFIER,
    I_ASSIGN_BINOP
} ICodeType;


ICode icode_new();
ICode icode_assign_new(ICodeType, LyraType);
ICode icode_append(ICode,ICode);
ICode icode_name_set(ICode, const char *);
ICode icode_number_set(ICode, int);
ICode icode_string_set(ICode, const char*);
ICode icode_boolean_set(ICode, LyraBoolean);
ICode icode_identifier_set(ICode, const char*);
ICode icode_binop_set(ICode, int, const char*, const char*);

void icode_printf(ICode, int);

#endif
