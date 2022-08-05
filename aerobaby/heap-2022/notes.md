# heap-2022

***Given binary allow us to do few operations such as add chunck, delete chunck, view chunck***

bug exist in code where we free and clean unchecks memory but still we dont' clean up
chuncks dangling pointer 
```C
    free(chunk->ptr); 
    chunk->ptr = NULL;
    chunk->used = false;
    free(chunks[chunk_id]);		// dangling pointer is still there
}
```

## Big Plans
> 	1. allocate 3 chunck
> 	2. First two contains size ptr equals to 0
> 	3. Size of third chunck is same to the size of `struct chunck_t`

![memory diagram](https://github.com/loggervicky69/ctf/blob/main/aerobaby/heap-2022/img/image.png?raw=true)
