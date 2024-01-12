#ifndef TAC_HEADER
#define TAC_HEADER
#include "hash.h"
#include "ast.h"

enum OPCODES { TAC_SYMBOL, 
    TAC_MOVE, TAC_ADD, TAC_SUB, TAC_MUL,
    TAC_DIV, TAC_LABEL, TAC_BEGINFUN, TAC_ENDFUN, 
    TAC_IFZ, TAC_JUMP, TAC_CALL, TAC_ARG, 
    TAC_RET, TAC_PRINT, TAC_READ, TAC_LESS,
    TAC_GREATER, TAC_LE, TAC_GE, TAC_EQ, TAC_DIF,
    TAC_AND, TAC_OR, TAC_NOT
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