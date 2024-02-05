#include "asm.h"
#include <string.h>
#include "hash.h"

void allocateTemp(FILE* output, HASH_NODE* node) {
    fprintf(output, 
                    "\t.globl _%s\n"
                    "\t.p2align	2\n"
                    "\t.data\n"
                    "_%s:\n"
                    "\t.long 0\n\n", 
                    node->text, node->text);
}

void allocateImediate(FILE* output, HASH_NODE* node) {
    if(node->type == SYMBOL_LIT_CHAR) {
            fprintf(output, "\t.globl _%d\n"
                            "\t.p2align	2\n"
                            "_%d:\n"
                            "\t.long %d\n\n", 
                            node->text[1], node->text[1], node->text[1]); 
        }
        else {
            fprintf(output, "\t.globl _%s\n"
                            "\t.p2align	2\n"
                            "_%s:\n"
                            "\t.long %s\n\n", 
                            node->text, node->text, node->text); 
        }
    
}

void allocateGlobalVariables(FILE* output, HASH_NODE** table) {
     for(int i = 0; i < HASH_SIZE; i++) {
        if(table[i] == 0) continue;
        for(HASH_NODE* node = table[i]; node != 0; node = node->next) {
            if(strncmp(node->text, "__temp", 6) == 0) {
               allocateTemp(output, node);
            } else {
                if(node->type == SYMBOL_LIT_INT || node->type == SYMBOL_LIT_FLOAT || node->type == SYMBOL_LIT_CHAR) {
                    allocateImediate(output, node);
                }
            }
        }
    }
}

void allocateAST(FILE* output, AST* node) {
    static int LC = 2;
    if(node == 0){
        return;
    }

    switch (node->type)
    {
    case AST_SYMBOL:
        if(node->symbol->type == SYMBOL_LIT_STRING){
            fprintf(output, "\t.section\t .data\n.LC%d:\n\t.string %s \n", LC, node->symbol->text);
            LC++;          
        }
        break;
    case AST_DECVAR:
        if(node->son[0]->type == AST_TYPECHAR) {
            fprintf(output, "\t.globl _%s\n"
                            "\t.p2align	2\n"
                            "_%s:\n"
                            "\t.long %d\n\n", node->symbol->text, node->symbol->text, node->son[1]->symbol->text[1]); 
        }
        else {
            fprintf(output, "\t.globl _%s\n"
                            "\t.p2align	2\n"
                            "_%s:\n"
                            "\t.long %s\n\n", node->symbol->text, node->symbol->text, node->son[1]->symbol->text); 
        }
        break;
    case AST_PARAM:
        fprintf(output, "\t.globl _%s\n"
                        "\t.p2align	2\n"
                       "\t.data\n"
                       "_%s:\n"
                       "\t.long 0\n", node->symbol->text, node->symbol->text);
        break;
        case AST_DECVEC:
            break;
        default:
            break;
    }
    
    for(int i = 0; i < MAX_SONS; ++ i) {
        allocateAST(output, node->son[i]);
    }
}


void generateASM(TAC* tac, AST* ast) {
    HASH_NODE** table = getTable();
    FILE *output;
    output = fopen("asm.s", "w");
    if(output == 0) {
        fprintf(stderr, "Error: Could not open output file\n");
        exit(1);
    }

    fprintf(output, "\t.section\t__DATA,__data\n");
    allocateGlobalVariables(output, table);
    
    fprintf(output, ".LC0:\n"
		         "\t.string	\"%%d\"\n");
    fprintf(output, ".LC1:\n"
                 "\t.string	\"%%f\"\n");
    allocateAST(output, ast);

    fprintf(output, "\t.globl _main\n"
                    "\t.p2align	2\n"
                    "_main:\n"
                    "\t.cfi_startproc\n"
                    "; %%bb.0:\n"
                    "\tsub	sp, sp, #16\n"
                    "\t.cfi_def_cfa_offset 16\n"); 
    allocateTAC(output, tac);
     fprintf(output, "\tadd	sp, sp, #16\n"
                    "\tret\n"
                    "\t.cfi_endproc\n");
    fclose(output);
}

void allocateTAC(FILE* output, TAC* root) {
    static int LC = 0, BL = 0, SP = 0;
    for(TAC* tac = root; tac != 0; tac = tac->next) {
        switch (tac->opcode)
        {
        case TAC_MOVE:
            fprintf(output, ";TAC_MOVE\n"
                            "\tadrp	x8, _%s@PAGE\n"
                            "\tldr	w8, [x8, _%s@PAGEOFF]\n"
                            "\tadrp	x9, _%s@PAGE\n"
                            "\tstr	w8, [x9, _%s@PAGEOFF]\n", tac->op1->text, tac->op1->text, tac->res->text, tac->res->text);     
            break;
        case TAC_ADD:
            fprintf(output, ";TAC_ADD\n"
                            "\tadrp	x8, _%s@PAGE\n"
                            "\tldr	w8, [x8, _%s@PAGEOFF]\n"
                            "\tadrp	x9, _%s@PAGE\n"
                            "\tldr	w9, [x9, _%s@PAGEOFF]\n"
                            "\tadd	w8, w8, w9\n"
                            "\tadrp	x9, _%s@PAGE\n"
                            "\tstr	w8, [x9, _%s@PAGEOFF]\n"
                            "\tadd	sp, sp, #16\n",
                             tac->op1->text, tac->op1->text, tac->op2->text, tac->op2->text, tac->res->text, tac->res->text);
            break;
        case TAC_PRINT:
        fprintf(output, "\taqui\n");
        break;

        // case TAC_BEGINFUN:
        //     fprintf(output, "_%s\n"
        //                     "\t.cfi_startproc\n"
        //                     "; %bb.0:\n"
        //                     "\tsub	sp, sp, #16\n"
        //                     "\t.cfi_def_cfa_offset 16\n",
        //                         tac->res->text);      
        //     break;
        // case TAC_ENDFUN:
        //     fprintf(output, "\tadd	sp, sp, #16\n"
        //                     "\tret\n"
        //                     "\t.cfi_endproc\n");
           // break;
        default:
            break;
        }
    }
}