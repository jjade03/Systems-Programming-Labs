// use curses
// scrolls down and randomly generates lines of '1' and '0'
// save lines and shift them downwards
// when reaching the border, line disappears
// runs in infinite loop and terminates when hitting ctrl + c
// takes in optional parameter of a number 1-10(?) which controls the speed
#include <stdio.h>
#include <stdlib.h>
#include <curses.h>
#include <signal.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <string.h>
#include <time.h>

void generateNum(int);
void quitProgram(int);

int main(int ac, char* av[]) {
    int speed;
    for(int i = 0; i < ac; i++) {
        if(ac == 2) {
            speed = atoi(av[1]);
        } else {
            speed = 1;
        }
    } // Defines the speed if given by the user, otherwise set to default
    signal(SIGINT, quitProgram);
    generateNum(speed);

    return 0;
}

void generateNum(int speed) {
    int high = 3;
    int low = 0;
    char tempArr[400][400];
    int count = 0;

    initscr();
    srand(time(NULL)); 
    for(int k = 0;; k++) {
        count = k;
        for(int i = 0; i < COLS; i++) {
            int randNum = low + (rand() % (high - low)); //pseudo-random num generator
            //int randNum = rand() % high; //check this
            if(randNum == 2) {
                tempArr[k][i] = ' ';
            } else if(randNum == 1) {
                tempArr[k][i] = '1';
            } else if(randNum == 0) {
                tempArr[k][i] = '0';
            }
        } // Generates single line
        for(int n = -1; n < k; n++) {
            mvprintw(n, 0,"%s", tempArr[count]);
            count--;
        }
        refresh();
        usleep(1000000/speed); // Dynamically changes speed based on user input
    } // Generates each line
    endwin();
} // Generates a '1' or '0'

void quitProgram(int sigNum) {
    signal(SIGINT, SIG_DFL);
    raise(SIGINT);
} // Terminates the program when ctrl + c is pressed