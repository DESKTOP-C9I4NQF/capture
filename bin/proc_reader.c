#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <fcntl.h>
#include <assert.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>
#include <dirent.h>
#include <regex.h>
#include <sys/types.h>

struct proc_struct 
{
    char fname[256];                        // buffer for the process name
    pid_t pid;                              // process id of process
    pid_t ppid;                             // process id of parent process
};

struct proc_ps
{
    struct dirent **namelist;               // files in a directory
    int n;                                  // index for namelist of files in directory
    char *tmp;                              // temparary buffer
    size_t tmpsize;                         // temparary buffer size
};


bool isNumber(const char* str)
{
    for (size_t idx = 0; str[idx] != '\0'; idx++)
        if (!isdigit(str[idx]))
            return false;

    return true;
}

bool readproc(struct proc_ps* self, struct proc_struct *proc_info)
{
    FILE *filp;
    bool success = false;
    char str[512], *req_str;

    // traversing thorugh
    while (self->n--)
    {
        if (isNumber(self->namelist[self->n]->d_name))
        {
            // open status file for a process
            snprintf(str, 512, "/proc/%s/status", self->namelist[self->n]->d_name);

            filp = fopen(str, "r");
            assert(filp != NULL);

            while (getline(&self->tmp, &self->tmpsize, filp) > 0)
            {
                // name line is found extract name from it.
                if (!memcmp("Name:", self->tmp, 5))
                {
                    req_str = index(self->tmp, '\t')+1;
                    *(index(req_str, '\n')) = '\0';

                    strcpy(proc_info->fname, req_str);
                    continue;
                }

                // pid line is found extract pid number
                if (!memcmp("Pid:", self->tmp, 4))
                {
                    req_str = index(self->tmp, '\t')+1;
                    proc_info->pid = strtol(req_str, NULL, 10);
                    continue;
                }

                // ppid line is found extract ppid number
                if (!memcmp("PPid:", self->tmp, 4))
                {
                    req_str = index(self->tmp, '\t')+1;
                    proc_info->ppid = strtol(req_str, NULL, 10);
                    continue;
                }
            }
            fclose(filp);
            success = true;
        }
        free(self->namelist[self->n]);
        if (success == true)
            return true;
    }
    return false;
}

void closeproc(struct proc_ps* self)
{
    free(self->tmp);
    free(self->namelist);
}

struct proc_ps *openproc()
{
    struct proc_ps* proctab = malloc(sizeof(struct proc_ps));
    assert(proctab != NULL);

    // set getline buffer to null and also size
    // to zero which wwill be increase afterwords
    // with realloc
    proctab->tmp = NULL;
    proctab->tmpsize = 0;

    proctab->n = scandir("/proc", &proctab->namelist, NULL, alphasort);
    assert(proctab->n != -1);

    return proctab;
}


