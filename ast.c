#include "ast.h"
extern int getLineNumber();

AST* astCreate(int type, HASH_NODE* symbol,AST* son0, AST* son1, AST* son2, AST* son3) {
    AST* newnode;
    newnode = (AST*) calloc(1,sizeof(AST));
    newnode->type = type;
    newnode->symbol = symbol;
    newnode->son[0] = son0;
    newnode->son[1] = son1;
    newnode->son[2] = son2;
    newnode->son[3] = son3;
    newnode->lineNumber = getLineNumber();
    return newnode;
}


//print all levels of the ast tree based on the current node, and give it spaces for each level
void astPrint(AST* node, int level) {
    if (node == 0) return;
    int i;
    for (i = 0; i < level; ++i) fprintf(stderr,"  ");
    switch (node->type) {
        case AST_SYMBOL: fprintf(stderr, "AST_SYMBOL"); break;
        case AST_ADD: fprintf(stderr, "AST_ADD"); break;
        case AST_SUB: fprintf(stderr, "AST_SUB"); break;
        case AST_MUL: fprintf(stderr, "AST_MUL"); break;
        case AST_DIV: fprintf(stderr, "AST_DIV"); break;
        case AST_LESS: fprintf(stderr, "AST_LESS"); break;
        case AST_GREATER: fprintf(stderr, "AST_GREATER"); break;
        case AST_LE: fprintf(stderr, "AST_LE"); break;
        case AST_GE: fprintf(stderr, "AST_GE"); break;
        case AST_EQ: fprintf(stderr, "AST_EQ"); break;
        case AST_DIF: fprintf(stderr, "AST_DIF"); break;
        case AST_AND: fprintf(stderr, "AST_AND"); break;
        case AST_OR: fprintf(stderr, "AST_OR"); break;
        case AST_NOT: fprintf(stderr, "AST_NOT"); break;
        case AST_LDEC: fprintf(stderr, "AST_LDEC"); break;
        case AST_DECVAR: fprintf(stderr, "AST_DECVAR"); break;
        case AST_DECVEC: fprintf(stderr, "AST_DECVEC"); break;
        case AST_DECFUNC: fprintf(stderr, "AST_DECFUNC"); break;
        case AST_PARAM: fprintf(stderr, "AST_PARAM"); break;
        case AST_BLOCK: fprintf(stderr, "AST_BLOCK"); break;
        case AST_CMDLST: fprintf(stderr, "AST_CMDLST"); break;
        case AST_PRINT: fprintf(stderr, "AST_PRINT"); break;
        case AST_RETURN: fprintf(stderr, "AST_RETURN"); break;
        case AST_IF: fprintf(stderr, "AST_IF"); break;
        case AST_ELSE: fprintf(stderr, "AST_ELSE"); break;
        case AST_WHILE: fprintf(stderr, "AST_WHILE"); break;
        case AST_INPUT: fprintf(stderr, "AST_INPUT"); break;
        case AST_CODE: fprintf(stderr, "AST_CODE"); break;
        case AST_TYPEINT: fprintf(stderr, "AST_TYPEINT"); break;
        case AST_TYPEFLOAT: fprintf(stderr, "AST_TYPEFLOAT"); break;
        case AST_TYPECHAR: fprintf(stderr, "AST_TYPECHAR"); break;
        case AST_VECLST: fprintf(stderr, "AST_VECLST"); break;
        case AST_PARAMINIT: fprintf(stderr, "AST_PARAMINIT"); break;
        case AST_PARAMLST: fprintf(stderr, "AST_PARAMLST"); break;
        case AST_LCODE: fprintf(stderr, "AST_LCODE"); break;
        //case AST_ATTR: fprintf(stderr, "AST_ATTR"); break;
        case AST_PARENTESES: fprintf(stderr, "AST_PARENTESES"); break;
        case AST_ARGLST: fprintf(stderr, "AST_ARGLST"); break;
        case AST_ARGLSTEND: fprintf(stderr, "AST_ARGLSTEND"); break;
        case AST_VEC: fprintf(stderr, "AST_VEC"); break;
        case AST_FUNC: fprintf(stderr, "AST_FUNC"); break;
        case AST_ATTREXPR: fprintf(stderr, "AST_ATTREXPR"); break;
        case AST_ATTRVEC: fprintf(stderr, "AST_ATTRVEC"); break;
        case AST_VECEND: fprintf(stderr, "AST_VECEND"); break;
        case AST_PROG: fprintf(stderr, "AST_PROG"); break;
        case AST_LDECINIT: fprintf(stderr, "AST_LDECINIT"); break;
        case AST_PRINTEXP: fprintf(stderr, "AST_PRINTEXP"); break;

        default: fprintf(stderr, "UNKNOWN"); break;
    }
    if(node->symbol != NULL) {
        fprintf(stderr,", %s\n", node->symbol->text);
    }
    else {
        fprintf(stderr,"\n");
    }
    for(i = 0; i < MAX_SONS; ++i) astPrint(node->son[i], level+1);
}

void uncompile(AST* node, FILE* output) {
    
    if (node == 0) return;
    int i;
    switch (node->type) {
        case AST_SYMBOL: 
            fprintf(output, "%s", node->symbol->text); 
            break;
        case AST_ADD: 
            uncompile(node->son[0], output);
            fprintf(output, " + ");
            uncompile(node->son[1], output);
            break;
        case AST_SUB: 
            uncompile(node->son[0], output);
            fprintf(output, " - ");
            uncompile(node->son[1], output);
            break;
        case AST_MUL: 
            uncompile(node->son[0], output);
            fprintf(output, " * ");
            uncompile(node->son[1], output);
            break;
        case AST_DIV: 
            uncompile(node->son[0], output);
            fprintf(output, " / ");
            uncompile(node->son[1], output);
            break;
        case AST_LESS: 
            uncompile(node->son[0], output);
            fprintf(output, " < ");
            uncompile(node->son[1], output);
            break;
        case AST_GREATER:
            uncompile(node->son[0], output);
            fprintf(output, " > ");
            uncompile(node->son[1], output);
            break;
        case AST_LE:
            uncompile(node->son[0], output);
            fprintf(output, " <= ");
            uncompile(node->son[1], output);
            break;
        case AST_GE:
            uncompile(node->son[0], output);
            fprintf(output, " >= ");
            uncompile(node->son[1], output);
            break;
        case AST_EQ:    
            uncompile(node->son[0], output);
            fprintf(output, " == ");
            uncompile(node->son[1], output);
            break;
        case AST_DIF:
            uncompile(node->son[0], output);
            fprintf(output, " != ");
            uncompile(node->son[1], output);
            break;
        case AST_AND:
            uncompile(node->son[0], output);
            fprintf(output, " && ");
            uncompile(node->son[1], output);
            break;
        case AST_OR:
            uncompile(node->son[0], output);
            fprintf(output, " || ");
            uncompile(node->son[1], output);
            break;
        case AST_NOT:
            fprintf(output, "!");
            uncompile(node->son[0], output);
            break;
        case AST_LDEC:
            uncompile(node->son[0], output);
            uncompile(node->son[1], output);
            break;
        case AST_DECVAR:
            uncompile(node->son[0], output);
            fprintf(output, " %s = ", node->symbol->text);
            uncompile(node->son[1], output);
            fprintf(output, ";\n");
            break;
        case AST_DECVEC:
            uncompile(node->son[0], output);
            fprintf(output, " %s[", node->symbol->text);
            uncompile(node->son[1], output);
            fprintf(output, "]");
            uncompile(node->son[2], output);
            fprintf(output, ";\n");
            break;
        case AST_DECFUNC:
            uncompile(node->son[0], output);
            fprintf(output, " %s(", node->symbol->text);
            uncompile(node->son[1], output);
            fprintf(output, ")");
            uncompile(node->son[2], output);
            fprintf(output, ";\n");
            break;
        case AST_PARAM:
            uncompile(node->son[0], output);
            fprintf(output, " %s", node->symbol->text);
            uncompile(node->son[1], output);
            break;
        case AST_BLOCK:
            fprintf(output, "{\n");
            uncompile(node->son[0], output);
            fprintf(output, "}\n");
            break;
        case AST_CMDLST:
            uncompile(node->son[0], output);
            uncompile(node->son[1], output);
            break;
        case AST_PRINT:
            fprintf(output, "print ");
            uncompile(node->son[0], output);
            fprintf(output, ";\n");
            break;
        case AST_RETURN:
            fprintf(output, "return ");
            uncompile(node->son[0], output);
            fprintf(output, ";\n");
            break;
        case AST_IF:
            fprintf(output, "if (");
            uncompile(node->son[0], output);
            fprintf(output, ")\n");
            uncompile(node->son[1], output);
            break;
        case AST_ELSE:
            fprintf(output, "else \n");
            uncompile(node->son[0], output);
            break;
        case AST_WHILE:
            fprintf(output, "while (");
            uncompile(node->son[0], output);
            fprintf(output, ")\n");
            uncompile(node->son[1], output);
            break;
        case AST_INPUT:
            fprintf(output, "input (");
            uncompile(node->son[0], output);
            fprintf(output, ");\n");
            break;
        case AST_CODE:
            uncompile(node->son[0], output);
            break;
        case AST_TYPEINT:
            fprintf(output, "int");
            break;
        case AST_TYPEFLOAT:
            fprintf(output, "float");
            break;
        case AST_TYPECHAR:
            fprintf(output, "char");
            break;
        case AST_VECLST:
            fprintf(output, " ");
            uncompile(node->son[0], output);
            uncompile(node->son[1], output);
            break;
        case AST_PARAMINIT:
            uncompile(node->son[0], output);
            uncompile(node->son[1], output);
            break;
        case AST_PARAMLST:
            fprintf(output, ", ");
            uncompile(node->son[0], output);
            uncompile(node->son[1], output);
            break;
        case AST_LCODE:
            fprintf(output, "code ");
            fprintf(output, "%s", node->symbol->text);
            uncompile(node->son[0], output);
            uncompile(node->son[1], output);
            break;
        // case AST_ATTR:
        //     uncompile(node->son[0], output);
        //     fprintf(output, " = ");
        //     uncompile(node->son[1], output);
        //     fprintf(output, ";\n");
        //     break;
        case AST_PARENTESES:
            fprintf(output, "(");
            uncompile(node->son[0], output);
            fprintf(output, ")");
            break;
        case AST_ARGLST:
            uncompile(node->son[0], output);
            uncompile(node->son[1], output);
            break;
        case AST_ARGLSTEND:
            fprintf(output, ",");
            uncompile(node->son[0], output);
            uncompile(node->son[1], output);
            break;
        case AST_VEC:
            fprintf(output, "%s[", node->symbol->text);
            uncompile(node->son[0], output);
            fprintf(output, "]");
            break;
        case AST_FUNC:
            fprintf(output, "%s(", node->symbol->text);
            uncompile(node->son[0], output);
            fprintf(output, ")");
            break;
        case AST_ATTREXPR:
            fprintf(output, "%s = ", node->symbol->text);
            uncompile(node->son[0], output);
            fprintf(output, ";\n");
            break;
        case AST_ATTRVEC:
            fprintf(output, "%s[", node->symbol->text);
            uncompile(node->son[0], output);
            fprintf(output, "] = ");
            uncompile(node->son[1], output);
            fprintf(output, ";\n");
            break;
        case AST_VECEND:
            uncompile(node->son[0], output);
            break;
        case AST_PROG:
            uncompile(node->son[0], output);
            break;
        case AST_LDECINIT:
            uncompile(node->son[0], output);
            uncompile(node->son[1], output);
            break;
        case AST_PRINTEXP:
        fprintf(output, "print ");
        uncompile(node->son[0], output);
        fprintf(output, ";\n");
        break;

        default: fprintf(output, "UNKNOWN"); break;
    }
}