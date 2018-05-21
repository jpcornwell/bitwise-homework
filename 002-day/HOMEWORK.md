Page 9 of the Wirth book has a typo:

    E = T | A "+" T

should be

    E = T | E "+" T

Read chapter 2 through 4.1 (12 pages) of Wirth's Compiler Construction book:
https://www.inf.ethz.ch/personal/wirth/CompilerConstruction

Implement an expression parser for a simple arithmetic language:

Parse an infix expression composed of integer literals and the
following operators, highest to lowest precedence:

    unary -, unary ~    (right associative)
    * / % << >> &       (left associative)
    + - | ^             (left associative)

Output an [S-expression](https://en.wikipedia.org/wiki/S-expression) that corresponds to the parse tree, e.g.

    12*34 + 45/56 + ~25

should generate an S-expression that looks like this

    (+ (+ (* 12 34) (/ 45 56)) (~ 25))

Extra credit:

How would you support right associative binary operators like ** (exponentiation)?

How would you support parenthesized expressions for explicit grouping?

While still using recursive descent, factor out the repetitive structure so
that the parsing for operators is driven by table information for what
operators exist and their precedence and associativity.

How might you use this to implement a language that supports user defined
operator symbols with user defined precedence and associativity?
