challenge store value in blackboard buffer
and then copy that buffer on stack with strcpy which would
we can overwrite return address and that will jump to `secret_func`


```gdb
see_thebb:
   0x080488ac <+167>:   pop    ebx
   0x080488ad <+168>:   pop    esi
   0x080488ae <+169>:   pop    ebp
   0x080488af <+170>:   ret
```
