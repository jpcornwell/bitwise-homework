This is a multi-part exercise. I expect this to take a while for people to do,
so I won't write up new homework until people's feedback on their progress
indicates that most people have solved at least parts of it. In general, even
if you get stuck, working on the problem yourself means that when you see me
solve it on stream, you'll be ready to appreciate the solution in a way you
couldn't have been otherwise.

Remember to look up terms and concepts on Wikipedia/Google if they're
unfamiliar.

Building on the parser you wrote yesterday, implement a calculator by
recursively evaluating the expression as part of the recursive descent, instead
of outputting S-expressions. [I just did this on stream.]

This is an interpreter.

Implement a bytecode stack machine for the same set of operators. The stack
machine's instructions like ADD and MUL correspond to the operators and pop
their arguments from the stack and push the result. There's also the LIT
opcode, followed in the bytecode stream by a 4-byte little endian integer,
which is pushed onto the stack. Finally, there's a HALT opcode to finish
execution.

This is a virtual machine.

Code fragment to get you started or as inspiration--you don't have to do it
this way.

#define POP() \
    (*--top)

#define PUSH(x) \
    (*top++ = (x))

#define POPS(n) \
    assert(top - stack >= (n))

#define PUSHES(n) \
    assert(top + (n) <= stack + MAX_STACK)

// These assert macros are here to help with debugging. A correct compiler cannot trigger stack underflow and
// can only trigger stack overflow if expression depth exceeds MAX_STACK, which can be checked in the compiler.
// The Java Virtual Machine and WebAssembly specifications rely on statically verifying that kind of safety
// property at load time for untrusted code, which lets them eliminate expensive runtime safety checks. That's
// the best of both worlds: high performance without compromises in security or correctness. If you finish
// the other exercises with time to spare, try to implement a static stack-effect checker for our bytecode.
// Hint: Start with your vm_exec, remove the dynamic semantics, and just leave in the stack effect checks,
// tracking only the changing top-of-stack offset, not the actual stack (which can then be removed). This is
// called abstract interpretation. It's not very useful for our expression language, but it generalizes.

int32_t vm_exec(const uint8_t *code) {
    enum { MAX_STACK = 1024 };
    int32_t stack[MAX_STACK];
    int32_t *top = stack;
    for (;;) {
        uint8_t op = *code++;
        switch (op) {
        case SUB: {
            POPS(2);
            // Note the stack's operand order!
            int32_t right = POP();
            int32_t left = POP();
            PUSHES(1);
            PUSH(left - right);
            break;
        }
        // ...
        case LIT:
            PUSHES(1);
            // Unaligned reads are legal and fast on modern PC-class processors.
            // This assumes the target architecture is little endian; as a bonus
            // exercise, figure out how to read this little endian int32 in a way
            // that's correct regardless of the target computer's native endianness.
            PUSH(*(uint32_t *)code);
            code += sizeof(uint32_t);
            break;
        case HALT:
            POPS(1);
            return POP();
        default:
            fatal("vm_exec: illegal opcode");
            return 0;
        }
    }
}

Finally, starting with your old parser again, write a recursive-descent code
generator for this stack machine corresponding to the expression.

This is a compiler.

Congrats! Give yourself a pat on the back. You've just written a lexer, parser,
interpreter, virtual machine, compiler, all from scratch. This is a pretty
complete set of language tools, albeit for a simple language.
