CFLAGS = -g -fms-extensions #-L ~/.local/lib -lduma

project4_3: lex.yy.o y.tab.o parse.o analyze.o symtab.o main.o list.o code.o cgen.o tm
	gcc $(CFLAGS) -o project4_3 *.o

tm: tm.c
	gcc $(CFLAGS) -o tm tm.c

y.tab.c: c-.y
y.tab.h: c-.y
	yacc -d c-.y

lex.yy.c: y.tab.h c-.l
	flex c-.l

clean:
	-rm lex.yy.c y.tab.*
	-rm *.o
	-rm project4_3
	-rm tm
