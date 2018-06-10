
#include <stdlib.h>
#include <string.h>

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

void get_ast_s_expr(AST_Node *node, char *buf) {
    if (node->kind == AST_NODE_INT) {
        sprintf(buf + strlen(buf), "%lu", node->val);
        return;
    } else {
        sprintf(buf + strlen(buf), "(");
        switch(node->kind) {
            case AST_NODE_SUBTRACT:
                sprintf(buf + strlen(buf), "- ");
                break;
            case AST_NODE_COMPLEMENT:
                sprintf(buf + strlen(buf), "~ ");
                break;
            case AST_NODE_MULT:
                sprintf(buf + strlen(buf), "* ");
                break;
            case AST_NODE_DIVIDE:
                sprintf(buf + strlen(buf), "/ ");
                break;
            case AST_NODE_MOD:
                sprintf(buf + strlen(buf), "%% ");
                break;
            case AST_NODE_SHIFT_LEFT:
                sprintf(buf + strlen(buf), "<< ");
                break;
            case AST_NODE_SHIFT_RIGHT:
                sprintf(buf + strlen(buf), ">> ");
                break;
            case AST_NODE_EXPON:
                sprintf(buf + strlen(buf), "** ");
                break;
            case AST_NODE_AND:
                sprintf(buf + strlen(buf), "& ");
                break;
            case AST_NODE_ADD:
                sprintf(buf + strlen(buf), "+ ");
                break;
            case AST_NODE_OR:
                sprintf(buf + strlen(buf), "| ");
                break;
            case AST_NODE_XOR:
                sprintf(buf + strlen(buf), "^ ");
                break;
            default:
                sprintf(buf + strlen(buf), "<op not recognized>");
                break;
        }
        if (node->left) {
            get_ast_s_expr(node->left, buf);
        }
        if (node->right) {
            sprintf(buf + strlen(buf), " ");
            get_ast_s_expr(node->right, buf);
        }
        sprintf(buf + strlen(buf), ")");
        return;
    }
}
