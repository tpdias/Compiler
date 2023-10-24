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
    int address = hashAddress(text);
        
        // Create a new node
        HASH_NODE* newNode = (HASH_NODE*)malloc(sizeof(HASH_NODE));
        newNode->type = type;
        newNode->text = strdup(text); // Make a copy of the text
        
        // Insert the node into the hash table
        newNode->next = Table[address];
        Table[address] = newNode;
        
        return newNode;
}

void hashPrint(void) {
    int i;
    for (i = 0; i < HASH_SIZE; ++i) {
        HASH_NODE* currentNode = Table[i];
        if (currentNode != NULL) {
            printf("Hash Table Position %d:\n", i);
            while (currentNode != NULL) {
                printf("  Type: %d, Text: %s\n", currentNode->type, currentNode->text);
                currentNode = currentNode->next;
            }
        }
    }
}
