int main(int argc, char *argv[])
{
    void fp();
    char* code;

    fp = code;
    if (argc != 2)
        exit(1);
    
    // copy code from user and execute that
    // code
    strncpy(code, argv[1], 8);
    fp();
    return 0;
}