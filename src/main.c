#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/lyra.h"
#include "../src/y.tab.h"

void yyerror(const char *str)
{
    fprintf(stderr, "error: %s\n", str);
}

int yywrap()
{
    return 1;
}

int usage()
{
    fprintf(stderr, "Usage: janec <file.j>\n");
}

char* get_outfile(const char* infile)
{
    int l = strlen(infile);
    int extlen = l - 2;

    if(strncmp(".j", infile+extlen, 2) != 0) {
        fprintf(stderr, "Unknown file extension. Should be '.j'\n", infile+extlen);
        exit(1);
    }

    char *outfile = strndup(infile, l);
    strncpy(outfile+extlen, ".c", 2);
    return outfile;
}

int main(int argc, char** argv)
{
    if(argc != 2) {
        usage();
        return 0;
    }

    const char* infilename = argv[1];
    char* outfilename = get_outfile(infilename);

    fclose(stdin);
    stdin = fopen(infilename, "r"); 

    fclose(stdout);
    stdout = fopen(outfilename, "w"); 
    free(outfilename);

    yyparse();

    return 0;
}

