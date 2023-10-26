#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tokens.h"

#define HASH_SIZE 997

typedef struct hash_node
{
    int type;
    char *text;
    struct hash_node *next;
} HASH_NODE;

void hashInit(void);
int hashAddress(char *text);
HASH_NODE *hashInsert(int type, char *text);
void hashPrint(void);
