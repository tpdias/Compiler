/*Thiago Parisotto Dias*/

#include "tacs.h"


//prototype
TAC* make_binop(int type, TAC* code0, TAC* code1);
TAC* make_if(TAC* code0, TAC* code1, TAC* code2);
TAC* make_while(TAC* code0, TAC* code1);
TAC* make_func(TAC* code0, TAC* params, TAC* symbol);

char *opcodeNames[] = { "TAC_SYMBOL", 
    "TAC_MOVE", "TAC_ADD", "TAC_SUB", "TAC_MUL",
    "TAC_DIV", "TAC_LABEL", "TAC_BEGINFUN", "TAC_ENDFUN",
    "TAC_IFZ", "TAC_JUMP", "TAC_CALL", "TAC_ARG",
    "TAC_RET", "TAC_PRINT", "TAC_READ", "TAC_LESS",
    "TAC_GREATER", "TAC_LE", "TAC_GE", "TAC_EQ", "TAC_DIF",
    "TAC_AND", "TAC_OR", "TAC_NOT", "TAC_ATTRVEC", 
    "TAC_INPUT", "TAC_RETURN", "TAC_VEC", "TAC_PARAM",
    "TAC_PRINTCHAR"
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
    if(tac->opcode == TAC_SYMBOL) return;
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

void tacPrint(TAC *tac) {
    if (!tac) return;
    tacPrintSingle(tac);
    tacPrint(tac->next);
}

TAC* revertTac(TAC* tac) {
    TAC* temp;
    for(temp = tac; temp->prev; temp = temp->prev) {
        temp->prev->next = temp;
    }
    return temp;
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
    if(!node) return 0;

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
    case AST_ATTREXPR: return tacJoin(code[0], tacCreate(TAC_MOVE, node->symbol, code[0]?code[0]->res:0, 0)); break;
    case AST_ATTRVEC: return tacJoin(code[0], tacJoin(code[1], tacCreate(TAC_ATTRVEC, node->symbol, code[0]?code[0]->res:0, code[1]?code[1]->res:0))); break;
    case AST_INPUT: return tacJoin(code[0], tacCreate(TAC_INPUT, makeTemp(), 0, 0)); break; 
    case AST_PRINT: 
            if(node->son[0]->type == AST_TYPECHAR){
                return tacJoin(tacJoin(code[0], tacCreate(TAC_PRINTCHAR, code[0]?code[0]->res:0, 0, 0)), code[1]); break;
            }
            return tacJoin(tacJoin(code[0], tacCreate(TAC_PRINT, code[0]?code[0]->res:0, 0, 0)), code[1]);
        break;
    case AST_PRINTEXP: return tacJoin(tacJoin(code[0], tacCreate(TAC_PRINT, code[0]?code[0]->res:0, 0, 0)), code[1]); break;
    case AST_RETURN: return tacJoin(code[0], tacCreate(TAC_RETURN, code[0]?code[0]->res:0, 0, 0)); break;
    case AST_IF: return make_if(code[0], code[1], code[2]); break;
    case AST_ELSE: return make_if(code[0], code[1], code[2]); break;
    case AST_WHILE: return make_while(code[0], code[1]); break;
    case AST_LCODE: return tacJoin(tacJoin(tacCreate(TAC_BEGINFUN, node->symbol, 0, 0), code[0]), code[1]); break;
    case AST_FUNC: return tacJoin(code[0], tacCreate(TAC_CALL, makeTemp(), node->symbol, 0)); break;
    case AST_ARGLSTEND: return tacJoin(code[1], tacJoin(code[0], tacCreate(TAC_ARG, code[0]?code[0]->res:0, 0, 0))); break;
    case AST_ARGLST: return tacJoin(code[1], tacJoin(code[0], tacCreate(TAC_ARG, code[0]?code[0]->res:0, 0, 0))); break;
    // case AST_DECFUNC: return make_func(tacCreate(TAC_SYMBOL, node->symbol, 0, 0), code[1], code[2]); break;
    case AST_VEC: return tacJoin(code[0], tacCreate(TAC_VEC, makeTemp(), node->symbol, 0)); break;
    default: 
        break;
    }
    return tacJoin(tacJoin(tacJoin(code[0], code[1]), code[2]), code[3]);
}


//implementation
TAC* make_binop(int type, TAC* code0, TAC* code1) {
    return tacJoin(tacJoin(code0, code1), tacCreate(type, makeTemp(), code0?code0->res:0, code1?code1->res:0)); 
}

TAC* make_if(TAC* code0, TAC* code1, TAC* code2) {
    TAC* newTac;
    HASH_NODE* newLabel = makeLabel();
    TAC* ifTac = tacCreate(TAC_IFZ, newLabel, code0?code0->res:0, 0);
    TAC* labelTac = tacCreate(TAC_LABEL, newLabel, 0, 0);

    if(!code2) {
        newTac = tacJoin(tacJoin(ifTac, code1), labelTac);
    } else {
        //caso em que tem else
        HASH_NODE* elseLabel = makeLabel();
        TAC* jumpTac = tacCreate(TAC_JUMP, elseLabel, 0, 0);
        TAC* elseLabelTac = tacCreate(TAC_LABEL, elseLabel, 0, 0);
        newTac = tacJoin(tacJoin(tacJoin(tacJoin(tacJoin(ifTac, code1), jumpTac), labelTac), code2), elseLabelTac);
    }
    return newTac;
}

TAC* make_while(TAC* code0, TAC* code1) {
    /*  TAC LABEL
        TAC IFZ
        TAC CODE0
        TAC JUMP
        TAC LABEL
        TAC CODE1
    */
    TAC* newTac;
    HASH_NODE* newLabelLoop = makeLabel();
    HASH_NODE* newLabelJump = makeLabel();
    TAC* whileTac = tacCreate(TAC_IFZ, newLabelJump, code0?code0->res:0, 0);
    TAC* labelTacLoop = tacCreate(TAC_LABEL, newLabelLoop, 0, 0);
    TAC* jumpTac = tacCreate(TAC_JUMP, newLabelLoop, 0, 0);
    TAC* labelTacJump = tacCreate(TAC_LABEL, newLabelJump, 0, 0);
    newTac = tacJoin(tacJoin(tacJoin(tacJoin(tacJoin(labelTacLoop, code0), whileTac), code1), jumpTac), labelTacJump);
    return newTac;
}

TAC* make_func(TAC* symbol, TAC* params, TAC* code0) {
    /*  TAC BEGINFUN
        TAC CODE0
        TAC ENDFUN
    */
    TAC* newTac;
    newTac = tacJoin(tacJoin(tacJoin(tacCreate(TAC_BEGINFUN, symbol->res, 0, 0), params), code0), tacCreate(TAC_ENDFUN, symbol->res, 0, 0));
    return newTac;
}