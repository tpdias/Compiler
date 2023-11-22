#include <stdio.h>
#include <stdlib.h>

//Função utilizada para debbuging, retorna o token encontrado em formato de string
const char* getTokenString(int token) {
    switch (token) {
        case KW_CHAR:
            return "KW_CHAR";
        case KW_INT:
            return "KW_INT";
        case KW_FLOAT:
            return "KW_FLOAT";
        case KW_CODE:
            return "KW_CODE";
        case KW_IF:
            return "KW_IF";
        case KW_ELSE:
            return "KW_ELSE";
        case KW_WHILE:
            return "KW_WHILE";
        case KW_INPUT:
            return "KW_INPUT";
        case KW_PRINT:
            return "KW_PRINT";
        case KW_RETURN:
            return "KW_RETURN";
        case OPERATOR_LE:
            return "OPERATOR_LE";
        case OPERATOR_GE:
            return "OPERATOR_GE";
        case OPERATOR_EQ:
            return "OPERATOR_EQ";
        case OPERATOR_DIF:
            return "OPERATOR_DIF";
        case TK_IDENTIFIER:
            return "TK_IDENTIFIER";
        case LIT_INT:
            return "LIT_INT";
        case LIT_REAL:
            return "LIT_REAL";
        case LIT_CHAR:
            return "LIT_CHAR";
        case LIT_STRING:
            return "LIT_STRING";
        case TOKEN_ERROR:
            return "TOKEN_ERROR";
        default:
            return "UNKNOWN_TOKEN";
    }
}

int main(int argc, char **argv) {
    initMe;
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
    //função de running, printa o token encontrado e o texto
    while(running) {
        token = yylex();
        //printa para fins de debug
        printf("Encontrado token %s, valor %s\n", getTokenString(token), yytext);
        if (!running) {
            break;
        }
    }
    printf("Foram encontradas %d linhas\n", getLineNumber());
    //aqui nao ta printando nada
    hashPrint();
    exit(0);
}
