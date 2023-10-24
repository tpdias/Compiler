#include "hash.h"
#include <stdlib.h>
#include <string.h>

HASH_NODE* Table[HASH_SIZE];

void hashInit(void) {
    int i;
    for( i = 0; i < HASH_SIZE; ++i )
        Table[i] = 0;
}

int hashAddress(char *text) {
    int i;
    int address = 1;
    for (i=0; i<strlen(yytext); i++)
        address = (address * yytext[i]) % HASH_SIZE + 1;
    
    return address - 1;
}

HASH_NODE *hashInsert(int type, char *text) {
    HASH_NODE *newNode;
    int address;
    
    newNode = (HASH_NODE*) calloc(1,sizeof(HASH_NODE));
    newNode->type = type;
    newNode->text = (char*) calloc(strlen(yytext) + 1, sizeof(char));
    strcopy(newNode->text, text);
    newNode->next = 0;
    
    address = hashAddress(text);
    newNode->text = Table[address];
    Table[address] = newNode;
    return newNode;
}

void hashPrint(void) {
    int i;
    HASH_NODE *node;
    
    for (i = 0; i<HASH_SIZE; ++i) {
        for (node=Table[i]; node; node = node->next) {
            printf("Table[%d] has %d", i, node->type);
        }
    }
    
}
