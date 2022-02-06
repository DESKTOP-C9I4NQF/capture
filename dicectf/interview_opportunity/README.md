# DiceCTF: interview-opportunity
### [Solution](https://github.com/elliot-star/ctf/edit/main/dicectf/interview_opportunity/exploit.py)

## High Level Plan
- Do stack over flow by byte 
- Leak library address from one byte overflow
- Jump to main again then do ropp and
- Call system with argument "/bin/sh"

binary allocates buffer of 10 bytes and read input 70 bytes 
So we can overwrite the stuff on binary and program read
same buffer with puts that means we can read arbitrary 
memory up to '\0' byte.

First approach that come in my mind is to reuse the same
code from .text region that will not help us for leaking
address as library and stack has aslr enable, next
approach is to execute main again by calling _libc_start_main
which will result into leak of address and also will read
buffer again and we do bufferoverflow again.

With the help of rr I recorded who program state and go
reverse instruction inorder to get last byte of _libc_start_main
which calls main.

Example main caller code
```
   0x7f12d1715559 <__libc_start_call_main+121>: mov    rax,QWORD PTR [rsp+0x8]
=> 0x7f12d171555e <__libc_start_call_main+126>: call   rax
   0x7f12d1715560 <__libc_start_call_main+128>: mov    edi,eax
   0x7f12d1715562 <__libc_start_call_main+130>: call   0x7f12d172cf50 <__GI_exit>
```
by Returning to above code we will call the main again and
then find offset of "/bin/sh" and system to execute
system and read the flag from the machine.

to Execute shell
> 19 binsh = _libc_start_main_main+1455183       # offset of /bin/sh
>
> 20 system = _libc_start_main_main+139597       # offset of system

Done :) 
