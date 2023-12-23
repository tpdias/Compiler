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
                fprintf(stderr, "Semantic ERROR Line %d: Variable %s already declared\n", node->lineNumber, node->symbol->text);
                semanticErrors += 1;
            }
            else {
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
                        node->symbol->datatype = DATATYPE_CHAR; // devo colocar int?
                    }
                }
            }
            break;
        default:
            break;
    }
    for(int i = 0; i < MAX_SONS; i++){
        check_and_set_declarations(node->son[i]);
    }
}

void set_node_types(AST *node) {
    if(node == NULL) {
        return;
    }
    for(int i = 0; i < MAX_SONS; i++) {
        set_node_types(node->son[i]);
    }
    if(node->type == AST_SYMBOL){
        if(node->symbol->type == SYMBOL_VEC || node->symbol->type == SYMBOL_FUN){
            fprintf(stderr, "Semantic ERROR Line %d: %s is not a scalar\n", node->lineNumber, node->symbol->text);
            semanticErrors += 1;
        } 
        node->datatype = node->symbol->datatype;
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
void compare_arguments (AST* node1, AST* node2) {
    if(node1 == 0 && node2 == 0) {
        return;
    }
    if(node1 == 0 || node2 == 0) {
        fprintf(stderr, "Semantic ERROR Line %d: Function called with wrong type of parameters\n", node1->lineNumber);
        semanticErrors += 1;
        return;
    }
    if(node1->son[0]->symbol->type != node2->son[0]->symbol->type) {
        fprintf(stderr, "Semantic ERROR Line %d: Function called with wrong type of parameters\n", node1->lineNumber);
        semanticErrors += 1;
        return;
    }
    compare_arguments(node1->son[1], node2->son[1]);
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

//checa se os operandos são do mesmo tipo
void check_operands(AST* node){
    if(node == 0) {
        return;
    }
    switch(node->type){
        case AST_ADD:
            if(node->son[0]->symbol->type != SYMBOL_LIT_INT && node->son[0]->symbol->type != SYMBOL_LIT_CHAR){
                fprintf(stderr, "Semantic ERROR Line %d: Operand of wrong type ADD\n, node->lineNumber", node->lineNumber);
                semanticErrors += 1;
                break;
            }
            if(node->son[1]->symbol->type != SYMBOL_LIT_INT && node->son[1]->symbol->type != SYMBOL_LIT_CHAR){
                fprintf(stderr, "Semantic ERROR Line %d: Operand of wrong type ADD\n", node->lineNumber);
                semanticErrors += 1;
            }
            break;
        case AST_SUB:
            if(node->son[0]->symbol->type != SYMBOL_LIT_INT && node->son[0]->symbol->type != SYMBOL_LIT_FLOAT){
                fprintf(stderr, "Semantic ERROR Line %d: Operand of wrong type SUB\n", node->lineNumber);
                semanticErrors += 1;
                break;
            }
            if(node->son[1]->symbol->type != SYMBOL_LIT_INT && node->son[1]->symbol->type != SYMBOL_LIT_FLOAT){
                fprintf(stderr, "Semantic ERROR Line %d: Operand of wrong type SUB\n", node->lineNumber);
                semanticErrors += 1;
            }
            break;
        case AST_MUL:
            if(node->son[0]->symbol->type != SYMBOL_LIT_INT && node->son[0]->symbol->type != SYMBOL_LIT_FLOAT){
                    fprintf(stderr, "Semantic ERROR Line %d: Operand of wrong type MUL\n", node->lineNumber);
                    semanticErrors += 1;
                    break;
                }
            if(node->son[1]->symbol->type != SYMBOL_LIT_INT && node->son[1]->symbol->type != SYMBOL_LIT_FLOAT){
                    fprintf(stderr, "Semantic ERROR Line %d: Operand of wrong type MUL\n", node->lineNumber);
                    semanticErrors += 1;
                }
            break;
        case AST_DIV:
            if(node->son[0]->symbol->type != SYMBOL_LIT_INT && node->son[0]->symbol->type != SYMBOL_LIT_FLOAT){
                    fprintf(stderr, "Semantic ERROR Line %d: Operand of wrong type DIV\n", node->lineNumber);
                    semanticErrors += 1;
                    break;
                }
            if(node->son[1]->symbol->type != SYMBOL_LIT_INT && node->son[1]->symbol->type != SYMBOL_LIT_FLOAT){
                    fprintf(stderr, "Semantic ERROR Line %d: Operand of wrong type DIV\n", node->lineNumber);
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
                fprintf(stderr, "Semantic ERROR Line %d: Operands of different types OR\n", node->lineNumber);
                semanticErrors += 1;
            }
            break;
        case AST_NOT:
            if(node->son[0]->symbol->type != SYMBOL_LIT_INT){
                fprintf(stderr, "Semantic ERROR Line %d: Operand of wrong type NOT\n", node->lineNumber);
                semanticErrors += 1;
            }
            break;
        case AST_ATTREXPR:
            if(node->symbol->type != SYMBOL_VAR){
                fprintf(stderr, "Semantic ERROR Line %d: Assignment to non-variable\n", node->lineNumber);
                semanticErrors += 1;
            }
            break;
        case AST_ATTRVEC:
            if(node->son[0]->symbol->type != SYMBOL_LIT_INT){
                fprintf(stderr, "Semantic ERROR Line %d: Vector index must be an integer\n", node->lineNumber);
                semanticErrors += 1;
            }
            if(node->son[1]->symbol->type != SYMBOL_LIT_INT){
                fprintf(stderr, "Semantic ERROR Line %d: Vector index must be an integer\n", node->lineNumber);
                semanticErrors += 1;
            }
            if(node->son[0]->symbol->type != node->son[1]->symbol->type){
                fprintf(stderr, "Semantic ERROR Line %d: Operands of different types\n", node->lineNumber);
                semanticErrors += 1;
            }
            break;
        case AST_INPUT:
            if(node->son[0]->symbol->type != SYMBOL_VAR){
                fprintf(stderr, "Semantic ERROR Line %d: Read operand must be a variable\n", node->lineNumber);
                semanticErrors += 1;
            }
            break;
        case AST_PRINT:
            if(node->son[0]->symbol->type != SYMBOL_LIT_INT && node->son[0]->symbol->type != SYMBOL_LIT_STRING){
                fprintf(stderr, "Semantic ERROR Line %d: Print operand must be an integer, boolean or string\n", node->lineNumber);
                semanticErrors += 1;
            }
            break;
        case AST_IF:
            if(node->son[0]->symbol->type != SYMBOL_LIT_INT){
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