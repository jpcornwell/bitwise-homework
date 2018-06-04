
#ifndef COMMON_H
#define COMMON_H

#include <stdint.h>

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
    TOKEN_EXPON,
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

#endif
