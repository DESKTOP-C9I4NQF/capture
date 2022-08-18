# pwn/cshell2

*Challenge Description*
> Well since cshell was pwned because tcache bins were used, I decided to restrict you to sizes
> above tcache allocation because then tcache can't be used :)!

- In every function given below we check for array out of bound by comparing with 0x0e which is max size of array
- 0x00000000004012EC function will add cshell user aliased as: add\_cshell2
- 0x0000000000401196 function will show cshell user data aliased as: show\_cshell2
- 0x000000000040155F function will delete cshell user and clean memory with zero aliased as: delete\_cshell2
- 0x0000000000401646 function will edit cshell user data and is aliased as: edit\_cshell2

**We have four functions and we create a jump table for it. which is from offset 0 to 5, zero contains NULL so even if we call it, it is going to segfault.**

