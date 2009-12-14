#include <stdlib.h>
#include <lyra.h>


int main(int argc, char** argv)
{
    CHECK_ARGCOUNT(argc, argv);
    OPEN_INFILE(argv, infilename);
    
    TRY_PARSE();

    TRY_TYPECHECK();

    return 1;
}
