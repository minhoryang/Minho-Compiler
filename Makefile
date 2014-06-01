CFLAGS = -g -fms-extensions

20091631: lex.yy.o y.tab.o parse.o symtab.o main.o list.o
	gcc -o 20091631 *.o

y.tab.c: c-.y
y.tab.h: c-.y
	yacc -d c-.y

lex.yy.c: y.tab.h c-.l
	flex c-.l

clean:
	-rm lex.yy.c y.tab.*
	-rm *.o
	-rm 20091631
