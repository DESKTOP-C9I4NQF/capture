#ifndef _HELPERFCN_H_
#define _HELPERFCN_H_

#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/uio.h>

void recvuntil(int fild, struct iovec* _iov_inpb, struct iovec* _iov_outb)
{
  char context;
  size_t n_bytes = 0;
  char *required_bytes;

  required_bytes = (char *)_iov_inpb->iov_base;

  // if output buffer is set to
  // null then we output to stdout
  // other wise we store in a buffer
  if (_iov_outb == NULL)
  {
    while (read(fild, &context, 1) > 0 && n_bytes < _iov_inpb->iov_len)
    {
      if (context == required_bytes[n_bytes])
        n_bytes++;
      else
        n_bytes = 0;
      write(STDOUT_FILENO, &context, 1);
    }
  }
  else
  {
    size_t count = 0;
    char *bufptr;

    // do dynamic buffering
    bufptr = malloc(sizeof(char) * 20);
    assert(bufptr != NULL);

    while (read(fild, &context, 1) > 0 && n_bytes < _iov_inpb->iov_len)
    {
      if (context == required_bytes[n_bytes])
        n_bytes++;
      else
        n_bytes = 0;

      bufptr[count] = context;
      if (++count % 20 == 0)
        bufptr = realloc(bufptr, sizeof(char) * (count+20));
    }

    _iov_outb->iov_len = count;
    _iov_outb->iov_base = (void *)bufptr;
  }
}
#endif
