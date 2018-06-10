
#ifndef PARSER_H
#define PARSER_H

#include <stdint.h>
#include <ctype.h>

#include "ast.h"

typedef enum TokenKind {
    TOKEN_INT = 128,
    TOKEN_MINUS,
    TOKEN_TILDE,
    TOKEN_STAR,
    TOKEN_SLASH,
    TOKEN_PERCENT,
    TOKEN_DOUBLE_LT,
    TOKEN_DOUBLE_GT,
    TOKEN_AMPERSAND,
    TOKEN_PLUS,
    TOKEN_PIPE,
    TOKEN_CARET,
    TOKEN_DOUBLE_STAR,
    TOKEN_LEFT_PAREN,
    TOKEN_RIGHT_PAREN,
    TOKEN_EOF,
    // ...
} TokenKind;

typedef struct Token {
    TokenKind kind;
    const char *start;
    const char *end;
    union {
        uint64_t val;
    };
} Token;

Token token;
const char *stream;

void next_token();

AST_Node *parse_unary();
AST_Node *parse_expon();
AST_Node *parse_mult();
AST_Node *parse_add();
AST_Node *parse_expression();

#endif
