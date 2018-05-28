
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

Possible ways to use a table driven approach:
---------------------------------------------
To allow the user to define their own operators with a designated arity,
precedence, and associativity would be an interesting challenge. The main
challenge would be getting the user supplied information to the code that
actually does the parsing.

For a compiled language, I guess you could place the information in a sort of
comment somewhere in the source code. Then the compiler would simply check to
see if any additional operators are defined by the user, add this to its own
operator table, and then use it during the parsing process.

I have a feeling that a lisp like language would have some sort of mind
boggling way of doing this at runtime, but I don't know a whole lot about lisps
yet. 

I guess any interpreted language could handle this though. Since interpreters
are executing statements/expressions one at a time, if the user can define
metadata which then modifies the behaviour of the interpreter (through some
sort of language built-in), then it wouldn't be too tough. You would just have
to ensure that the order of execution was correct. So in other words, the
metadata has to be defined and read in by the interpreter before it gets to the
code that contains a user defined operator.

The other thing though, is the actual behaviour of the operator would have to
be defined. How this could be handled would be a lot trickier. I imagine it
would be something rather like a function, where the user defines the logic,
and then the compiler/interpreter would either have to execute that logic
directly or figure out how to translate that to some other format which is then
what actually gets executed.
