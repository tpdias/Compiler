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
int get_total_semantic_errors();
void set_node_types(AST *node);

void check_function(AST *node, AST *root);
AST *find_function_declaration(char *text, AST *node);
int count_params(AST *node);
void compare_arguments(AST *node1, AST *node2);
void check_return_compatibility(AST *node, int dataType);
#endif
//end of file