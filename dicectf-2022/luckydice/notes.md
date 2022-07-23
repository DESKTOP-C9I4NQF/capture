# luckydice


### **file ./luckydice**
> ./luckydice: ELF 64-bit LSB shared object, x86-64, version 1 (SYSV), dynamically linked, interpreter /lib64/ld-linux-x86-64.so.2, BuildID[sha1]=47a6c0819a74aaf28d5bf7f62c07273695226568, for GNU/Linux 3.2.0, not stripped  

main
- Binary Takes  input from stdin in sizeof buffer is 0x32.
- It convert that string into unsigned long integer.  

  roll_dice
  - allocate buffer of size given as input from fgets with calloc and fill it with random data in range of \[0,6\).

- test
