#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <time.h>
#include <sys/ioctl.h>

int __cols = 0;
int __rows = 0;

int __colors[] = {30, 31, 32, 33, 34, 35, 36, 37};
char *__picto[] = {
    "\u25A0", "\u25B2", "\u25B6", "\u25BC", "\u25C0",
    "\u25C9", "\u25FC", "\u25E2", "\u25E5", "\u25C6",
    " ", " ", " "
};

void diep(char *str) {
	printf("[-] %s: %s\n", str, strerror(errno));
	exit(EXIT_FAILURE);
}

int rnd(int low, int high) {
    return low + rand() % (high + 1 - low);
}

void reload() {
    struct winsize ws;
    
    if(ioctl(0, TIOCGWINSZ, &ws) < 0)
        diep("TIOCGWINSZ");

    __rows = ws.ws_row;
    __cols = ws.ws_col;
}


//
// ANSI escape
//
void clear() {
    printf("\033[2J");
}

void move(int x, int y) {
    printf("\033[%d;%df", x, y);
}

void show() {
    printf("\033[?25l");
}

void hide() {
    printf("\033[?25h");
}

void color(int id) {
    printf("\033[1;%dm", __colors[id]);
}

void write(int id) {
    printf("%s", __picto[id]);
}

//
// initializer
//
void initialize() {
    // update console
    reload();
    
    // randomize
    srand(time(NULL));
    
    // clean the screen
    show();
    clear();
}

//
// worker
//
int main(void) {
    initialize();
    
    while(1) {
        move(rnd(0, __rows), rnd(0, __cols));
        color(rnd(0, (sizeof(__colors) / sizeof(int)) - 1));
        write(rnd(0, (sizeof(__picto) / sizeof(char *)) - 1));

        fflush(stdout);
        usleep(500);
    }
    
    return 0;
}
