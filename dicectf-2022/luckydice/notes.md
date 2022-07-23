# luckydice


### **file ./luckydice**
> ./luckydice: ELF 64-bit LSB shared object, x86-64, version 1 (SYSV), dynamically linked, interpreter /lib64/ld-linux-x86-64.so.2, BuildID[sha1]=47a6c0819a74aaf28d5bf7f62c07273695226568, for GNU/Linux 3.2.0, not stripped  

main
- Binary Takes  input from stdin in sizeof buffer is 0x32.
- It convert that string into unsigned long integer.  

  roll\_dice
  - allocate buffer of size given as input from fgets with calloc and fill it with random data in range of \[0,6\).

- Nextly program takes input again prints it, but In this case It is <u>format string</u> 

## High Level Plan
1. overwrite luck dice memory with 12
2. win shell 

## How to execute High Level plan
> 10th index of printf contains our target pointer and we will overwrite with one byte by overflow few characters of string length
```
	%243c%10$hhn
	%243c -> will pad required characters
	%10$n -> will target to 10th index of printf i.e. printf will take input from stack.
```
flag: hope{tellin_ya_its_rigged_a02b54785626f795}
