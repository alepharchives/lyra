#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <gc.h>
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
    fprintf(stderr, "Usage: lyrac <filename.lyra>\n");
}

char* get_outfile(const char* infile)
{
    const char* ext = ".lyra";
    const int extlen = strlen(ext);

    int infile_full_len = strlen(infile);
    int infile_name_len = infile_full_len - extlen;
    const char* infile_extp = infile + infile_name_len;

    if(strncmp(ext, infile_extp, extlen) != 0) {
        fprintf(stderr, "Unknown file extension. Should be '%s'\n", ext);
        exit(1);
    }

    const char* outext = ".c";
    const int outextlen = strlen(outext);

    int outfile_full_len = infile_name_len + outextlen + 1;
    char *outfile = GC_malloc(sizeof(char) * outfile_full_len);
    memcpy(outfile, infile, infile_name_len);
    memcpy(outfile+infile_name_len, outext, outextlen);
    outfile[outfile_full_len] = '\0';
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

    yyparse();

    return 0;
}
