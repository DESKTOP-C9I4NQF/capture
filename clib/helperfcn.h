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
#include <sys/ptrace.h>

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
    }
  }
  else
  {
    size_t count = 0;
    char *bufptr;

    // do dynamic buffering
    bufptr = malloc(sizeof(char) * 20);
    assert(bufptr != NULL);

    while (n_bytes < _iov_inpb->iov_len && read(fild, &context, 1) > 0)
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


/*
 * dumps hex value from
 * in supplied file desciptor
 * from given buffer and size
 * bytes
 */
void hexdump(int fild, unsigned char *bufptr, size_t size)
{
  for (size_t ind = 0; ind < size; ind++)
  {
    dprintf(fild, "\\x%02x", bufptr[ind]);
  }
}


// reverses endian of a 64 bit integer
void reverse_endian(uint64_t *inp_little)
{
  uint8_t* buf = (uint8_t*)inp_little;
  uint8_t temp;

  for (uint8_t i = 0; i < 4; i++)
  {
    temp = buf[i];
    buf[i] = buf[7-i];
    buf[7-i] = temp;
  }
}

/*
 * ptrace_memorydump is used to read nbytes froom
 * memory and store in the buf pointer 
 */
void ptrace_dumpMemory(const pid_t child_pid, size_t* bufptr, const size_t nbytes, const size_t address)
{
  const size_t size = nbytes / 8;

  for (size_t i = 0; i < size; i++)
    bufptr[i] = ptrace(PTRACE_PEEKTEXT, child_pid, address+(i*8), NULL);
}

/*
 * returns pid of
 * the child process
 * with given stdin, stdout,
 * stderr file descriptors
 */
pid_t popen2(const char* prog_name,  int stdin_fd, int stdout_fd, int stderr_fd)
{
  pid_t child_pid;
  child_pid = fork();
  if (!child_pid)
  {
    dup2(stdin_fd, STDIN_FILENO);
    dup2(stdout_fd, STDOUT_FILENO);
    dup2(stderr_fd, STDERR_FILENO);

    execl(prog_name, prog_name, NULL); 
  }
  return child_pid;
}

#endif
