#ifndef TAC_HEADER
#define TAC_HEADER
#include "hash.h"
#include "ast.h"

enum OPCODES { TAC_VOID,
    TAC_SYMBOL, TAC_ADD, TAC_SUB, TAC_MUL, TAC_DIV

};

typedef struct tac_node
{
    int opcode;
    HASH_NODE *res;
    HASH_NODE *op1;
    HASH_NODE *op2;
    struct tac_node *prev;
    struct tac_node *next;
} TAC;

TAC *tacCreate(int type, HASH_NODE *res, HASH_NODE *op1, HASH_NODE *op2);
void tacPrintSingle(TAC *tac);
void tacPrintBack(TAC *tac);
TAC* codegen(AST *node);
TAC *tacJoin(TAC *l1, TAC *l2);




#endif