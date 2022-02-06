babyrop
High Level Plan
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

```
 dangling          dangling 
 pointer           poinnter
 of struct 1       of struct 2
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
  string            struct 3
 for struct 3      
 
 
 dangling pointer
 of struct 2
       -------------------------------
       |                             |
       | struct 3                    |
       | {                           |
       |    length;                  |
       |    string;  /* dangling     |
       |               pointer of    |
       | };            struct 1 */   |
       |                             |
       -------------------------------
```

then we will free both the structure and create a new structure
and string with the size of same so Linked list will allocate
that free memory LIFO manner

Now we can use struct 3 or struct 2 to manipulate struct 1.

So we can read and write in memory any where we want uptil
memory region has the permission to read and write.


secondly we have bigger chunks of memory that are stored in
doubly linked list which has forward and backward pointer

At the first allocation of two big chunks of 2000 bytes
It will initialize the double linked list which is
pointing to the main arena and which is pointing 
to library address so that will leak the library 
offset 

we can find some index from stack address in library address
so we can jump from library offset to the stack offset
