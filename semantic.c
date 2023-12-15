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
        fprintf(stderr, "Semantic ERROR: Function called with wrong number of parameters\n");
        semanticErrors += 1;
        return;
    }
        fprintf(stderr, "Comparing arguments\n");

    if(node1->son[0]->symbol->type != node2->son[0]->symbol->type) {
        fprintf(stderr, "type1 %d, type2 %d\n", node1->son[0]->symbol->type, node2->son[0]->symbol->type);
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
//recebe a root como parametro, percorre a lista e procura a declaracao text
AST * findFunctionDeclaration(char * text, AST* node) {
    if(node == NULL) {
        return 0;
    }
    if(node->type == AST_DECFUNC && strcmp(node->symbol->text, text) == 0) {
        fprintf(stderr, "Found function %s\n", text);
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