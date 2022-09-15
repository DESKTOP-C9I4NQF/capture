#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <crypt.h>
#include <search.h>
#include <assert.h>

#define array_size(array) (sizeof(array) / sizeof(*array))

// hashes and there corresponding plaintext
struct textInfo {
    char* cipherText;
    char* plainText;
};

int main(void)
{
    ENTRY e, *ep;
    char* text = NULL, *cipher;
    size_t size = 0, idx;
    FILE* filp;

    struct textInfo tInfo[] = {
        {"$1$abadsalt$0abdVS0D4YnJJ4b7l0RRr1", NULL},
        {"$1$abadsalt$p394aiqZnKUyrO5Rg9Tf01", NULL},
        {"$1$abadsalt$cJYsdaTkB9F9L9yH2Qjtd.", NULL},
        {"$1$abadsalt$lFZDGpRdmOwRbu6HWuqjv0", NULL},
        {"$1$abadsalt$1AI/LbmumKa5e6dOxiVe11", NULL},
        {"$1$abadsalt$e2hAp/NXE.Uezx3ZOwA5L0", NULL},
        {"$1$abadsalt$Cua6x6Rgd8UUHn7Mnzibj.", NULL},
        {"$1$abadsalt$7XBxlsUB3yXcL62wQpgjK/", NULL},
        {"$1$abadsalt$DnSSAXOSmaoAAhN4WKaU90", NULL},
        {"$1$abadsalt$Cua6x6Rgd8UUHn7Mnzibj.", NULL},
        {"$1$abadsalt$7wLTt8frOzyxahbB9Lzdi.", NULL},
    };

    hcreate(array_size(tInfo));

    for (idx = 0; idx < array_size(tInfo); idx++)
    {
        e.key = tInfo[idx].cipherText;
        e.data = &tInfo[idx];

        // add element to hash table
        ep = hsearch(e, ENTER);
        if (ep == NULL) {
            fprintf(stderr, "entry failed\n");
            exit(EXIT_FAILURE);
        }
    }

    filp = fopen("/usr/share/dict/words", "r");
    assert (filp != NULL);

    for (;;)
    {
    /* print two entries from the table, and
        show that two are not in the table */
        ssize_t cursize;
        
        cursize = getline(&text, &size, filp);
        if (cursize < 0) break;
        text[--cursize] = '\0';

        e.key = crypt(text, "$1$abadsalt");
        ep = hsearch(e, FIND);
        if (ep)
        {
            struct textInfo* tmp = (struct textInfo*)ep->data;
            tmp->plainText = strdup(text);
        }
    }
    fclose(filp);
    free(text);

    for (idx = 0; idx < array_size(tInfo); idx++)
    {
        if (!tInfo[idx].plainText)
        {
            // same hash found 
            e.key = tInfo[idx].cipherText;
            ep = hsearch(e, FIND);
            if (ep)
            {
                struct textInfo* tmp = (struct textInfo*)ep->data;
                tInfo[idx].plainText = strdup(tmp->plainText);
            }
        }
        printf("cipherText: `%s`, plainText: `%s`\n", tInfo[idx].cipherText, tInfo[idx].plainText);
    }

    printf("FLAG: `");
    for (idx = 0; idx < array_size(tInfo); idx++)
    {
        if (tInfo[idx].plainText)
            putchar(tInfo[idx].plainText[1]);
        // free plainText as it is no longer useful
        free(tInfo[idx].plainText);
    }
    printf("`\n");

    hdestroy();
    return 0;
}