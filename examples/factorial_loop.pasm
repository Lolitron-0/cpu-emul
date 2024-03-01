in
// target
popr edx
// mult
push 1
// index
push 1

// nothing
push -1

loop:
// pop cmp
pop

push 1
add
// index
popr eax
pushr eax
mul
pushr eax
pushr edx
ja loop


pop
pop
out
