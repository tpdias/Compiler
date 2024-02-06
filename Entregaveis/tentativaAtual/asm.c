#include "asm.h"
#include <string.h>
#include "hash.h"

void allocateTemp(FILE* output, HASH_NODE* node) {
    fprintf(output, 
                    "\t.globl _%s\n"
                    "\t	.p2align	2, 0x0\n"
                    "_%s:\n"
                    "\t.long 0\n\n", 
                    node->text, node->text);
}

void allocateImediate(FILE* output, HASH_NODE* node) {
    if(node->type == SYMBOL_LIT_CHAR) {
            fprintf(output, "\t.globl _%d\n"
                            "_%d:\n"
                            "\t.long %d\n\n", 
                            node->text[1], node->text[1], node->text[1]); 
        }
        else {
            fprintf(output, "\t.globl _%s\n"
                            "\t	.p2align	2, 0x0\n"
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
    if(node == 0){
        return;
    }

    switch (node->type)
    {
    case AST_DECVAR:
        if(node->son[0]->type == AST_TYPECHAR) {
            fprintf(output, "\t.globl _%s\n"
                            "_%s:\n"
                            "\t.long %d\n\n", node->symbol->text, node->symbol->text, node->son[1]->symbol->text[1]); 
        }
        else {
            fprintf(output, "\t.globl _%s\n"
                            "\t	.p2align	2, 0x0\n"
                            "_%s:\n"
                            "\t.long %s\n\n", node->symbol->text, node->symbol->text, node->son[1]->symbol->text); 
        }
        break;
    case AST_PARAM:
        fprintf(output, "\t.globl _%s\n"
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

   

    fprintf(output, "	.section	__TEXT,__text,regular,pure_instructions\n\n"
                    "\t.globl _main\n"
                    "_main:\n"
                    "\t.cfi_startproc\n"
                    "; %%bb.0:\n"
                    "\t sub	sp, sp, #32\n"
                    "\t.cfi_def_cfa_offset 32\n"
                     "\tstp	x29, x30, [sp, #16]             ; 16-byte Folded Spill\n"
                     "\tadd	x29, sp, #16\n"
                     "\t.cfi_def_cfa w29, 16\n"
                     "\t.cfi_offset w30, -8\n"
                     "\t.cfi_offset w29, -16\n"
                     "\tmov	w8, #0\n"
                     "\tstr	w8, [sp, #8]                    ; 4-byte Folded Spill\n"
                     "\tstur	wzr, [x29, #-4]\n");
                    // "\tmov	w0, #0\n"
	                // "\tstr	wzr, [sp, #12]\n"); 
    

    
    allocateTAC(output, tac);
    

    fprintf(output, 	"\tldr	w0, [sp, #8]                    ; 4-byte Folded Reload\n"
                    "\tldp	x29, x30, [sp, #16]             ; 16-byte Folded Reload\n"
                    "\tadd	sp, sp, #32\n"
                    "\tret\n"
                    "\t.cfi_endproc\n\n");

    fprintf(output, "\t.section	__DATA,__data\n");
    allocateGlobalVariables(output, table);
    allocateAST(output, ast);   

    //for prints
    fprintf(output, "\t.section	__TEXT,__cstring,cstring_literals\n"
                    "\tl_.str.int:                                 ; @.str\n"
	                ".asciz	\"%%d\\n\"\n"
                    "\tl_.str.char:                                 ; @.str\n"
                    ".asciz	\"%%c\\n\"\n");
    fclose(output);
}

void allocateTAC(FILE* output, TAC* root) {        
    for(TAC* tac = root; tac != 0; tac = tac->next) {
        switch (tac->opcode)
        {
        case TAC_MOVE:
            if(tac->op1->type == SYMBOL_LIT_CHAR) {
                fprintf(output, ";MOVE\n"
                            "\tadrp	x8, _%d@PAGE\n"
                            "\tldr	w8, [x8, _%d@PAGEOFF]\n"
                            "\tadrp	x9, _%s@PAGE\n"
                            "\tstr	w8, [x9, _%s@PAGEOFF]\n", tac->op1->text[1], tac->op1->text[1], tac->res->text, tac->res->text);     
            } else {
                fprintf(output, ";MOVE\n"
                            "\tadrp	x8, _%s@PAGE\n"
                            "\tldr	w8, [x8, _%s@PAGEOFF]\n"
                            "\tadrp	x9, _%s@PAGE\n"
                            "\tstr	w8, [x9, _%s@PAGEOFF]\n", tac->op1->text, tac->op1->text, tac->res->text, tac->res->text);     
            }
            break;
        case TAC_ADD:
            fprintf(output, ";ADD\n"
                            "\tadrp	x8, _%s@PAGE\n"
                            "\tldr	w8, [x8, _%s@PAGEOFF]\n"
                            "\tadrp	x9, _%s@PAGE\n"
                            "\tldr	w9, [x9, _%s@PAGEOFF]\n"
                            "\tadd	w8, w8, w9\n"
                            "\tadrp	x9, _%s@PAGE\n"
                            "\tstr	w8, [x9, _%s@PAGEOFF]\n",
                             tac->op1->text, tac->op1->text, tac->op2->text, tac->op2->text, tac->res->text, tac->res->text);
            break;
        case TAC_PRINT:
            fprintf(output, 
                            ";PRINT\n"
                            
                            "\tadrp	x8, _%s@PAGE\n"
                            "\tldr	w9, [x8, _%s@PAGEOFF]\n"
                            "\t\t; implicit-def: $x8\n"
                            "\tmov	x8, x9\n"
                            "\tmov	x9, sp\n"
                            "\tstr	x8, [x9]\n"
                            "\tadrp	x0, l_.str.int@PAGE\n"
                            "\tadd	x0, x0, l_.str.int@PAGEOFF\n"
                            "\tbl	_printf\n"
                            // "\tldr	w0, [sp, #8]                    ; 4-byte Folded Reload\n"
                            , tac->res->text, tac->res->text);

        break;
        case TAC_PRINTCHAR:
            fprintf(output, 
                            ";PRINT CHAR\n"
                            // "\tsub	sp, sp, #32\n"
                            // "\t.cfi_def_cfa_offset 32\n"
                            // "\tstp	x29, x30, [sp, #16]             ; 16-byte Folded Spill\n"
                            // "\tadd	x29, sp, #16\n"
                            // "\t.cfi_def_cfa w29, 16\n"
                            // "\t.cfi_offset w30, -8\n"
                            // "\t.cfi_offset w29, -16\n"
                            // "\tmov	w8, #0\n"
                            // "\tstr	w8, [sp, #8]                    ; 4-byte Folded Spill\n"
                            // "\tstur	wzr, [x29, #-4]\n"
                            "\tadrp	x8, _%s@PAGE\n"
                            "\tldr	w9, [x8, _%s@PAGEOFF]\n"
                            "\t\t; implicit-def: $x8\n"
                            "\tmov	x8, x9\n"
                            "\tmov	x9, sp\n"
                            "\tstr	x8, [x9]\n"
                            "\tadrp	x0, l_.str.char@PAGE\n"
                            "\tadd	x0, x0, l_.str.char@PAGEOFF\n"
                            "\tbl	_printf\n", tac->res->text, tac->res->text);
                            // "\tldr	w0, [sp, #8]                    ; 4-byte Folded Reload\n"
                            // "\tldp	x29, x30, [sp, #16]             ; 16-byte Folded Reload\n"
                            // "\tadd	sp, sp, #32\n", tac->res->text, tac->res->text);
        break;
        case TAC_INPUT:
            fprintf(output, ";INPUT\n"
                            "\tadrp	x8, _%s@PAGE\n"
                            "\tldr	x8, [x8, _%s@PAGEOFF]\n"
                            "\tstr	x8, [x9]\n"
                            "\tadrp	x0, l_.str.int@PAGE\n"
                            "\tadd	x0, x0, l_.str.int@PAGEOFF\n"
                            "\tbl	_scanf\n"
                            , tac->res->text, tac->res->text);
            break;
        // case TAC_ARG:
        //     fprintf(output, ";TAC_ARG\n"
        //                     "\tadrp	x8, _%s@PAGE\n"
        //                     "\tldr	w8, [x8, _%s@PAGEOFF]\n"
        //                     "\tadrp	x9, _%s@PAGE\n"
        //                     "\tstr	w8, [x9, _%s@PAGEOFF]\n", tac->op1->text, tac->op1->text, tac->res->text, tac->res->text);
        //     break;
        // case TAC_BEGINFUN:
        //     fprintf(output, ";TAC_BEGINFUN\n"
        //                     "\tsub	sp, sp, #16\n"
        //                     "\t.cfi_def_cfa_offset 16\n");
        //     break;
        // case TAC_PARAM:
        //     fprintf(output, ";TAC_PARAM\n"
        //                     "\tadrp	x8, _%s@PAGE\n"
        //                     "\tldr	w8, [x8, _%s@PAGEOFF]\n"
        //                     "\tadrp	x9, _%s@PAGE\n"
        //                     "\tstr	w8, [x9, _%s@PAGEOFF]\n", tac->op1->text, tac->op1->text, tac->res->text, tac->res->text);
        //     break;
        // case TAC_ENDFUN:
        //     fprintf(output, ";TAC_ENDFUN\n"
        //                     "\tadd	sp, sp, #16\n"
        //                     "\tret\n"
        //                     "\t.cfi_endproc\n");
        //     break;
        default:
            break;
        }
    }
}