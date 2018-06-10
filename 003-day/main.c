
#include <stdlib.h>
#include <string.h>

#include "ast.h"
#include "parser.h"

void set_input_stream(char *source) {
    stream = source;
}

void test_s_expression(char *expected) {
    AST_Node *tree;
    char s_expr[1000] = "";

    tree = parse_expression();
    get_ast_s_expr(tree, s_expr);
    if (strcmp(s_expr, expected) != 0) {
        printf("S-expression test failed\nExpected: %s\nActual: %s\n",
                expected, s_expr);
        exit(1);
    }
}

void run_test_suite() {
    set_input_stream("1 + 2");
    test_s_expression("(+ 1 2)");

    set_input_stream("1+2");
    test_s_expression("(+ 1 2)");

    set_input_stream("1 + 2 * 3");
    test_s_expression("(+ 1 (* 2 3))");

    set_input_stream("2 ** 3 ** 4");
    test_s_expression("(** 2 (** 3 4))");

    set_input_stream("5 + ~2");
    test_s_expression("(+ 5 (~ 2))");
}

int main(int argc, char **argv) {
    run_test_suite();
    printf("Tests completed successfully\n");

    return 0;
}
