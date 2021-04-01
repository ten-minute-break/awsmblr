# AWSMblr

The project for anyone who wants to try out writing a small interpreter for an assembly-level language!

## Who is this for?

Some people who want to know more about how scripting languages are implemented and maybe want to put some into their game.
This kind of virtual machine instruction set is VERY reminiscent of Lua, as it was used originally in WoW, so I wanted to start with that. :)

Warning: this project looks bigger on the outside. ;)

## What is it?

It's a small assembly-like language! It can't do much; integer-only arithmetic, some comparisons, etc. but it lays down some nice groundwork if you want to continue on and extend it!

It doesn't even have a parser or anything like that, just backend entry through C++ itself, here's a snippet:

```cpp
auto env = Environment{};     // map between registry numbers and values
auto stack = Stack{};         // a stack for holding integers

auto program = Program{
  pushc(10),  // push the constant 10 onto the stack
  pushc(20),  // push the constant 20 onto the stack
  add(),      // add two values on the stack together and put the result onto the stack
  popr(1)     // remove the value from the stack and put it into the registry R1
};

auto result = Interpret(program, env, stack);
// the result here tells us that the status was Success, and that the instruction pointer was on instruction 4

PrintEnvironment(env);
// we get JSON from this: { "R1": 30 }
```

## How do I actually start?

The `awsmblr.h` file contains a large comment with all the documentation on the instructions you need to implement. Don't be scared, take it one by one, feel free to discuss!

Once done, there's a million ways forward: from making your own core instructions, to implementing instructions as helper functions that use the current core instructions en masse -- anything is possible. Here's a couple of directions:

### Extensions

- implement the standard x86-like assembly commands like `MOV`, `INC` and `DEC`
- implement additional operations like `MUL`, `DIV`, `MOD`, etc.
- implement jump-if-zeros
- negation would be useful

### Usability

- create a loader for bytecode from a binary format (already really close, just needs to be loaded correctly)
- create a tokenizer and parser, so that the bytecode could be loaded from a text file and reinterpreted without restarting the program
- improve error-handling and reporting

### Design

- the [JVM](https://docs.oracle.com/javase/specs/jvms/se7/html/jvms-6.html) is very similar to our infrastructure. They have some weird instructions like `PUSH0`, `PUSH1`, `DUP2`... were they just being silly or should we have those too?
- try implementing some other types except for just `int`. how does that affect our codebase?
- if we wanted to go for types, should we include typechecking like the [CIL](https://en.wikipedia.org/wiki/Common_Intermediate_Language) does? it actually has types on the assembly-level, how cool is that?!
- to make a scripting language useful, it needs to be able to connect directly to outside variables (gameplay/engine-side structs etc.); in our case, we would need to be able to pass references to some outside variables as the `Environment`, and read and affect them from the script
- maybe we need a higher-level scripting language on top of this that is compiled into awsmblr: basically, every statement or expression in that language, like `R1 + R3`, would be translated into a whole series of instructions (`pushr(1), pushr(3), add()` in this case) during translation

All of these are cool additions for later challenges :) let's talk about what you find interesting and where you would want to pull this!
As always, feel free to leave your code in a separate branch via pull requests!
