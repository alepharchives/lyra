#include <stdlib.h>
#include <lyra.h>


int main(int argc, char** argv)
{
    CHECK_ARGCOUNT(argc, argv);
    OPEN_INFILE(argv, infilename);
    
    TRY_PARSE();

    TRY_TYPECHECK();

    symbolTable = symtab_new();
    iCode = icode_new();

    if(!inter_translate_program(rootAst)) {
        return -1;
    }

    icode_printf(iCode, 1);
    return 1;
}
