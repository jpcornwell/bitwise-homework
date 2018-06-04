
#ifndef AST_H
#define AST_H

#include <stdio.h>

#include "common.h"

typedef struct AST_Node {
    TokenKind kind;
    struct AST_Node *left;
    struct AST_Node *right;
    union {
        uint64_t val;
    };
} AST_Node;

AST_Node *create_ast_node(Token);

void print_ast(AST_Node *node);

#endif
