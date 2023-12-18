//Thiago Parisotto Dias
#include "semantic.h"
#include "ast.h"

int semanticErrors = 0;

void check_undeclared(AST *node){
    semanticErrors += checkUndeclaredHash();
}

void check_and_set_declarations(AST *node){
    if(node == NULL) {
        return;
    }
    switch(node->type){
        case AST_DECVAR:
            //check if it is arleady declared
            if(node->symbol->type != SYMBOL_IDENTIFIER){
                fprintf(stderr, "Semantic ERROR: Variable %s already declared\n", node->symbol->text);
                semanticErrors += 1;
            }
            else {
                node->symbol->type = SYMBOL_VAR;
            }
            break;
        case AST_DECVEC:
            //check if it's arleady declared
            if(node->symbol->type != SYMBOL_IDENTIFIER){
                fprintf(stderr, "Semantic ERROR: Vector %s already declared\n", node->symbol->text);
                semanticErrors += 1;
            }
            else {
                node->symbol->type = SYMBOL_VEC;
            }
            break;
        case AST_DECFUNC:
            //check if it's arleady declared
            if(node->symbol->type != SYMBOL_IDENTIFIER){
                fprintf(stderr, "Semantic ERROR: Function %s already declared\n", node->symbol->text);
                semanticErrors += 1;
            }
            else {
                node->symbol->type = SYMBOL_FUN;
            }
            break;
        default:
            break;
    }
    for(int i = 0; i < MAX_SONS; i++){
        check_and_set_declarations(node->son[i]);
    }
}

void check_usage(AST *node, AST *root) {
    if(node == NULL) {
        return;
    }
    switch(node->type) {
        case AST_ATTREXPR:
            if(node->symbol->type != SYMBOL_VAR) {
                fprintf(stderr, "Semantic ERROR: %s is not a variable\n", node->symbol->text);
                semanticErrors += 1;
            }
            break;
        case AST_ATTRVEC:
            if(node->symbol->type != SYMBOL_VEC) {
                fprintf(stderr, "Semantic ERROR: %s is not a vector\n", node->symbol->text);
                semanticErrors += 1;
            }
            break;
        case AST_FUNC:
            checkFunction(node, root);
            break;
        default:
            break;
    }
    for(int i = 0; i < MAX_SONS; i++) {
        check_usage(node->son[i], root);
    }
}

//checa se a funcao ja foi declarada e se o numero de parametros esta correto
void checkFunction(AST* node, AST* root) {
    AST* funcDec = findFunctionDeclaration(node->symbol->text, root);
    if(funcDec == 0) {
        fprintf(stderr, "Semantic ERROR: Function %s not declared\n", node->symbol->text);
        semanticErrors += 1;
    } else {
        if(countParams(node->son[0]) != countParams(funcDec->son[1])) {
            fprintf(stderr, "Semantic ERROR: Function %s called with wrong number of parameters\n", node->symbol->text);
            semanticErrors += 1;
        } else {
            compareArguments(node->son[0], funcDec->son[1]);
        }
    }
    return;
}

//comparar parametros
void compareArguments (AST* node1, AST* node2) {
    if(node1 == 0 && node2 == 0) {
        return;
    }
    if(node1 == 0 || node2 == 0) {
        fprintf(stderr, "Semantic ERROR: Function called with wrong type of parameters\n");
        semanticErrors += 1;
        return;
    }
    if(node1->son[0]->symbol->type != node2->son[0]->symbol->type) {
        fprintf(stderr, "Semantic ERROR: Function called with wrong type of parameters\n");
        semanticErrors += 1;
        return;
    }
    compareArguments(node1->son[1], node2->son[1]);
}

//checar o numero de parametros
int countParams(AST* node) {
    if(node == 0) {
        return 0;
    }
    return 1 + countParams(node->son[1]);
}

//recebe a root como parametro, percorre a lista e procura a declaracao da função dada por text
AST * findFunctionDeclaration(char * text, AST* node) {
    if(node == NULL) {
        return 0;
    }
    if(node->type == AST_DECFUNC && strcmp(node->symbol->text, text) == 0) {
        return node;
    }
    AST* result = 0;
    for(int i = 0; i < MAX_SONS; i++) {
        result = findFunctionDeclaration(text, node->son[i]);
        if(result != 0) {
            return result;
        }
    }
    return 0;
}

//checa se os operandos são do mesmo tipo
void check_operands(AST* node){
    if(node == 0) {
        return;
    }
    switch(node->type){
        case AST_ADD:
            if(node->son[0]->symbol->type != SYMBOL_LIT_INT && node->son[0]->symbol->type != SYMBOL_LIT_FLOAT){
                fprintf(stderr, "Semantic ERROR: Operand of wrong type ADD\n");
                semanticErrors += 1;
                break;
            }
            if(node->son[1]->symbol->type != SYMBOL_LIT_INT && node->son[1]->symbol->type != SYMBOL_LIT_FLOAT){
                fprintf(stderr, "Semantic ERROR: Operand of wrong type ADD\n");
                semanticErrors += 1;
            }
            break;
        case AST_SUB:
            if(node->son[0]->symbol->type != SYMBOL_LIT_INT && node->son[0]->symbol->type != SYMBOL_LIT_FLOAT){
                fprintf(stderr, "Semantic ERROR: Operand of wrong type SUB\n");
                semanticErrors += 1;
                break;
            }
            if(node->son[1]->symbol->type != SYMBOL_LIT_INT && node->son[1]->symbol->type != SYMBOL_LIT_FLOAT){
                fprintf(stderr, "Semantic ERROR: Operand of wrong type SUB\n");
                semanticErrors += 1;
            }
            break;
        case AST_MUL:
            if(node->son[0]->symbol->type != SYMBOL_LIT_INT && node->son[0]->symbol->type != SYMBOL_LIT_FLOAT){
                    fprintf(stderr, "Semantic ERROR: Operand of wrong type MUL\n");
                    semanticErrors += 1;
                    break;
                }
            if(node->son[1]->symbol->type != SYMBOL_LIT_INT && node->son[1]->symbol->type != SYMBOL_LIT_FLOAT){
                    fprintf(stderr, "Semantic ERROR: Operand of wrong type MUL\n");
                    semanticErrors += 1;
                }
            break;
        case AST_DIV:
            if(node->son[0]->symbol->type != SYMBOL_LIT_INT && node->son[0]->symbol->type != SYMBOL_LIT_FLOAT){
                    fprintf(stderr, "Semantic ERROR: Operand of wrong type DIV\n");
                    semanticErrors += 1;
                    break;
                }
            if(node->son[1]->symbol->type != SYMBOL_LIT_INT && node->son[1]->symbol->type != SYMBOL_LIT_FLOAT){
                    fprintf(stderr, "Semantic ERROR: Operand of wrong type DIV\n");
                    semanticErrors += 1;
                }
            break;
        case AST_LESS:
        break;
        case AST_GREATER:
        break;
        case AST_LE:
        break;
        case AST_GE:
        break;
        case AST_EQ:
        break;
        case AST_AND:
        break;
        case AST_OR:
            if(node->son[0]->symbol->type != node->son[1]->symbol->type){
                fprintf(stderr, "Semantic ERROR: Operands of different types OR\n");
                semanticErrors += 1;
            }
            break;
        case AST_NOT:
            if(node->son[0]->symbol->type != SYMBOL_LIT_INT){
                fprintf(stderr, "Semantic ERROR: Operand of wrong type NOT\n");
                semanticErrors += 1;
            }
            break;
        case AST_ATTREXPR:
            if(node->symbol->type != SYMBOL_VAR){
                fprintf(stderr, "Semantic ERROR: Assignment to non-variable\n");
                semanticErrors += 1;
            }
            break;
        case AST_ATTRVEC:
            if(node->son[0]->symbol->type != SYMBOL_LIT_INT){
                fprintf(stderr, "Semantic ERROR: Vector index must be an integer\n");
                semanticErrors += 1;
            }
            if(node->son[1]->symbol->type != SYMBOL_LIT_INT){
                fprintf(stderr, "Semantic ERROR: Vector index must be an integer\n");
                semanticErrors += 1;
            }
            if(node->son[0]->symbol->type != node->son[1]->symbol->type){
                fprintf(stderr, "Semantic ERROR: Operands of different types\n");
                semanticErrors += 1;
            }
            break;
        case AST_INPUT:
            if(node->son[0]->symbol->type != SYMBOL_VAR){
                fprintf(stderr, "Semantic ERROR: Read operand must be a variable\n");
                semanticErrors += 1;
            }
            break;
        case AST_PRINT:
            if(node->son[0]->symbol->type != SYMBOL_LIT_INT && node->son[0]->symbol->type != SYMBOL_LIT_STRING){
                fprintf(stderr, "Semantic ERROR: Print operand must be an integer, boolean or string\n");
                semanticErrors += 1;
            }
            break;
        case AST_IF:
            if(node->son[0]->symbol->type != SYMBOL_LIT_INT){
                fprintf(stderr, "Semantic ERROR: If condition must be a boolean\n");
                semanticErrors += 1;
            }
            break;
        default:
            break;
    }
    for(int i = 0; i < MAX_SONS; i++){
        check_operands(node->son[i]);
    }
}   