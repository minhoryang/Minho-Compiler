20091631: main.o util.o yywrap.o lex.yy.o
	gcc -o 20091631 *.o

lex.yy.o: lex.yy.c

lex.yy.c: lex/tiny.l
	flex lex/tiny.l

clean:
	-rm *.o lex.yy.c 20091631 
