#include <curses.h>
#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <curses.h>

void primeNum();
void userRequest();
int currentNum = 0;

int main() {
    signal(SIGINT, userRequest);
    primeNum();

    return 0;
}

void primeNum() {
    int flag = 0;
    for(int k = 2;; k++) {
        for(int i = 2; i <= k/2; i++) {
            if(k % i == 0) {
                flag = 1;
                break;
            } // If number is not prime, breaks
        }
        if(flag == 0) {
          currentNum = k; // Sets the most recent prime number
        } else {
            flag = 0;
        }
    }
} // Loop to infintitely generate numbers and find out whether or not they are prime

void userRequest(int sigNum) {
    int ch;
    initscr();
    printw("%d\tQuit [y/n]? ", currentNum);
    ch = getch();
    printw("\n");
    endwin();

    printf("%d\tQuit [y/n]? %c\n", currentNum, ch);

    if((char)ch == 'y' || (char)ch == 'Y') {
        signal(SIGINT, SIG_DFL);
        raise(SIGINT);
    } // Ends the program
} // Prompts user to enter a character when ctrl + c is entered, using the curses library