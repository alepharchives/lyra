OBJECTS=../bin/lyrac \
        ../bin/lyra2ast \
        ../bin/lyra2ir

all: $(OBJECTS)
	@echo "All done"

CC=gcc
AR=ar
CFLAGS=-I../include -L../bin

../bin/liblyra.a: y.tab.o lex.yy.o ast.o util.o symtab.o semantic.o icode.o inter.o pirgen.o
	@mkdir -p ../bin
	$(AR) -rcs $@ $+

../bin/lyra2ast: ../bin/liblyra.a lyra2ast.o
	$(CC) $(CFLAGS) -o $@ lyra2ast.o -lgc -llyra

../bin/lyra2ir: ../bin/liblyra.a lyra2ir.o
	$(CC) $(CFLAGS) -o $@ lyra2ir.o -lgc -llyra

../bin/lyrac: ../bin/liblyra.a lyrac.o
	$(CC) $(CFLAGS) -o $@ lyrac.o -lgc -llyra

lex.yy.c: ../spec/lyra.l
	flex $<

y.tab.c: ../spec/lyra.y
	yacc -d $<

clean:
	rm -f lex.yy.c
	rm -f y.tab.*
	rm -f *.o
