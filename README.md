# cpu-emul
Tiny interpreter for assembly-like language. 
\
\
By default memory (stack in particular) value type is `int64_t`, to change it iterpreter should be recompiled (see `MemoryValueType` alias). Requirements to this type is to:
-  be _DefaultConstructible_
- be _CopyConstructible_
- be _Constructible from string_ (you can write wrapper for non-trivial types and throw `ArgumentConstructionException` in case of parsing failure)
-  _Support arithmetic operations_.

Use `-h`/`--help` for usage.
\
\
Commands are case insensitive, labels are declared `like_so:`. Each command should be on next line, arguments are separated with spaces. Comments should be on separate line and start with `// ` (note **space** after slashes here - it is required). See `examples/` for details.
### Available commands:
| Command           | Semantics |
| :----------------: | :------ |
|   begin      |   Tells processor where execution should start   |
| end          |   Finishes program   |
| push _value_   |  Pushes _value_ on stack   |
| pop |  Removes value from stack   |
| pushr _reg_ | Pushes value from register _reg_ on stack |
| popr _reg_ | Removes top value from stack and writes it into register _reg_ |
| add | Adds to top stack values and pushes result on stack (operands are popped) |
| sub | Substracts 2nd top stack value from top one and pushes result on stack (operands are popped) |
| mul | Multiplicates 2 top stack values and pushes result on stack (operands are popped) |
| div | Divides 1st top stack value from 2nd top one and pushes result on stack (operands are popped) |
| out | Prints (into stdout) top stack value and removes it |
| outr _reg_ | Prints (into stdout) _reg_ register value |
| in | Reads (from stdin) value and pushes it on stack |
| jmp _label_ | Unconditional jump in _label_ label |
| jeq _label_ | Jumps if two top stack values are equal |
| jne _label_ | Jumps if two top stack values are not equal |
| ja _label_ | Jumps if 1st top stack value is bigger than 2nd top  |
| jae _label_ | Jumps if 1st top stack value is not less than 2nd top |
| jb _label_ | Jumps if 1st top stack value is less than 2nd top  |
| jbe _label_ | Jumps if 1st top stack value is not bigger than 2nd top |
| call _label_ | Call of a function located on _label_ label |
| ret | Returns from function (use "end" in outermost scope) |
