#ifndef _LYRA_H
#define _LYRA_H

#include <stdio.h>
#include <libgen.h>

#define CHECK_ARGCOUNT(argc, argv) if(argc != 2) { \
                            usage(basename(argv[0])); \
                            return -1; \
                         } \
                         if(1)\

#define OPEN_INFILE(a, fname) const char* fname = a[1];\
                            if(!check_fname(fname)) { \
                                fprintf(stderr, "Unknown file extension.  Expecting .lyra\n"); \
                                exit(1); \
                            } \
                            fclose(stdin); \
                            stdin = fopen(fname, "r"); \
                            if(1) \

void usage();
int check_fname(const char*);
char* get_fname_from(const char*, const char*);


#endif
