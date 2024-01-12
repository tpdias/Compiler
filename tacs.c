#include "tacs.h"

char *opcodeNames[] = { "TAC_VOID",
    "TAC_SYMBOL", "TAC_ADD", "TAC_SUB", "TAC_MUL", 
    "TAC_DIV"
    };

TAC * tacCreate(int opcode, HASH_NODE *res, HASH_NODE *op1, HASH_NODE *op2) {
    TAC *newTac;
    newTac = (TAC*) calloc(1, sizeof(TAC));
    newTac->opcode = opcode;
    newTac->res = res;
    newTac->op1 = op1;
    newTac->op2 = op2;
    newTac->prev = 0;
    newTac->next = 0;
    return newTac;
}
void tacPrintSingle(TAC * tac) {
    if (!tac) return;
    fprintf(stderr, "TAC(");
    fprintf(stderr, "%s", opcodeNames[tac->opcode]);
    if (tac->res) fprintf(stderr, ", %s", tac->res->text);
    else fprintf(stderr, ", 0");
    if (tac->op1) fprintf(stderr, ", %s", tac->op1->text);
    else fprintf(stderr, ", 0");
    if (tac->op2) fprintf(stderr, ", %s", tac->op2->text);
    else fprintf(stderr, ", 0");
    fprintf(stderr, ")\n");
}

void tacPrintBack(TAC *tac) {
    if (!tac) return;
    tacPrintSingle(tac);
    tacPrintBack(tac->prev);
}

TAC *tacJoin(TAC *l1, TAC *l2) {
    TAC *tac;
    if (!l1) return l2;
    if (!l2) return l1;
    for (tac = l2; tac->prev; tac = tac->prev) {
        tac->prev = l1;
        return l2;
    }
}
TAC *codegen(AST *node) {
    TAC *code[MAX_SONS];
    if(!node) return;

    for(int i=0; i<MAX_SONS; i++) {
        code[i] = codegen(node->son[i]);
    }
    switch (node->type)
    {
    case AST_SYMBOL: 
        return tacCreate(TAC_SYMBOL, node->symbol, 0, 0);
        break;
    case AST_ADD: 
        return tacCreate(TAC_ADD, 0, code[0]?code[0]->res:0, code[1]?code[1]->res:0);
        break;
    case AST_SUB: break;
    case AST_MUL: break;
    case AST_DIV: break;
    
    default: fprintf(stderr, "ERROR: Unknown node type: %d\n", node->type);
        break;
    }
    return tacJoin(tacJoin(tacJoin(code[0], code[1]), code[2]), code[3]);
}