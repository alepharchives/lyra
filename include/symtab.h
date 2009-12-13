#ifndef _SYMTAB_H
#define _SYMTAB_H

typedef struct _SymTab* SymTab;

extern SymTab symbolTable;

SymTab symtab_new();

SymTab symtab_define(SymTab, const char*, LyraType);
SymTab symtab_lookup(SymTab, const char*);

LyraType symtab_type_get(SymTab);

SymTab symtab_string_set(SymTab, const char*);
SymTab symtab_number_set(SymTab, int);
SymTab symtab_boolean_set(SymTab, LyraBoolean);

#endif
