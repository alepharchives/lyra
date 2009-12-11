#ifndef _SYMTAB_H
#define _SYMTAB_H

typedef _SymTab* SymTab;


SymTab symtab_new();

SymTab symtab_define(SymTab, const char*, LyraType);
SymTab symtab_lookup(SymTab, const char*);

SymTab symtab_string_set(SymTab, const char*);
SymTab symtab_number_set(SymTab, int);
SymTab symtab_boolean_set(SymTab, LyraBoolean);

#endif
