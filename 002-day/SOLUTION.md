
Homework:
=========

The assigned parser is defined by parse.c.

Extra credit:
=============

Right associative binary operators:
-----------------------------------

So my first solution to this problem was to simply manipulate the AST until it
had the desired structure. This approach came to mind because I was already
doing AST manipulation to accomplish the left associativity. Doing this is
ugly, but it could certainly be worse. I went ahead and committed this work so
that it can be viewed in the future, but there is a much better solution.

The preferred way to implement this is to simply make the parsing function a
recursive one. For the 'expon' production in the grammar, instead of using EBNF
and the 'repeat' syntax like the other rules, you can simply make the rule
right recursive. Right recursion is easily handled by a recursive descent
parser, and it results in right associativity.

Parenthesized expressions:
--------------------------
TODO

Factoring out repeated parsing code by using a table:
-----------------------------------------------------
TODO

Possible uses for a table driven approach:
------------------------------------------
TODO
