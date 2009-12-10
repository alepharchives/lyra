#include <stdlib.h>
#include <string.h>
#include <gc.h>
#include <lyra.h>

void usage(const char* basename)
{
    fprintf(stderr, "Usage: %s <filename.lyra>\n", basename);
}

int check_fname(const char* infile)
{
    const char* ext = ".lyra";
    const int extlen = strlen(ext);

    int infile_full_len = strlen(infile);
    int infile_name_len = infile_full_len - extlen;
    const char* infile_extp = infile + infile_name_len;

    if(strncmp(ext, infile_extp, extlen) != 0) {
        return 0;
    }

    return 1;
}

char* get_fname_from(const char* infile, const char* outext)
{
    const char* ext = ".lyra";
    const int extlen = strlen(ext);

    int infile_full_len = strlen(infile);
    int infile_name_len = infile_full_len - extlen;

    const int outextlen = strlen(outext);

    int outfile_full_len = infile_name_len + outextlen + 1;
    char *outfile = GC_malloc(sizeof(char) * outfile_full_len);
    memcpy(outfile, infile, infile_name_len);
    memcpy(outfile+infile_name_len, outext, outextlen);
    outfile[outfile_full_len] = '\0';
    return outfile;
}
