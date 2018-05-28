
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
This is actually a pretty simple one too. Just modify the grammar and
accompanying code to allow the 'unary' production to loop back to the 'add'
non-terminal, like so.

unary -> '(' add ')';

Now subexpressions within parentheses will remain pushed down in the AST and
will therefore be evaluated before their parent nodes.

Another way to think about it is that you are simply taking subexpressions
within parentheses, evaluating them in isolation, and then taking that result
and putting it back into the original expression.

Factoring out repeated parsing code by using a table:
-----------------------------------------------------
This is now incorporated into the parser via the function 'parse_using_table'.
It is definitely a little clunky but it satisfies the requirements.

I was surprised just how similar the parsing logic was for the left and right
associative binary operators.

Possible uses for a table driven approach:
------------------------------------------
TODO
