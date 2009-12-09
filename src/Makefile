all: lyrac
	@echo "All done"

CC=gcc
CFLAGS=-Wall -I../include

lyrac: y.tab.c lex.yy.c main.c
	$(CC) -o $@ $+ -lgc

lex.yy.c: ../spec/lyra.l
	flex $<

y.tab.c: ../spec/lyra.y
	yacc -d $<


clean:
	rm -f lex.yy.c
	rm -f y.tab.*
	rm -f *.o
	rm -f lyra
