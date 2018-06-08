
#include "ast.h"
#include "parser.h"

void set_input_stream(char *source) {
    stream = source;
}

void generate_s_expression() {
    AST_Node *tree;

    tree = parse_expression();
    print_ast(tree);
    printf("\n");
}

void run_test_suite() {
    set_input_stream("1 + 2");
    generate_s_expression();

    set_input_stream("1 + 2 * 3");
    generate_s_expression();
}

int main(int argc, char **argv) {
    run_test_suite();
    printf("Tests completed successfully\n");

    return 0;
}
