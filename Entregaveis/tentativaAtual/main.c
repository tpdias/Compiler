// Thiago Parisotto Dias
#include <stdio.h>
#include <stdlib.h>
#include "ast.h"
extern FILE* yyin;
extern int isRunning();
extern int getLineNumber();
extern int yyparse();
extern void initMe();
extern void hashPrint();
extern AST* getRoot();

int main(int argc, char **argv) {
    FILE* output;
    //se não tiver 2 args
    if (argc <= 2) {
        fprintf(stderr, "Call: ./etapa3 input.txt output.txt\n");
        exit(1);
    }
    //abertura de arquivo para leitura
    if (0 == (yyin = fopen(argv[1],"r"))) {
        fprintf(stderr, "Cannot open file %s\n", argv[1]);
        exit(2);
    }
    //abertura de arquivo para escrita
    if (0 == (output = fopen(argv[2],"w"))) {
        fprintf(stderr, "Cannot open file %s\n", argv[2]);
        exit(2);
    }
    //inicialização
    initMe();

    yyparse();

    fprintf(stderr, "Parsing done!\n");
    
    uncompile(getRoot(), output);

    fprintf(stderr, "Uncompiling done!\n");

    hashPrint();
    
    printf("Sucesso!\n");


    exit(0);
}

