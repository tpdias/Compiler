//Thiago Parisotto Dias
#include "semantic.h"
#include "ast.h"
#include <stdlib.h>

int semanticErrors = 0;

void check_undeclared(AST *node){
    semanticErrors += checkUndeclaredHash();
}

void check_and_set_declarations(AST *node, AST *root){
    if(node == NULL) {
        return;
    }
    switch(node->type){
        case AST_DECVAR:
            //check if it is arleady declared
            if(node->symbol->type != SYMBOL_IDENTIFIER){
                fprintf(stderr, "Semantic ERROR Line %d: Variable %s already declared\n", node->lineNumber, node->symbol->text);
                semanticErrors += 1;
            }
            else {
                if(node->son[0]->type == AST_TYPEINT) {
                    node->symbol->datatype = DATATYPE_INT;
                } else {
                    if(node->son[0]->type == AST_TYPEFLOAT) {
                        node->symbol->datatype = DATATYPE_FLOAT;
                    }
                    else{
                        node->symbol->datatype = DATATYPE_CHAR; 
                    }
                }
                node->symbol->type = SYMBOL_VAR;
            }
            break;
        case AST_DECVEC:
            //check if it's arleady declared
            if(node->symbol->type != SYMBOL_IDENTIFIER){
                fprintf(stderr, "Semantic ERROR Line %d: Vector %s already declared\n", node->lineNumber, node->symbol->text);
                semanticErrors += 1;
            }
            else {
                if(node->son[0]->type == AST_TYPEINT) {
                    node->symbol->datatype = DATATYPE_INT;
                } else {
                    if(node->son[0]->type == AST_TYPEFLOAT) {
                        node->symbol->datatype = DATATYPE_FLOAT;
                    }
                    else{
                        node->symbol->datatype = DATATYPE_CHAR; 
                    }
                }
                node->symbol->type = SYMBOL_VEC;
            }
            break;
        case AST_DECFUNC:
            //check if it's arleady declared
            if(node->symbol->type != SYMBOL_IDENTIFIER){
                fprintf(stderr, "Semantic ERROR Line %d: Function %s already declared\n", node->lineNumber, node->symbol->text);
                semanticErrors += 1;
            }
            else {
                node->symbol->type = SYMBOL_FUN;
                if(node->son[0]->type == AST_TYPEINT) {
                    node->symbol->datatype = DATATYPE_INT;
                } else {
                    if(node->son[0]->type == AST_TYPEFLOAT) {
                        node->symbol->datatype = DATATYPE_FLOAT;
                    }
                    else{
                        node->symbol->datatype = DATATYPE_CHAR;
                    }
                }
            }
            set_function_implementation(node->symbol->text, root);
            if(node->symbol->isImplemented > 1) {
                fprintf(stderr, "Semantic ERROR Line %d: Function %s already implemented\n", node->lineNumber, node->symbol->text);
                semanticErrors += 1;
            } else if(node->symbol->isImplemented == 0) {
                fprintf(stderr, "Semantic ERROR Line %d: Function %s not implemented\n", node->lineNumber, node->symbol->text);
                semanticErrors += 1;
            }
            break;
            case AST_PARAM:
            //check if it's arleady declared
            if(node->symbol->type != SYMBOL_IDENTIFIER){
                fprintf(stderr, "Semantic ERROR Line %d: Parameter %s already declared\n", node->lineNumber, node->symbol->text);
                semanticErrors += 1;
            }
            else {
                node->symbol->type = SYMBOL_PARAM;
                if(node->son[0]->type == AST_TYPEINT) {
                    node->symbol->datatype = DATATYPE_INT;
                } else {
                    if(node->son[0]->type == AST_TYPEFLOAT) {
                        node->symbol->datatype = DATATYPE_FLOAT;
                    }
                    else{
                        node->symbol->datatype = DATATYPE_CHAR; 
                    }
                }
                node->symbol->type = SYMBOL_VAR;
            }
            break;
        case AST_INPUT:
            fprintf(stderr, "input %d\n", node->son[0]->type);
            if(node->son[0]->type == AST_TYPEINT) {
                node->datatype = DATATYPE_INT;
            } else {
                if(node->son[0]->type == AST_TYPEFLOAT) {
                    node->datatype = DATATYPE_FLOAT;
                }
                else{
                    node->datatype = DATATYPE_CHAR; 
                }
            }
            break;
        default:
            break;
    }
    for(int i = 0; i < MAX_SONS; i++){
        check_and_set_declarations(node->son[i], root);
    }
}

void check_and_set_nodes(AST *node) {
    if(node == NULL) {
        return;
    }
    for(int i = 0; i < MAX_SONS; i++) {
        check_and_set_nodes(node->son[i]);
    }
    if(node->type == AST_SYMBOL){
        if(node->symbol->type == SYMBOL_VEC || node->symbol->type == SYMBOL_FUN){
            fprintf(stderr, "Semantic ERROR Line %d: %s is not a scalar\n", node->lineNumber, node->symbol->text);
            semanticErrors += 1;
        } 
        node->datatype = node->symbol->datatype;
    } else if(node->type == AST_FUNC || node->type == AST_VEC) {
        node->datatype = node->symbol->datatype;
    } else if(node->type == AST_PARENTESES) {
        node->datatype = node->son[0]->datatype;
    } else if(checkIsArithmetic(node->type)) {
        AST *son0 = node->son[0];
        AST *son1 = node->son[1];
        if(checkIsCompatible(son0->datatype, son1->datatype)) {
            node->datatype = son0->datatype;
        } else {
            fprintf(stderr, "Semantic ERROR Line %d: Incompatible types\n", node->lineNumber);
            semanticErrors += 1;
        }
    } else if(checkIsRelational(node->type)) {
        node->datatype = DATATYPE_BOOL;
    } else if(checkIsLogical(node->type)) {
        node->datatype = DATATYPE_BOOL;
    }


}

void check_usage(AST *node, AST *root) {
    if(node == NULL) {
        return;
    }
    switch(node->type) {
        case AST_ATTREXPR:
            if(node->symbol->type != SYMBOL_VAR) {
                fprintf(stderr, "Semantic ERROR Line %d: %s is not a variable\n", node->lineNumber, node->symbol->text);
                semanticErrors += 1;
            }
            break;
        case AST_ATTRVEC:
            if(node->symbol->type != SYMBOL_VEC) {
                fprintf(stderr, "Semantic ERROR Line %d: %s is not a vector\n", node->lineNumber, node->symbol->text);
                semanticErrors += 1;
            }
            break;
        case AST_FUNC:
            check_function(node, root);
            break;
        default:
            break;
    }
    for(int i = 0; i < MAX_SONS; i++) {
        check_usage(node->son[i], root);
    }
}

//checa se a funcao ja foi declarada e se o numero de parametros esta correto
void check_function(AST* node, AST* root) {
    AST* funcDec = find_function_declaration(node->symbol->text, root);
    if(node->symbol->isImplemented < 1) {
        fprintf(stderr, "Semantic ERROR Line %d: Function %s not implemented\n", node->lineNumber, node->symbol->text);
        semanticErrors += 1;
    }
    if(funcDec == 0) {
        fprintf(stderr, "Semantic ERROR Line %d: Function %s not declared\n", node->lineNumber, node->symbol->text);
        semanticErrors += 1;
    } else {
        if(count_params(node->son[0]) != count_params(funcDec->son[1])) {
            fprintf(stderr, "Semantic ERROR Line %d: Function %s called with wrong number of parameters\n", node->lineNumber, node->symbol->text);
            semanticErrors += 1;
        } else {
            compare_arguments(node->son[0], funcDec->son[1]);
        }
    }
    return;
}

//comparar parametros
void compare_arguments (AST* funcCall, AST* declaration) {
    if(declaration == 0 && funcCall == 0) {
        return;
    }
    if(declaration == 0 || funcCall == 0) {
        fprintf(stderr, "Semantic ERROR Line %d: Function called with wrong type of parameters\n", declaration->lineNumber);
        semanticErrors += 1;
        return;
    }
    if(declaration->son[0]->symbol->datatype != funcCall->son[0]->symbol->datatype) {
        fprintf(stderr, "Semantic ERROR Line %d: Function called with wrong type of parameters\n", declaration->lineNumber);
        semanticErrors += 1;
        return;
    }
    compare_arguments(declaration->son[1], funcCall->son[1]);
}

//checar o numero de parametros
int count_params(AST* node) {
    if(node == 0) {
        return 0;
    }
    return 1 + count_params(node->son[1]);
}

//recebe a root como parametro, percorre a lista e procura a declaracao da função dada por text
AST * find_function_declaration(char * text, AST* node) {
    if(node == NULL) {
        return 0;
    }
    if(node->type == AST_DECFUNC && strcmp(node->symbol->text, text) == 0) {
        return node;
    }
    AST* result = 0;
    for(int i = 0; i < MAX_SONS; i++) {
        result = find_function_declaration(text, node->son[i]);
        if(result != 0) {
            return result;
        }
    }
    return 0;
}

void set_function_implementation(char *text, AST *node) {
    if(node == NULL) {
        return;
    }
    if(node->type == AST_LCODE && strcmp(node->symbol->text, text) == 0) {
        node->symbol->isImplemented += 1;
    }
    for(int i = 0; i < MAX_SONS; i++) {
        set_function_implementation(text, node->son[i]);
    }
}

//checa se os operandos são do mesmo tipo
void check_operands(AST* node){
    if(node == 0) {
        return;
    }
    if(checkIsArithmetic(node->type)) {
        if(checkIsCompatible(node->son[0]->datatype, node->son[1]->datatype) == 0) {
            fprintf(stderr, "Semantic ERROR Line %d: Operands of different types1\n", node->lineNumber);
            semanticErrors += 1;
        }
    } else if(checkIsLogical(node->type)) {
        if(node->son[0]->datatype != DATATYPE_BOOL || node->son[1]->datatype != DATATYPE_BOOL){
            fprintf(stderr, "Semantic ERROR Line %d: Operands of different types2\n", node->lineNumber);
            semanticErrors += 1;
        }
    } else if(checkIsRelational(node->type)) {
        if(checkIsCompatible(node->son[0]->datatype, node->son[1]->datatype) == 0){
            fprintf(stderr, "Semantic ERROR Line %d: Operands of different types\n", node->lineNumber);
            semanticErrors += 1;
        }
    } 

    switch(node->type){
        case AST_ATTREXPR:
            if(node->symbol->type != SYMBOL_VAR){
                fprintf(stderr, "Semantic ERROR Line %d: Assignment to non-variable\n", node->lineNumber);
                semanticErrors += 1;
            }
            if(node->son[0]->type == AST_INPUT){
                if(node->symbol->datatype != node->son[0]->datatype){
                    fprintf(stderr, "Semantic ERROR Line %d: Assignment of different types\n", node->lineNumber);
                    semanticErrors += 1;
                }
            } else {
                if(node->son[0]->datatype != node->symbol->datatype){
                    fprintf(stderr, "Semantic ERROR Line %d: Assignment of different types\n", node->lineNumber);
                    semanticErrors += 1;
                }
            }
            break;
        case AST_ATTRVEC:
            if(node->son[0]->symbol->datatype != DATATYPE_INT){
                fprintf(stderr, "%d, datatype\n", node->son[0]->symbol->datatype);
                fprintf(stderr, "Semantic ERROR Line %d: Vector index must be an integer\n", node->lineNumber);
                semanticErrors += 1;
            }
            if(node->son[1]->datatype != node->symbol->datatype){
                fprintf(stderr, "type: %d\n", node->son[1]->datatype);
                fprintf(stderr, "Semantic ERROR Line %d: Vector value wrong type\n", node->lineNumber);
                semanticErrors += 1;
            }
            break;
        case AST_INPUT:
            if(node->son[0]->type != AST_TYPEFLOAT && node->son[0]->type != AST_TYPECHAR && node->son[0]->type != AST_TYPEINT){
                fprintf(stderr, "Semantic ERROR Line %d: Input parameter must be a type (char, int or float)\n", node->lineNumber);
                semanticErrors += 1;
            }
            break;
        case AST_IF:
            if(checkIsCompatible(node->son[0]->datatype, DATATYPE_BOOL) == 0){
                fprintf(stderr, "Semantic ERROR Line %d: If condition must be a boolean\n", node->lineNumber);
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

void check_misc(AST* node) {
    if(node == 0) {
        return;
    }
    if(node->type == AST_LCODE){
        check_return_compatibility(node, node->symbol->datatype);
    }
    for(int i = 0; i < MAX_SONS; i++){
        check_misc(node->son[i]);
    }
}

void check_return_compatibility(AST *node, int datatype) {
    if(node == 0) {
        return;
    }
    if(node->type == AST_RETURN) {
        if(node->son[0]->datatype != datatype) {
            fprintf(stderr, "Semantic ERROR Line %d: Return type incompatible\n", node->lineNumber);
            semanticErrors += 1;
        }
    }
    for(int i = 0; i < MAX_SONS; i++){
        check_return_compatibility(node->son[i], datatype);
    }
}

int get_total_semantic_errors(){
    return semanticErrors;
}

int checkIsCompatible(int type1, int type2){
    if(type1 == type2){
        return 1;
    }
    if(type1 == DATATYPE_INT && type2 == DATATYPE_CHAR){
        return 1;
    }
    if(type1 == DATATYPE_CHAR && type2 == DATATYPE_INT){
        return 1;
    }
    return 0;
}

int checkIsRelational(int type){
    if(type == AST_LESS || type == AST_GREATER || type == AST_LE || type == AST_GE || type == AST_EQ || type == AST_DIF){
        return 1;
    }
    return 0;
}

int checkIsArithmetic(int type){
    if(type == AST_ADD || type == AST_SUB || type == AST_MUL || type == AST_DIV){
        return 1;
    }
    return 0;
}

int checkIsLogical(int type){
    if(type == AST_AND || type == AST_OR || type == AST_NOT){
        return 1;
    }
    return 0;
}