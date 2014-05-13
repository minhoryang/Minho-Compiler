20091631: main.o util.o lex.yy.o
	gcc -o 20091631 *.o

lex.yy.o: lex.yy.c

lex.yy.c: c-.l
	flex c-.l

clean:
	-rm *.o lex.yy.c 20091631 
