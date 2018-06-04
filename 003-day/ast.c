
#include <stdlib.h>

#include "ast.h"

AST_Node *create_ast_node(Token token) {
    AST_Node *node;

    node = malloc(sizeof(*node));

    node->kind = token.kind;
    node->val = token.val;
    node->left = NULL;
    node->right = NULL;

    return node;
}

void print_ast(AST_Node *node) {
    if ((*node).kind == TOKEN_INT) {
        printf("%lu", (*node).val);
        return;
    } else {
        printf("(");
        switch((*node).kind) {
            case TOKEN_MINUS:
                printf("- ");
                break;
            case TOKEN_TILDE:
                printf("~ ");
                break;
            case TOKEN_STAR:
                printf("* ");
                break;
            case TOKEN_SLASH:
                printf("/ ");
                break;
            case TOKEN_PERCENT:
                printf("%% ");
                break;
            case TOKEN_DOUBLE_LT:
                printf("<< ");
                break;
            case TOKEN_DOUBLE_GT:
                printf(">> ");
                break;
            case TOKEN_EXPON:
                printf("** ");
                break;
            case TOKEN_AMPERSAND:
                printf("& ");
                break;
            case TOKEN_PLUS:
                printf("+ ");
                break;
            case TOKEN_PIPE:
                printf("| ");
                break;
            case TOKEN_CARET:
                printf("^ ");
                break;
            case TOKEN_EOF:
                printf("EOF ");
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
