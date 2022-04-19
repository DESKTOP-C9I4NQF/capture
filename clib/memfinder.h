#ifndef _MEMFINDER_H
#define _MEMFINDER_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <fcntl.h>
#include <assert.h>
#include <sys/uio.h>
#include <sys/stat.h>

struct iovec* mapfile(char *filename)
{
    struct stat stat_struct;
    char buffer[1024];
    ssize_t nbytes;
    size_t count = 0;
    struct iovec *iov;
    int fild;

    // open file with supplied name in
    // readonly mode so that we can map file
    // in memory 
    fild = open(filename, O_RDONLY);
    assert(fild > 0);

    iov = malloc(sizeof(struct iovec));
    assert(iov != NULL);

    // calculate total size of file
    fstat(fild, &stat_struct);
    iov->iov_base = malloc(stat_struct.st_size * sizeof(char));
    assert(iov->iov_base != NULL);

    // copy file in memory
    while ( (nbytes = read(fild, buffer, 1024)) > 0 )
    {
        memcpy((char*)iov->iov_base + count, buffer, nbytes); 
        iov->iov_len += nbytes;
    }
    close(fild);

    return iov;
}

void free_iovec(struct iovec* iov)
{
    // free structure from memory
    free(iov->iov_base);
    free(iov);
}

bool memsearch(struct iovec* iov, struct iovec* req)
{
    ssize_t size = iov->iov_len - req->iov_len;

    if (size < 0)
        return false;

    // search for bytes in memory 
    // by comparing linearly if found that update
    // struct return true
    for (size_t idx = 0; idx < size; idx++)
    {
        if (!memcmp((char*)iov->iov_base+idx, req->iov_base, req->iov_len-1))
        {
            iov->iov_base = iov->iov_base+idx+req->iov_len;
            iov->iov_len = size-idx;
            return true;
        }
    }
}

#endif
