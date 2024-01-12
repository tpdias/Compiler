#include "tacs.h"

char *opcodeNames[] = { "TAC_SYMBOL", 
    "TAC_MOVE", "TAC_ADD", "TAC_SUB", "TAC_MUL",
    "TAC_DIV", "TAC_LABEL", "TAC_BEGINFUN", "TAC_ENDFUN",
    "TAC_IFZ", "TAC_JUMP", "TAC_CALL", "TAC_ARG",
    "TAC_RET", "TAC_PRINT", "TAC_READ", "TAC_LESS",
    "TAC_GREATER", "TAC_LE", "TAC_GE", "TAC_EQ", "TAC_DIF",
    "TAC_AND", "TAC_OR", "TAC_NOT"
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
    tac = l2;
    while(tac->prev) tac = tac->prev;
    tac->prev = l1;
    return l2;
}

TAC *codegen(AST *node) {
    TAC *code[MAX_SONS];
    if(!node) return;

    for(int i=0; i<MAX_SONS; i++) {
        code[i] = codegen(node->son[i]);
    }
    switch (node->type)
    {
    //symbol
    case AST_SYMBOL: return tacCreate(TAC_SYMBOL, node->symbol, 0, 0); break;
    //operations
    case AST_ADD: return make_binop(TAC_ADD, code[0], code[1]); break;
    case AST_SUB: return make_binop(TAC_SUB, code[0], code[1]); break;
    case AST_MUL: return make_binop(TAC_MUL, code[0], code[1]); break;
    case AST_DIV: return make_binop(TAC_DIV, code[0], code[1]); break;
    case AST_LESS: return make_binop(TAC_LESS, code[0], code[1]); break;
    case AST_GREATER: return make_binop(TAC_GREATER, code[0], code[1]); break;
    case AST_LE: return make_binop(TAC_LE, code[0], code[1]); break;
    case AST_GE: return make_binop(TAC_GE, code[0], code[1]); break;
    case AST_EQ: return make_binop(TAC_EQ, code[0], code[1]); break;
    case AST_DIF: return make_binop(TAC_DIF, code[0], code[1]); break;
    case AST_AND: return make_binop(TAC_AND, code[0], code[1]); break;
    case AST_OR: return make_binop(TAC_OR, code[0], code[1]); break;
    case AST_NOT: return make_binop(TAC_NOT, code[0], code[1]); break;
    //commands
    case AST_ATTREXPR: break;
    case AST_ATTRVEC: break;
    case AST_INPUT: break;
    case AST_PRINT: break;
    case AST_PRINTEXP: break;
    case AST_RETURN: break;
    case AST_IF: break;
    case AST_ELSE: break;
    case AST_WHILE: break;

    case AST_VEC: break;
    case AST_PARAM: break;
    case AST_FUNC: break;
    case AST_ARGLST: break;
    case AST_DECFUNC: break;

   



    default: fprintf(stderr, "ERROR: Unknown node type: %d\n", node->type);
        break;
    }
    return tacJoin(tacJoin(tacJoin(code[0], code[1]), code[2]), code[3]);
}

//prototype
TAC* make_binop(int type, TAC* code0, TAC* code1);


//implementation
TAC* make_binop(int type, TAC* code0, TAC* code1) {
    return tacJoin(tacJoin(code0, code1), tacCreate(type, makeTemp(), code0?code0->res:0, code1?code1->res:0)); 
}