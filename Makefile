#
# UFRGS - Compiladores B - Marcelo Johann - 2009/2 - Etapa 1
#
# Makefile for single compiler call
# All source files must be included from code embedded in scanner.l
# In our case, you probably need #include "hash.c" at the beginning
# and #include "main.c" in the last part of the scanner.l
#


etapa3: y.tab.o lex.yy.c
	gcc -o etapa3 y.tab.o lex.yy.c

lex.yy.c: scanner.l
	lex scanner.l
	
y.tab.o: y.tab.c
	gcc -c y.tab.c

y.tab.c: parser.y
	yacc -d -v parser.y

clean:
	rm etapa3 lex.yy.c y.tab.c y.tab.h y.output y.tab.o
