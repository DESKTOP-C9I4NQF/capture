## babyrop [Solution](https://github.com/elliot-star/ctf/blob/main/dicectf/baby-rop/solution.py)
# High Level Plan
program pretty much checking all things except It is keeping
dangling pointer when our string and structure are been freed
so our task it to overlap the pointer in order control size
and address of structure.


As we already know that tcache will hijack the small chuncks
and put it in singly linked list for fast access. So we are
going to Create two structure which will query size of 16
bytes and allocated memory is given to us is 32 bytes and
then we will query string of 100 bytes to mismatch size of
structure

# Plan Execution
- Overlap pointers of small chunks to manipulate memory
- From Big chunks to leak address of main arena (near library)
- leak address of stack
- Do ropping


## Diagram

```
 dangling          dangling 
 pointer           poinnter
 of struct 0       of struct 1
    |                |
    v                v
 ----------        ----------
 |        |        |        |
 | memory |        | memory |
 |        |        |        |
 ----------        ----------
     ^                ^    
     |                |    
     |                |    
 allocated         allocated    
  string            struct 2
 for struct 2      
 
 
 dangling pointer
 of struct 1
       -------------------------------
       |                             |
       | struct 2                    |
       | {                           |
       |    length;                  |
       |    string;  /* dangling     |
       |               pointer of    |
       | };            struct 0 */   |
       |                             |
       -------------------------------
```

then we will free both the structure and create a new structure
and string with the size of same so Linked list will allocate
that free memory LIFO manner

Now we can use struct 2 or struct 1 to manipulate struct 0.

So we can read and write in memory any where we want uptil
memory region has the permission to read and write.

## Overlapping pointers of struct 0 && string of struct 2 
>  new.create_safe_string(0, 100, b"")
>
>  new.create_safe_string(1, 100, b"")
>
>  new.free_safe_string(0)
>
>  new.free_safe_string(1)
>
>  new.create_safe_string(2, 16, b"")


secondly we have bigger chunks of memory that are stored in
doubly linked list which has forward and backward pointer

At the first allocation of two big chunks of 2000 bytes
It will initialize the double linked list which is
pointing to the main arena and which is pointing 
to library address so that will leak the library 
offset 


## Leaking Library base
>   new.create_safe_string(3, 2000, b"")
>
>   new.create_safe_string(4, 2000, b"")
>
>   new.free_safe_string(3)
>
>   new.free_safe_string(4)
>
>   new.create_safe_string(5, 2000, b"")
>
>   library_base = get_library_offset(new.read_safe_string(5))

we can find some index from stack address in library address
so we can jump from library offset to the stack offset

We have to leak both stack local address and base address
Stack local address will change as environment we need
to leak to library pointers to stack

then we are good to go we will overwrite the stack
return address with our address.

### Ropping
- Find gadget's from binary or library
- Call following functions to read the flag
    1. mprotect to make stack executable then put our shellcode on stack
    2. open function to open the flag file
    3. read function to read the file descriptor
    4. write to write it to stdout i.e. transfered to us via socket

