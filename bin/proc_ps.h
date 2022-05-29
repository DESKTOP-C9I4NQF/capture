#include <stdio.h>
#include <unistd.h>
#include <stdbool.h>

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

extern struct proc_ps *openproc();
extern void closeproc(struct proc_ps *);
extern bool readproc(struct proc_ps *, struct proc_struct *);
