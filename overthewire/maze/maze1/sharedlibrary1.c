#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <unistd.h>
#include <fcntl.h>
#include <assert.h>

int puts(const char* __restrict __format)
{
        char* filepath = "/etc/maze_pass/maze2", *mappedfucker;
        size_t size;
        FILE* filp;

        filp = fopen(filepath, "r");
        assert(filp != NULL);

        fseek(filp, 0, SEEK_END);
        size = ftell(filp);
        fseek(filp, 0, SEEK_SET);

        mappedfucker = malloc(size);
        assert(mappedfucker != NULL);

        fread(mappedfucker, 1, size, filp);
        fclose(filp);

        fwrite(mappedfucker, 1, size, stdout);
        free(mappedfucker);

        return -1;
}