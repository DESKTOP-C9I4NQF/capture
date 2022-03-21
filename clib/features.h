#ifndef _STDIO_H
#define _STDIO_H

#include <stdio.h>
#endif

/* requests a number of bytes to be compared */
int rq_memcmp(const char *dest, const char *src, size_t bytes, size_t rq_cmp)
{
        size_t count = 0;
        for (size_t i = 0; i < bytes; i++)
        {
                if ((unsigned char)dest[i] == (unsigned char)src[i])
                {
                        if (++count == rq_cmp)
                                return 0;
                        else
                                continue;
                }
        }
        return 1;

}
