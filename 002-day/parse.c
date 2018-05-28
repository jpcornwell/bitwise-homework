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

typedef enum OpArity {
    OP_UNARY,
    OP_BINARY,
} OpArity;

typedef enum OpAssoc {
    ASSOC_LEFT,
    ASSOC_RIGHT,
} OpAssoc;

typedef struct OpEntry {
    TokenKind kind;
    OpArity arity;
    OpAssoc assoc;
} OpEntry;

#define OPS_PER_PREC_LEVEL 10
#define MAX_PREC_LEVEL 2

OpEntry entry_table[][OPS_PER_PREC_LEVEL] = {
    {
        {TOKEN_EXPON, OP_BINARY, ASSOC_RIGHT},
    },
    {
        {TOKEN_STAR, OP_BINARY, ASSOC_LEFT},
        {TOKEN_SLASH, OP_BINARY, ASSOC_LEFT},
        {TOKEN_PERCENT, OP_BINARY, ASSOC_LEFT},
        {TOKEN_DOUBLE_LT, OP_BINARY, ASSOC_LEFT},
        {TOKEN_DOUBLE_GT, OP_BINARY, ASSOC_LEFT},
        {TOKEN_AMPERSAND, OP_BINARY, ASSOC_LEFT},
    },
    {
        {TOKEN_PLUS, OP_BINARY, ASSOC_LEFT},
        {TOKEN_MINUS, OP_BINARY, ASSOC_LEFT},
        {TOKEN_PIPE, OP_BINARY, ASSOC_LEFT},
        {TOKEN_CARET, OP_BINARY, ASSOC_LEFT},
    },
};

int get_op_from_row(int prec_level, Token token, OpEntry *op) {

    for (int i = 0; i < OPS_PER_PREC_LEVEL; i++) {
        OpEntry a = entry_table[prec_level][i];
        if (a.kind == token.kind) {
            op->kind = a.kind;
            op->arity = a.arity;
            op->assoc = a.assoc;
            return 1;
        }
    }

    return 0;
}

typedef struct AST_Node {
    TokenKind kind;
    struct AST_Node *left;
    struct AST_Node *right;
    union {
        uint64_t val;
    };
} AST_Node;

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

AST_Node *parse_add();

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

AST_Node *parse_using_table(int prec_level) {
    AST_Node *node;
    AST_Node *left;
    AST_Node *right;
    OpEntry op;

    if (prec_level == -1) {
        if (token.kind == TOKEN_INT) {
            node = create_ast_node(token);
            next_token();
            return node;
        } else if (token.kind == TOKEN_LEFT_PAREN) {
            next_token();
            node = parse_using_table(MAX_PREC_LEVEL);
            if (token.kind == TOKEN_RIGHT_PAREN) {
                next_token();
                return node;
            } else {
                printf("Expected right parenthesis\n");
                exit(1);
            }
        }
    }

    node = parse_using_table(prec_level - 1);

    while (get_op_from_row(prec_level, token, &op)) {
        if (op.arity == OP_BINARY) {
            left = node;
            node = create_ast_node(token);
            next_token();
            if (op.assoc == ASSOC_LEFT) {
                right = parse_using_table(prec_level - 1);
            } else if (op.assoc == ASSOC_RIGHT) {
                right = parse_using_table(prec_level);
            }
            node->left = left;
            node->right = right;
        }
    }

    return node;
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

void print_tokens(Token *tokens) {
    for (size_t i = 0; i < buf_len(tokens); i++) {
        print_token(tokens[i]);
    }
}

int main(int argc, char **argv) {
//    char *source = "12*(34 + 45)/56 + 2 ** 3 ** 4";
    char *source = "(1 + 2) * 3 ** 4 ** 5";
    stream = source;

    AST_Node *tree;

    // tree = parse_expression();
    next_token();
    tree = parse_using_table(MAX_PREC_LEVEL);
    print_ast(tree);
    printf("\n");

    return 0;
}
