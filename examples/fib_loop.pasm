push 1
popr eax
outr eax
push 1
popr ebx
outr ebx
push 2
popr ecx

push 1
pushr ecx

A:
popr ecx
pop

// refill stack
pushr eax
pushr ebx
add

// move register values
pushr eax
popr ebx
popr eax
outr eax

// increase index
push 40
push 1
pushr ecx
add

// condition
jb A

