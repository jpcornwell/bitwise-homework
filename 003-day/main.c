

#include "ast.h"
#include "parser.h"

int main(int argc, char **argv) {
    char *source = "12*34 + 45/56 + ~~25 ** 2 ** 3";
    stream = source;

    AST_Node *tree;

    // tree = parse_expression();
    tree = parse_expression();
    print_ast(tree);
    printf("\n");

    return 0;
}
