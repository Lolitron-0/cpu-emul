// num
fact:
push 1
jae retlabel
pop

popr eax
pushr eax
push 1
pushr eax
sub
call fact
mul
ret

retlabel:
pop
ret

begin
in
call fact
out
