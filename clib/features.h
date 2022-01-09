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

/* returns pointer to given format string and store it in memory */
char *make_msg(char *fmt, ...)
{
        char *p = 0;
        int size = 0;
        va_list ap;
        
        va_start(ap, fmt);
        size = vsnprintf(p, size, fmt, ap);
        va_end(ap);
        
        ++size;                 /* for '\0' */
        p = malloc(size);
        if (p == NULL)
                return NULL;
        
        va_start(ap, fmt);
        size = vsnprintf(p, size, fmt, ap);
        va_end(ap);
        
        if (size < 0)           /* if vsnprintf fails */
        {
                free(p);
                return NULL;
        }
        
        return p;
}
