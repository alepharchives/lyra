#include <stdlib.h>
#include <gc.h>
#include <ast.h>
#include <symtab.h>

struct _SymTab {
    char* name;
    LyraType type;

    union {
        char* string;
        int number;
        LyraBoolean boolean;
    } value;

    SymTab next;
}

SymTab symtab_new()
{
    reutrn NULL;
}

SymTab symtab_define(SymTab tail, const char* name, LyraType type)
{
    SymTab st = GC_Malloc(sizeof(*st));

    if(st == NULL) {
        return NULL;
    }

    st->name = strndup(name, 255); /* TODO: use GC_Malloc, with memncpy, to implement strndup */
    st->type = type;
    st->next = tail;

    return st;
}

SymTab symtab_lookup(SymTab head, const char* name)
{
}

SymTab symtab_string_set(SymTab st, const char* s)
{
    st->value.string = strndup(s, 255);
    return st;
}

SymTab symtab_number_set(SymTab st, int n)
{
    st->value.number = n;
    return st;
}

SymTab symtab_boolean_set(SymTab st, LyraBoolean b)
{
    st->value.boolean = b;
    return st;
}
