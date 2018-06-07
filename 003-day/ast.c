
#include <stdlib.h>

#include "ast.h"

AST_Node *create_ast_node(AstNodeKind kind, uint64_t val) {
    AST_Node *node;

    node = malloc(sizeof(*node));

    node->kind = kind;
    node->val = val;
    node->left = NULL;
    node->right = NULL;

    return node;
}

void print_ast(AST_Node *node) {
    if ((*node).kind == AST_NODE_INT) {
        printf("%lu", (*node).val);
        return;
    } else {
        printf("(");
        switch((*node).kind) {
            case AST_NODE_SUBTRACT:
                printf("- ");
                break;
            case AST_NODE_COMPLEMENT:
                printf("~ ");
                break;
            case AST_NODE_MULT:
                printf("* ");
                break;
            case AST_NODE_DIVIDE:
                printf("/ ");
                break;
            case AST_NODE_MOD:
                printf("%% ");
                break;
            case AST_NODE_SHIFT_LEFT:
                printf("<< ");
                break;
            case AST_NODE_SHIFT_RIGHT:
                printf(">> ");
                break;
            case AST_NODE_EXPON:
                printf("** ");
                break;
            case AST_NODE_AND:
                printf("& ");
                break;
            case AST_NODE_ADD:
                printf("+ ");
                break;
            case AST_NODE_OR:
                printf("| ");
                break;
            case AST_NODE_XOR:
                printf("^ ");
                break;
            default:
                printf("<op not recognized>");
                break;
        }
        if (node->left) {
            print_ast((*node).left);
        }
        if (node->right) {
            printf(" ");
            print_ast((*node).right);
        }
        printf(")");
        return;
    }
}
