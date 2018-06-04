
#ifndef PARSER_H
#define PARSER_H

#include <stdint.h>
#include <ctype.h>

#include "common.h"
#include "ast.h"

Token token;
const char *stream;

int next_token();

AST_Node *parse_unary();
AST_Node *parse_expon();
AST_Node *parse_mult();
AST_Node *parse_add();
AST_Node *parse_expression();

#endif
