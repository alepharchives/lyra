all: ../bin/lyrac
	@echo "All done"

CC=gcc
CFLAGS=-I../include

../bin/lyrac: y.tab.c lex.yy.c main.c 
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
