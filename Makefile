#
# UFRGS - Compiladores B - Marcelo Johann - 2009/2 - Etapa 1
#
# Makefile for single compiler call
# All source files must be included from code embedded in scanner.l
# In our case, you probably need #include "hash.c" at the beginning
# and #include "main.c" in the last part of the scanner.l
#

etapa6: hash.o ast.o y.tab.o lex.yy.o semantic.o tac.o asm.o
	gcc -Wall -o etapa6 hash.o ast.o y.tab.o lex.yy.o semantic.o tacs.o asm.o

asm.o: asm.c
	gcc -Wall -c asm.c

tac.o: tacs.c
	gcc -Wall -c tacs.c

semantic.o: semantic.c
	gcc -Wall -c semantic.c

hash.o: hash.c
	gcc -c hash.c

ast.o: ast.c
	gcc -c ast.c

y.tab.o: y.tab.c
	gcc -c y.tab.c

y.tab.c: parser.y
	yacc -d -v parser.y

lex.yy.o: lex.yy.c
	gcc -c lex.yy.c

lex.yy.c: scanner.l
	flex --header-file=lex.yy.h scanner.l

clean:
	rm -f *.o etapa5 lex.yy.c lex.yy.h y.tab.c y.tab.h y.output
