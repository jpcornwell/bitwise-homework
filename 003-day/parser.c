#define _CRT_SECURE_NO_WARNINGS
#include <stdint.h>
#include <stddef.h>
#include <assert.h>
#include <stdlib.h>
#include <ctype.h>

#include "parser.h"

// lexer

int next_token() {
    int is_invalid_token = 0;
    while (*stream == ' ') {
        stream++;
    }
    token.start = stream;
    switch (*stream) {
    case '0':
    case '1':
    case '2':
    case '3':
    case '4':
    case '5':
    case '6':
    case '7':
    case '8':
    case '9': {
        uint64_t val = 0;
        while (isdigit(*stream)) {
            val *= 10;
            val += *stream++ - '0';
        }
        token.kind = TOKEN_INT;
        token.val = val;
        break;
    }
    case '-':
        token.kind = TOKEN_MINUS;
        stream++;
        break;
    case '~':
        token.kind = TOKEN_TILDE;
        stream++;
        break;
    case '/':
        token.kind = TOKEN_SLASH;
        stream++;
        break;
    case '%':
        token.kind = TOKEN_PERCENT;
        stream++;
        break;
    case '<':
        if (*(stream+1) == '<') {
            token.kind = TOKEN_DOUBLE_LT;
            stream += 2;
        } else {
            is_invalid_token = 1;
            token.kind = *stream++;
        }
        break;
    case '>':
        if (*(stream+1) == '>') {
            token.kind = TOKEN_DOUBLE_GT;
            stream++;
        } else {
            is_invalid_token = 1;
            token.kind = *stream++;
        }
        break;
    case '*':
        if (*(stream+1) == '*') {
            token.kind = TOKEN_EXPON;
            stream += 2;
        } else {
            token.kind = TOKEN_STAR;
            stream++;
        }
        break;
    case '&':
        token.kind = TOKEN_AMPERSAND;
        stream++;
        break;
    case '+':
        token.kind = TOKEN_PLUS;
        stream++;
        break;
    case '|':
        token.kind = TOKEN_PIPE;
        stream++;
        break;
    case '^':
        token.kind = TOKEN_CARET;
        stream++;
        break;
    case '(':
        token.kind = TOKEN_LEFT_PAREN;
        stream++;
        break;
    case ')':
        token.kind = TOKEN_RIGHT_PAREN;
        stream++;
        break;
    case '\0':
        token.kind = TOKEN_EOF;
        stream++;
        break;
    default:
        is_invalid_token = 1;
        token.kind = *stream++;
        break;
    }
    token.end = stream;

    if (is_invalid_token) {
        return 0;
    } else {
        return 1;
    }
}

// parser
//
// grammar:
// expression -> add;
// add        -> mult ( ('+' | '-' | '|' | '^') mult )*;
// mult       -> expon ( ('*' | '/' | '%' | '<<' | '>>' | '&') expon )*;
// expon      -> unary | (unary '**' expon);
// unary      -> ( '-' | '~' ) unary | INT | ( '(' add ')' );
//
// INT is just an integer number token
// Binary operators are left associative except for exponentiation which is
//   right associative

AST_Node *parse_unary() {
    AST_Node *node;

    if (token.kind == TOKEN_MINUS ||
        token.kind == TOKEN_TILDE) {
        node = create_ast_node(token);
        next_token();
        (*node).left = parse_unary();
        (*node).right = NULL;
    } else if (token.kind == TOKEN_INT) {
        node = create_ast_node(token);
        next_token();
    } else if (token.kind == TOKEN_LEFT_PAREN) {
        next_token();
        node = parse_add();
        if (token.kind == TOKEN_RIGHT_PAREN) {
            next_token();
        } else {
            printf("Expected right parenthesis\n");
            exit(1);
        }
    }
    return node;
}

AST_Node *parse_expon() {
    AST_Node *node;
    AST_Node *left;
    AST_Node *right;

    left = parse_unary();
    node = left;

    while (token.kind == TOKEN_EXPON) {
        node = create_ast_node(token);
        next_token();
        right = parse_expon();
        node->left = left;
        node->right = right;
    }

    return node;
}

AST_Node *parse_mult() {
    AST_Node *node;
    AST_Node *left;
    AST_Node *right;

    node = parse_expon();

    while (token.kind == TOKEN_STAR ||
           token.kind == TOKEN_SLASH ||
           token.kind == TOKEN_PERCENT ||
           token.kind == TOKEN_DOUBLE_LT ||
           token.kind == TOKEN_DOUBLE_GT ||
           token.kind == TOKEN_AMPERSAND) {
        left = node;
        node = create_ast_node(token);
        next_token();
        right = parse_expon();
        (*node).left = left;
        (*node).right = right;
    }

    return node;
}

AST_Node *parse_add() {
    AST_Node *node;
    AST_Node *left;
    AST_Node *right;

    node = parse_mult();

    while (token.kind == TOKEN_PLUS ||
           token.kind == TOKEN_MINUS ||
           token.kind == TOKEN_PIPE ||
           token.kind == TOKEN_CARET) {
        left = node;
        node = create_ast_node(token);
        next_token();
        right = parse_mult();
        (*node).left = left;
        (*node).right = right;
    }

    return node;
}

AST_Node *parse_expression() {
    AST_Node *tree;
    next_token();
    tree = parse_add();
    return tree;
}

