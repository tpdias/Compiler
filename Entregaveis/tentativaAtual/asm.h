/*Thiago Parisotto Dias*/

#ifndef ASM_H
#define ASM_H
#include <stdio.h>
#include <stdlib.h>
#include "ast.h"
#include "tacs.h"

void generateASM(TAC* tac, AST* ast);
void allocateGlobalVariables(FILE* output, HASH_NODE** table);

void allocateTemp(FILE* output, HASH_NODE* node);
void allocateImediate(FILE* output, HASH_NODE* node);

void allocateAST(FILE* output, AST* node);
void allocateTAC(FILE* output, TAC* tac);

#endif