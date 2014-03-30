20091631: main.o util.o yywrap.o lex.yy.o
	gcc -o 20091631 *.o

lex.yy.o: lex.yy.c

lex.yy.c: lex/c-.l
	flex lex/c-.l

clean:
	-@rm *.o lex.yy.c 20091631 
