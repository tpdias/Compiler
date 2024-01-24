//Thiago Parisotto Dias Tabela Hash
//Utilizada a implementação feita pelo professor durante as aulas com algumas leves modificações
#include "hash.h"
extern int getLineNumber();

HASH_NODE* Table[HASH_SIZE];



void hashInit(void) {
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
    HASH_NODE *newNode = hashFind(text);
    
    if(newNode != 0) return newNode;
    
    int address = hashAddress(text);

    newNode = (HASH_NODE*) calloc(1,sizeof(HASH_NODE));
    newNode->type = type;
    
    newNode->lineNumber = getLineNumber();
    newNode->isImplemented = 0;
    if(newNode->type == SYMBOL_LIT_CHAR) newNode->datatype = DATATYPE_CHAR; 
    else if(newNode->type == SYMBOL_LIT_FLOAT) newNode->datatype = DATATYPE_FLOAT;
    else if(newNode->type == SYMBOL_LIT_INT) newNode->datatype = DATATYPE_INT;

    
    newNode->text = (char*) calloc(strlen(text) + 1, sizeof(char));
    strcpy(newNode->text, text);
        
    newNode->next = Table[address];
    Table[address] = newNode;
    return newNode;
}

void hashPrint(void) {
    int i;
    HASH_NODE *node;
    for (i = 0; i<HASH_SIZE; ++i) {
        for (node=Table[i]; node; node = node->next) {
            printf("Table[%d] has %s, type: %d, dataType: %d\n", i, node->text, node->type, node->datatype);
        }
    }
    
}

HASH_NODE *hashFind(char *text) {
    int i;
    HASH_NODE *node;
    for (i = 0; i<HASH_SIZE; ++i) {
        for (node=Table[i]; node; node = node->next) {
            if(strcmp(node->text, text) == 0){
                return node;
            }
        }
    }
    return 0;
}

int checkUndeclaredHash(void) {
    HASH_NODE *node;
    int undeclaredVariables = 0;
    for (int i = 0; i < HASH_SIZE; i++) {
        if (Table[i] == 0) continue;
        for (node = Table[i]; node; node = node->next) {
            if (node->type == SYMBOL_IDENTIFIER) {
                fprintf(stderr, "Semantic ERROR Line %d: Undeclared identifier %s\n", node->lineNumber, node->text);
                undeclaredVariables++;
            }
            fprintf(stderr, "Table[%d] has %s, type: %d, dataType: %d\n", i, node->text, node->type, node->datatype);
        }
    }
    return undeclaredVariables;
}

HASH_NODE *makeTemp(void) {
    static int serialNumber = 0;
    static char buffer[256];
    sprintf(buffer, "__temp%d", serialNumber++);
    return hashInsert(SYMBOL_IDENTIFIER, buffer);
}

HASH_NODE *makeLabel(void) {
    static int serialNumber = 0;
    static char buffer[256];
    sprintf(buffer, "__label%d", serialNumber++);
    return hashInsert(SYMBOL_IDENTIFIER, buffer);
}