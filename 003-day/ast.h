
#ifndef AST_H
#define AST_H

#include <stdio.h>
#include <stdint.h>

typedef enum AstNodeKind {
    AST_NODE_INT,
    AST_NODE_SUBTRACT,
    AST_NODE_COMPLEMENT,
    AST_NODE_MULT,
    AST_NODE_DIVIDE,
    AST_NODE_MOD,
    AST_NODE_SHIFT_LEFT,
    AST_NODE_SHIFT_RIGHT,
    AST_NODE_AND,
    AST_NODE_ADD,
    AST_NODE_OR,
    AST_NODE_XOR,
    AST_NODE_EXPON,
} AstNodeKind;

typedef struct AST_Node {
    AstNodeKind kind;
    struct AST_Node *left;
    struct AST_Node *right;
    union {
        uint64_t val;
    };
} AST_Node;

AST_Node *create_ast_node(AstNodeKind, uint64_t);

void print_ast(AST_Node *node);

#endif
