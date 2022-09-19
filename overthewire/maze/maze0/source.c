#include <stdio.h>
#include <string.h>

int main(int argc, char *argv[])
{
    char buf[0x14];
    int access_status;
    memset(buf, 0, 0x14);

    access_status = access("/tmp/128ecf542a35ac5270a87dc740918404");
    if (access_status)
    {
        setresuid(geteuid(), geteuid(), geteuid());

        int fild = open("/tmp/128ecf542a35ac5270a87dc740918404");
        if (fild < 0)
            exit(-1);

        read(fild, buf, 0x13);
        write(1, buf, 0x13);
    }

    return 0;
}