//Thiago Parisotto Dias Tabela Hash
//Utilizada a implementação feita pelo professor durante as aulas com algumas leves modificações
#include "hash.h"


HASH_NODE* Table[HASH_SIZE];



void hashInit(void) {
    printf("inicializado tabela hash\n");
    int i;
    for( i = 0; i < HASH_SIZE; ++i )
        Table[i] = 0;
}

int hashAddress(char *text) {
    int i;
    int address = 1;
    for (i=0; i<strlen(text); i++)
        address = (address * text[i]) % HASH_SIZE + 1;
    
    return address - 1;
}

HASH_NODE *hashInsert(int type, char *text) {
    HASH_NODE *newNode;
    int address;
    
    newNode = (HASH_NODE*) calloc(1,sizeof(HASH_NODE));
    newNode->type = type;
    newNode->text = (char*) calloc(strlen(text) + 1, sizeof(char));
    strcpy(newNode->text, text);
    newNode->next = 0;
    
    address = hashAddress(text);
    newNode->next = Table[address];
    Table[address] = newNode;
    return newNode;
}

void hashPrint(void) {
    int i;
    HASH_NODE *node;
    for (i = 0; i<HASH_SIZE; ++i) {
        for (node=Table[i]; node; node = node->next) {
            printf("Table[%d] has %d, value: %s\n", i, node->type, node->text);
        }
    }
    
}

