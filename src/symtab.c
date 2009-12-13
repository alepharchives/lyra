#include <stdlib.h>
#include <string.h>
#include <gc.h>
#include <lyra.h>

SymTab symbolTable;

struct _SymTab {
    char* name;
    LyraType type;

    union {
        char* string;
        int number;
        LyraBoolean boolean;
    } value;

    SymTab next;
};

SymTab symtab_new()
{
    return NULL;
}

SymTab symtab_define(SymTab tail, const char* name, LyraType type)
{
    SymTab st = GC_malloc(sizeof(*st));

    if(st == NULL) {
        return NULL;
    }

    st->name = strndup(name, 255); /* TODO: use GC_Malloc, with memncpy, to implement strndup */
    st->value.string = "";
    st->value.number = 0;
    st->value.boolean = L_FALSE;
    st->type = type;
    st->next = tail;

    return st;
}

SymTab symtab_lookup(SymTab head, const char* name)
{
    SymTab current = head;

    while(current != NULL) {
        if(strncmp(current->name, name, 255) == 0) {
            return current;
        }

        current = current->next;

    }

    return NULL;
}

LyraType symtab_type_get(SymTab st)
{
    return st->type;
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
