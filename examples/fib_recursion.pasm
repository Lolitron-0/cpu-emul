// n1, n2, index
fib:
push 40
jeq retlbl
pop

// index
popr eax
// n2
popr ebx
// n1
popr ecx

pushr ebx
pushr ecx
add
popr edx
outr edx
// n1
pushr ebx
// n2
pushr edx
push 1
pushr eax
// index
add

call fib

retlbl:
ret


begin

push 1
out
// n1
push 1

push 1
out
// n2
push 1
// index
push 2
call fib
