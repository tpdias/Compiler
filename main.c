// Thiago Parisotto Dias
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv) {
    //se não tiver 2 args
    if (argc <= 2) {
        fprintf(stderr, "Call: ./a.out file_name\n");
        exit(1);
    }
    //abertura de arquivo para leitura
    if (0 == (yyin = fopen(argv[1],"r"))) {
        fprintf(stderr, "Cannot open file %s\n", argv[1]);
        exit(2);
    }
    //inicialização
    initMe;
    //função de running, printa o token encontrado e o texto
    yyparse();
//    while(isRunning()) {
//        token = yylex();
//        //printa para fins de debug
//        if (!isRunning()) {
//            break;
//        }
//    }
    hashPrint();

    printf("Sucesso!\n");
    exit(0);
}

