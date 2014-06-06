CFLAGS = -g -fms-extensions #-L ~/.local/lib -lduma

project3_3: lex.yy.o y.tab.o parse.o analyze.o symtab.o main.o list.o
	gcc $(CFLAGS) -o project3_3 *.o

y.tab.c: c-.y
y.tab.h: c-.y
	yacc -d c-.y

lex.yy.c: y.tab.h c-.l
	flex c-.l

clean:
	-rm lex.yy.c y.tab.*
	-rm *.o
	-rm project3_3
