//Thiago Parisotto Dias
#ifndef SEMANTIC_H
#define SEMANTIC_H

#include "ast.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

void check_and_set_declarations(AST *node, AST *root);
void check_undeclared(AST *node);
void check_usage(AST *node, AST *root);
void check_operands(AST *node);
void check_misc(AST *node);
int get_total_semantic_errors();
void check_and_set_nodes(AST *node);

int checkIsCompatible(int dataType1, int dataType2);
int checkIsLogical(int dataType);
int checkIsRelational(int dataType);
int checkIsArithmetic(int dataType);

void set_function_implementation(char *text, AST *node);
void check_function(AST *node, AST *root);
AST *find_function_declaration(char *text, AST *node);
int count_params(AST *node);
void compare_arguments(AST *node1, AST *node2);
void check_return_compatibility(AST *node, int dataType);
#endif
//end of file