#include <stdlib.h>
#include <lyra.h>


int main(int argc, char** argv)
{
    CHECK_ARGCOUNT(argc, argv);
    OPEN_INFILE(argv, infilename);
    
    TRY_PARSE();

    TRY_TYPECHECK();

    TRY_TRANSLATE_TO_IR();

    icode_printf(iCode, 1);

    return 0;
}
