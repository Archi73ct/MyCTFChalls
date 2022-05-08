#include <stdio.h>
#include <memory.h>
#include <unistd.h>
#include <stdlib.h>
char flag[128];

int main(int argc, char **argv) {
    setvbuf(stdout, NULL, _IONBF, 0);
	setvbuf(stdin, NULL, _IONBF, 0);
	setvbuf(stderr, NULL, _IONBF, 0);
    puts("Show me your pwn-fu\n");
    int c = 0;
    unsigned int i = 0;
    FILE * fpflag = fopen("flag", "r");
    fgets(flag, 128, fpflag);
    fclose(fpflag);

    void* allocs[5];
    for (;;) {
        puts("1. malloc\n2. free\n3. read\n4. write\n");
        scanf("%d", &c);
        switch (c)
        {
        case 1:
            /* code */
            puts("Index?");
            scanf("%d", &i);
            if (i < 0 || i > 5) {
                puts("Invalid index");
                break;
            }
            allocs[i] = malloc(0x200);
            break;
        case 2:
            puts("Index?");
            scanf("%d", &i);
            if (i < 0 || i > 5) {
                puts("Invalid index");
                break;
            }
            free(allocs[i]);
            break;
        case 3:
            puts("Index?");
            scanf("%d", &i);
            if (i < 0 || i > 5) {
                puts("Invalid index");
                break;
            }
            printf("result: %s\n", allocs[i]);
            break;
        case 4:
            puts("Index?");
            scanf("%d", &i);
            if (i < 0 || i > 5) {
                puts("Invalid index");
                break;
            }
            fgets(allocs[i], 0x200, stdin);
            break;
        default:
            exit(1);
            break;
        }
    }
}