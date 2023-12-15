//Thiago Parisotto Dias
#ifndef SEMANTIC_H
#define SEMANTIC_H

#include "ast.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

void check_and_set_declarations(AST *node);
void check_undeclared(AST *node);
void check_usage(AST *node);
void check_operands(AST *node);
void check_misc(AST *node);

#endif
//end of file