#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <dirent.h>
#include <string.h>
#include <stdlib.h>

#define MAX_WORDS 20
#define MAX_LENGTH 100

char words[MAX_WORDS][MAX_LENGTH];
int count = 0;
char dir_name[20];

int myStringCompare(const void *a, const void *b) {
    return strcmp(a, b);
} // Wrapper function around strcmp

void do_ls() {
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
    int flagA = 0, flagS = 0, flagR = 0;
    char option;
    int tempCount = 0;
    int flagOn = 0;
    int check = 0;
    
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
    
    if(flagOn == 1) {
        check++;
    }
    for(int k = 0; k < ac; k++) {
        if(ac > 1 && k > check) {
            strcpy(&dir_name[tempCount], av[k]);
        } else if(k == 0) {
            strcpy(&dir_name[k], ".");
        }
    }
    printf("\n");

    do_ls();
    
    if(flagA == 1) {
        for(int i = 0; i < MAX_WORDS; i++) {
            if(*words[i] == '\0') break; // Checks for null pointer
            printf("%s\n", words[i]);
        } // Outputs array contents including hidden files
    } else if(flagS == 1) {
        for(int i = 0; i < MAX_WORDS; i++) {
            if(*words[i] != '\0' && *words[i] != '.') {
                qsort(words, MAX_WORDS, MAX_LENGTH, myStringCompare);
                printf("%s\n", words[i]);
            }
        } // Outputs sorted array contents
    } else if(flagR == 1) {
        for(int i = MAX_WORDS; i > 0; i--) {
            if(*words[i] != '\0' && *words[i] != '.') {
                qsort(words, MAX_WORDS, MAX_LENGTH, myStringCompare);
                printf("%s\n", words[i]);
            }
        } // Outputs array contents sorted in reverse order
    } else {
        for(int i = 0; i < MAX_WORDS; i++) {
            if(*words[i] == '\0') break;
            if(*words[i] != '.') {
                printf("%s\n", words[i]);
            }
        } // Outputs array contents
    }
    return 0;
}