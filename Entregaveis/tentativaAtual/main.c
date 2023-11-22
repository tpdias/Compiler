// Thiago Parisotto Dias
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv) {
    //se não tiver 2 args
    if (argc <= 1) {
        fprintf(stderr, "Call: ./a.out");
        exit(1);
    }
    //abertura de arquivo para leitura
    if (0 == (yyin = fopen(argv[1],"r"))) {
        fprintf(stderr, "Cannot open file %s\n", argv[1]);
        exit(2);
    }
    //inicialização
    initMe();

    yyparse();

    hashPrint();

    printf("Sucesso!\n");
    exit(0);
}

