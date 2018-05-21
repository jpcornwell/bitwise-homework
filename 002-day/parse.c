#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdint.h>
#include <stddef.h>
#include <assert.h>
#include <stdlib.h>
#include <ctype.h>

#define MAX(x, y) ((x) >= (y) ? (x) : (y))

void *xrealloc(void *ptr, size_t num_bytes) {
    ptr = realloc(ptr, num_bytes);
    if (!ptr) {
        perror("xrealloc failed");
        exit(1);
    }
    return ptr;
}

void *xmalloc(size_t num_bytes) {
    void *ptr = malloc(num_bytes);
    if (!ptr) {
        perror("malloc failed");
        exit(1);
    }
    return ptr;
}

// stretchy buffers

typedef struct BufHdr {
    size_t len;
    size_t cap;
    char buf[0];
} BufHdr;

#define buf__hdr(b) ((BufHdr *)((char *)b - offsetof(BufHdr, buf)))
#define buf__fits(b, n) (buf_len(b) + (n) <= buf_cap(b))
#define buf__fit(b, n) (buf__fits(b, n) ? 0 : ((b) = buf__grow((b), buf_len(b) + (n), sizeof(*(b)))))

#define buf_len(b) ((b) ? buf__hdr(b)->len : 0)
#define buf_cap(b) ((b) ? buf__hdr(b)->cap : 0)
#define buf_push(b, x) (buf__fit(b, 1), b[buf_len(b)] = (x), buf__hdr(b)->len++)
#define buf_free(b) ((b) ? (free(buf__hdr(b)), (b) = NULL) : 0)

void *buf__grow(const void *buf, size_t new_len, size_t elem_size) {
    size_t new_cap = MAX(1 + 2*buf_cap(buf), new_len);
    assert(new_len <= new_cap);
    size_t new_size = offsetof(BufHdr, buf) + new_cap*elem_size;
    BufHdr *new_hdr;
    if (buf) {
        new_hdr = xrealloc(buf__hdr(buf), new_size);
    } else {
        new_hdr = xmalloc(new_size);
        new_hdr->len = 0;
    }
    new_hdr->cap = new_cap;
    return new_hdr->buf;
}

// lexing: translating char stream to token stream

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
    case '*':
        token.kind = TOKEN_STAR;
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

void print_token(Token token) {
    switch (token.kind) {
    case TOKEN_INT:
        printf("TOKEN INT: %lu\n", token.val);
        break;
    default:
        printf("TOKEN '%c': %.*s\n", token.kind, (int)(token.end - token.start),
                token.start);
        break;
    }
}

void lex_test() {
    char *source = "12*34 + 45/56 + ~25";
    stream = source;
    while (next_token()) {
        print_token(token);
    }
}

int main(int argc, char **argv) {
    lex_test();
    return 0;
}
