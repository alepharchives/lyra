OBJECTS=../bin/lyrac \
        ../bin/lyraast

all: $(OBJECTS)
	@echo "All done"

CC=gcc
AR=ar
CFLAGS=-I../include -L../bin

../bin/liblyra.a: y.tab.o lex.yy.o ast.o util.o symtab.o semantic.o
	@mkdir -p ../bin
	$(AR) -rcs $@ $+

../bin/lyraast: ../bin/liblyra.a lyraast.o
	$(CC) $(CFLAGS) -o $@ lyraast.o -lgc -llyra

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
