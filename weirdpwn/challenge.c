#include <stdio.h>
#include <sys/param.h>
#include <stdlib.h>
#include <string.h>

void setup() {
    setvbuf(stdout, NULL, _IONBF, 0);
	setvbuf(stdin, NULL, _IONBF, 0);
	setvbuf(stderr, NULL, _IONBF, 0);
}

void print_menu() {
    puts("1. new image");
    puts("2. print image");
    puts("3. draw line");
    puts("4. get time");
    puts("5. exit");
    return;
}

#define MAX_SZ 64

void print_image(char p[MAX_SZ][MAX_SZ]) {
    for (size_t y = 0; y < MAX_SZ; y++) {
        for (size_t x = 0; x < MAX_SZ; x++) {
            if (p[y][x] == 0) {
                putc('.', stdout);
            }
            else {
                putc((int)p[y][x], stdout);
            }
        }
        putc(10, stdout);
    }
}

void new_image(char p[MAX_SZ][MAX_SZ]) {
    puts("Clearing image!");
    for (size_t y = 0; y < MAX_SZ; y++) {
        memset(p[y], 0, sizeof(p[y]));
    }
}

// Using bresenhams line algorightm
void draw_line(char p[MAX_SZ][MAX_SZ]) {
    // How many lines?
    size_t lines = 0;
    puts("How many lines would you like to draw?");
    scanf("%d", &lines);
    if (lines > 20 ) {
        puts("Slow down there!");
        return;
    }
    for (int t = 0; t < lines; t++) {
        int x1=0,x2=0,y1=0,y2=0,color=0;
        int dx=0,dy=0,sx=0,sy=0;
        int y;
        puts("Draw a line");
        puts("Format: x1 y1 x2 y2 color");
        if (scanf("%d %d %d %d %d", &x1, &y1, &x2, &y2, &color) != 5) {
            puts("Format error");
            return;
        }
        dx = abs(x2-x1), sx = x1<x2 ? 1 : -1;
        dy = abs(y2-y1), sy = y1<y2 ? 1 : -1;
        int err = (dx > dy ? dx : -dy) / 2, e2;
        for(;;) {
            p[y1][x1] = (char)(color & 0xff);
            if (x1 == x2 && y1 == y2) break;
            e2 = err;
            if (e2 > -dx) {err -= dy; x1 += sx;}
            if (e2 < dy) {err += dx; y1 += sy;}
        }
    }
}

int main() {
    // Main challenge
    char p[MAX_SZ][MAX_SZ];
    int choice;
    setup();
    puts("Hello and welcome to my drawing app");
    while (1) {
        print_menu();
        scanf("%d", &choice);
        switch (choice)
        {
        case 1:
            new_image(p);
            break;
        case 2:
            print_image(p);
            break;
        case 3:
            draw_line(p);
            break;
        case 4:
            system("time");
        case 5:
            return 0;
        default:
            puts("Not a real option");
            break;
        }
    }
}