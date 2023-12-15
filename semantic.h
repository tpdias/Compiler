//Thiago Parisotto Dias
#ifndef SEMANTIC_H
#define SEMANTIC_H

#include "ast.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

void check_and_set_declarations(AST *node);
void check_undeclared(AST *node);
void check_usage(AST *node, AST *root);
void check_operands(AST *node);
void check_misc(AST *node);

void checkFunction(AST *node, AST *root);
AST *findFunctionDeclaration(char *text, AST *node);
int countParams(AST *node);
void compareArguments(AST *node1, AST *node2);
#endif
//end of file