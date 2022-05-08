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
