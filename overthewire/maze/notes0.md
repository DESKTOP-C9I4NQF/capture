# maze 0 

> In this level there is very interesting things which is visible that same file accessed twice in program and
> is used reference with its name rather than virtual file system index.

# We can exploit this program with race condition

doing any operation in the virtual file system going to take long time
so we are going to use rename systemcall which won't make any changes
in the virtual filesystem but just change filename with user given 
string.