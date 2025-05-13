#include <asm-generic/ioctls.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <dirent.h>
#include <string.h>
#include <stdlib.h>
#include <sys/ioctl.h>

/* Constant Variables */
#define MAX_WORDS 20
#define MAX_LENGTH 100

/* Public Variables */
char words[MAX_WORDS][MAX_LENGTH];
char longest[100];
char dir_name[20];
int flagA = 0, flagS = 0, flagR = 0;

/* Functions */
void displayFiles(int, int);

int compareElements(const void *a, const void *b) {
    if (flagR == 0) {
        return strcmp(a, b);
    } else {
        return strcmp(b, a);
    }
} // Wrapper function around strcmp -- compares elements

int compareLength(const void *a, const void *b) {
    size_t fa = strlen((const void *)a);
    size_t fb = strlen((const void *)b);

    if (flagR == 0) {
        return (fa > fb) - (fa < fb);
    } else {
        return (fa < fb) - (fa > fb);
    }
} // Sorts by length of element

void do_ls() {
    int count = 0;
    DIR *dir_ptr;
    struct dirent *dirent_ptr;
    dir_ptr = opendir(dir_name); // Opens directory
    if(dir_ptr == 0) {
        perror(dir_name); // If directory name doesn't exist, outputs an error
    } else {
        while((dirent_ptr = readdir(dir_ptr)) != 0) {
            for(int i = 0; i < MAX_WORDS; i++) {
                for(int k = 0; k < MAX_LENGTH; k++) {
                    strcpy(words[count], dirent_ptr->d_name); // reads files into array
                }
            }
            count++;
        } // Reads until end of directory
        strcpy(words[count], "\0"); // If end of array, input null terminator

        closedir(dir_ptr);
    } // If directory name exists, continues the program
}

int main(int ac, char *av[]) {
    struct winsize wbuf;
    char option;
    int tempCount = 0;
    int flagOn = 0;
    int columnNum = 0;
    int maxSize = 0;

    if(ioctl(0, TIOCGWINSZ, &wbuf) != -1) {
        maxSize = (wbuf.ws_col / 100) + 1;
    } // Obtains screen size

    while((option = getopt(ac, av, "asr")) != -1) {
        switch(option) {
            case 'a':
                flagA = 1;
                flagOn = 1;
                break;
            case 's':
                flagS = 1;
                flagOn = 1;
                break;
            case 'r':
                flagR = 1;
                flagOn = 1;
                break;
        }
    }

    for(int k = 0; k < ac; k++) {
        if(ac > 1 && k > flagOn) {
            strcpy(&dir_name[tempCount], av[k]);
        } else if(k == 0) {
            strcpy(&dir_name[k], ".");
        }
    }
    printf("\n");

    do_ls();
    qsort(words, MAX_WORDS, MAX_LENGTH, compareElements);
    for (int i = 0; i < MAX_WORDS; i++) {
        if(strlen(longest) < strlen(words[i])) {
            strcpy(longest, words[i]);
        }
    } // Finds longest element
    
    if(flagS == 1) {
        qsort(words, MAX_WORDS, MAX_LENGTH, compareLength); // Sorts array by length
        displayFiles(columnNum, maxSize);
    } else if(flagR == 1) {
        qsort(words, MAX_WORDS, MAX_LENGTH, compareLength);
        displayFiles(columnNum, maxSize); // Outputs hidden files
    } else if(flagA == 1) {
      displayFiles(columnNum, maxSize); // Outputs array contents including hidden files
    } else {
      displayFiles(columnNum, maxSize);
    }

    return 0;
}

void displayFiles(int columnNum, int maxSize) {
    int start = 0;
    int length = strlen(longest);
    int increase = 0;
    int offset = 13 / (maxSize + 1);
    
    for (int i = 0; i < MAX_WORDS; i++) {
        if (*words[i] != '\0' && (*words[i] != '.' || flagA == 1)) {
            if (columnNum <= maxSize) {
                if (*words[i + start] != '.' || flagA == 1) {
                    printf("%-*s\t", length, words[i + start]);
                }
                columnNum++;
                start = offset * columnNum;
                if (columnNum > maxSize) {
                    printf("\n");
                    increase++;
                    start = 0;
                    i = i - maxSize;
                    if (increase > offset) {
                      break;
                    }
                    columnNum = 0;
                }
            }
        }
    } // Outputs array contents in the specified order
    printf("\n");
}