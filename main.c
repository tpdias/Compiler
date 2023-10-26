#include <stdio.h>
#include <stdlib.h>
int main(int argc, char **argv) {
    hashInit;
    //posso iniciar o hash aqui?
    if (argc <= 2) {
        fprintf(stderr, "Call: ./a.out file_name\n");
        exit(1);
    }
    if (0 == (yyin = fopen(argv[1],"r"))) {
        fprintf(stderr, "Cannot open file %s\n", argv[1]);
        exit(2);
    }
    while(running) {
        token = yylex();
        if (!running) {
            break;
        }
    }
    printf("Foram encontradas %d linhas\n", getLineNumber());
    //aqui nao ta printando nada
    hashPrint();
    exit(0);
}
