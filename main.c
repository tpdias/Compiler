// Thiago Parisotto Dias
#include <stdio.h>
#include <stdlib.h>
#include "ast.h"
#include "asm.h"
extern FILE* yyin;
extern int yyparse();
extern void initMe();
extern AST* getRoot();
extern TAC* getTAC();
extern void generateASM(TAC* tac, AST* ast);

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

    generateASM(getTAC(), getRoot());
    
    printf("Sucesso!\n");
    fclose(output);
    fclose(yyin);
    exit(0);
}

