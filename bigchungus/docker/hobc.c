#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> 
#include <ctype.h>
#include <string.h>

#define MAXLINE 1024
#define MAX_IMG_SIZE 1024*10

void setup() {
    setvbuf(stdout, NULL, _IONBF, 0);
    setvbuf(stdin, NULL, _IONBF, 0);
    setvbuf(stderr, NULL, _IONBF, 0);
}
typedef struct _ppm {
    char magic[2];
    ssize_t width;
    ssize_t height;
    ssize_t size;
    unsigned long maxval;
} ppm;

void parse_magic(ppm* image) {
    char c = 0;
    read(STDIN_FILENO, &c, 1);
    if (c == 'P') {
        read(STDIN_FILENO, &c, 1);
        if (c == '3') {
            image->magic[0] = 'P';
            image->magic[1] = '3';
            read(STDIN_FILENO, &c, 1);
            return;
        }
    }
    printf("Error, not ppm\n");
    exit(-1);
}

void read_size(ppm* image) {
    char line[MAXLINE];
    int c, i = 0;
    ssize_t w,h  = 0;
    memset(line, 0, MAXLINE);
    do {
        c = getchar();
        if (c == '\n' || c == EOF) break;
        line[i++] = c & 0xff;
    } while (i < MAXLINE-1);
    int r = sscanf(line, "%lld %lld", &w, &h);
    if (r != 2) {
        printf("Size not read!\n");
        exit(-1);
    }
    printf("Size %lldx%lld\n", w, h);
    image->width = w;
    image->height = h;
    image->size = w*h;
    return;
}

void read_max_val(ppm* image) {
    char line[MAXLINE];
    int c, i = 0;
    unsigned long mval = 0;
    memset(line, 0, MAXLINE);
    do {
        c = getchar();
        if (c == '\n' || c == EOF) break;
        line[i++] = c & 0xff;
    } while (i < MAXLINE-1 && c != 0);
    int r = sscanf(line, "%lld", &mval);
    if (r != 1) {
        printf("Could not read max value\n");
        exit(-1);
    }
    image->maxval = mval;
    return;
}

int main() {
    char line[MAXLINE];
    setup();
    printf("Welcome to the image parser!\n");
    printf("Please send your ppm image:\n");
    ppm *image = malloc(sizeof(ppm));
    printf("Oh no my pointers are leaky %p %p\n", image, malloc);
    // read the header
    parse_magic(image);
    // read size
    read_size(image);
    // read max val for RGB stuff 
    read_max_val(image);
    printf("Maxval %d\n", image->maxval);
    char* data;
    memset(line, 0, MAXLINE);
    if (image->size > MAX_IMG_SIZE) {
        data = (char*)malloc(MAX_IMG_SIZE+1);
    } else {
        data = (char*)malloc(image->size+1);
    }
    unsigned int i = 0;
    int c = 0;
    unsigned int cx = 0;
    int res = 0;
    do {
        c = getchar();
        if (isspace(c)) {
            int r = sscanf(line, "%d", &res);
            if (r != 1) continue;
            data[cx++] = res & 0xff;
            memset(line, 0, MAXLINE);
            i = 0;
            continue;
        }
        line[i++] = c & 0xff;

    } while (c != EOF && i < MAXLINE-1 && c != 0 && cx < (size_t)image->size);
    // Trim size in case of miscalculation
    image->size = cx;

    // Count the bytes

    malloc(image->maxval);

    // print info
    char *y = (char*)malloc(255);
    for (int i = 0; i < image->size; i++) {
        y[data[i]]++;
    }
    for (int i = 0; i < 255; i++) {
        printf("%d:\t%d", i, y[i]);
    }
    free(y);
}