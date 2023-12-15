//Thiago Parisotto Dias
#include "semantic.h"
#include "ast.h"
#include <stdio.h>
#include <stdlib.h>

int semanticErrors = 0;
AST* root;
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

/*The use of the indentifiers should be compatible with its declaration, variables can only be used without indexation
vectors can only be used with indexation, functions should only be used with their calls, with parameters between parenteses
*/
void check_usage(AST *node) {
    if(node == NULL) {
        return;
    }
    switch(node->type) {
        case AST_ATTREXPR:
                        fprintf(stderr, "DEBUG: %s function\n", node->symbol->text);

            if(node->symbol->type != SYMBOL_VAR) {
                fprintf(stderr, "Semantic ERROR: %s is not a variable\n", node->symbol->text);
                semanticErrors += 1;
            }
            break;
        case AST_ATTRVEC:
            fprintf(stderr, "DEBUG: %s function\n", node->symbol->text);
            if(node->symbol->type != SYMBOL_VEC) {
                fprintf(stderr, "Semantic ERROR: %s is not a vector\n", node->symbol->text);
                semanticErrors += 1;
            }
            break;
        case AST_FUNC:
            checkFunction(node);
            break;
        default:
            break;
    }
    for(int i = 0; i < MAX_SONS; i++) {
        check_usage(node->son[i]);
    }
}

//checa se a funcao ja foi declarada e se o numero de parametros esta correto
void checkFunction(AST* node) {
    AST* funcDec = findFunctionDeclaration(node->symbol->text, root);
    if(funcDec == 0) {
        fprintf(stderr, "Semantic ERROR: Function %s not declared\n", node->symbol->text);
        semanticErrors += 1;
        return;
    }
    //checar o numero de parametros

}

//recebe a root como parametro, percorre a lista e procura a declaracao text
AST * findFunctionDeclaration(char * text, AST* node) {
    
}