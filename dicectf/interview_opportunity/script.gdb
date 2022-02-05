break main
disp/8i $rip
set disassembly-flavor intel
set pagination off
break *0x0040124f
run < input
continue
si
return
