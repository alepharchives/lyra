OBJECTS=../bin/lyrac \
        ../bin/lyraast

all: $(OBJECTS)
	@echo "All done"

CC=gcc
CFLAGS=-I../include

../bin/lyraast: y.tab.c lex.yy.c ast.o util.o lyraast.o
	@mkdir -p ../bin
	$(CC) $(CFLAGS) -o $@ $+ -lgc

../bin/lyrac: y.tab.c lex.yy.c ast.o util.o lyrac.o
	@mkdir -p ../bin
	$(CC) $(CFLAGS) -o $@ $+ -lgc

lex.yy.c: ../spec/lyra.l
	flex $<

y.tab.c: ../spec/lyra.y
	yacc -d $<


clean:
	rm -f lex.yy.c
	rm -f y.tab.*
	rm -f *.o
	rm -rf bin
