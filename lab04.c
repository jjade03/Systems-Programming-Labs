#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

#define BUFFER_SIZE 2048

int flagB = 0, flagE = 0, flagN = 0, flagS = 0;
int maxLines = 0, count = 0;
char buffer[BUFFER_SIZE];
int nonBlank = 0;

void blankLines(char[], int, int);
void numLines(char[], int, int);
void endLine(char[]);
void squeezeLines(char[]);
void testFun(char[]);

int main(int ac, char *av[]) {
    char strArr[BUFFER_SIZE];
    int numBytes;
    int src[4];
    int increm = 0;
    int i = 0; //rename
    int c = 0; // rename
    int charFlag = 0;
    char option;

    while((option = getopt(ac, av, "bens")) != -1) {
        switch(option) {
            case 'b':
                flagB = 1;
                increm = 1;
                break;
            case 'e':
                flagE = 1;
                increm = 1;
                break;
            case 'n':
                flagN = 1;
                increm = 1;
                break;
            case 's':
                flagS = 1;
                increm = 1;
                break;
        }
    }
    
    for(int t = 0; t < ac; t++) {
        if(t > increm) {
            src[c] = open(av[t], O_RDONLY);
        } else {
            continue;
        } // By comparing t to increm and incrementing it inside the while loop, the cat arguments will be ignored (supposedly)
        c++;
        if (src < 0) {
            perror(av[1]); // do not use unless its an error resulting from a system call
            return 1;
        } //put this here?
    }

    int x = 0;
    int nullCount = 0;
    int tempCount = 0;
    while ((numBytes = read(src[count], buffer, BUFFER_SIZE)) > 0) {
        for (i = 0; i < numBytes; i++) {
            strArr[x] = buffer[i];
            x++;
            if(buffer[i] == '\n') {
                maxLines++;
                tempCount = i + 1;
                while(buffer[tempCount] != '\n') {
                    if(buffer[tempCount] != ' ' && buffer[tempCount] != '\t') {
                        charFlag = 1;
                        break;
                    } 
                    tempCount++;
                }
                if(charFlag == 0) continue;
                nonBlank++;
                charFlag = 0;
            }
        } // Loops until end of file
        strArr[x] = '\0'; // Adds null terminator
        x++;
        count++;
        if(numBytes < 0) {
            perror("Read error");
            return 1;
        }
        if(count > src[count]) break;
    }
    close(src[count]);

    if(flagB == 1 || (flagB == 1 && flagN == 1)) {
        blankLines(strArr, nonBlank, count);
    } else if(flagS == 1) {
        squeezeLines(strArr);
    } else if(flagE == 1) {
        endLine(strArr);
    }  else if(flagN == 1 && flagB != 1) {
        numLines(strArr, maxLines, count);
    }  else{
        for(int k = 0; k < BUFFER_SIZE; k++) {
            if(strArr[k] == '\0') {
                nullCount++;
            }
            if(nullCount == count) break;
            printf("%c", strArr[k]);
        }
        printf("\n");
    } // If no flags are on, print normally

    return 0;
}

void blankLines(char strArr[], int nonBlank, int count) {
    int charFlag = 0;
    int nullCount = 0;
    int lineCount = 1;  
    int tempCount = 0;

    printf("\t%d\t", lineCount); // prints out the first line number
    lineCount++;
    for(int k = 0; k < BUFFER_SIZE; k++) {
        tempCount = k + 1;
        if(strArr[k] == '\n') {
            while(strArr[tempCount] != '\n') {
                if(strArr[tempCount] != ' ' && strArr[tempCount] != '\t') {
                    charFlag = 1;
                    break;
                } 
                tempCount++;
            }
            if(flagE == 1) {
                printf("$");
            }
        }
        if(strArr[k] == '\0') {
            nullCount++;
        }
        if(nullCount == count) break;
        printf("%c", strArr[k]);

        if(charFlag == 0) continue;
        if(lineCount <= nonBlank) {
            printf("\t%d\t", lineCount);  
        }
        lineCount++;     
        charFlag = 0; 
    }
    printf("\n");
}

void endLine(char strArr[]) {
    int nullCount = 0;
    int lineCount = 1;
    int tempCount = 0;
    int charFlag = 0;

    if(flagN == 1) {
        printf("\t%d\t", lineCount); // prints out the first line number
    }
    for(int k = 0; k < BUFFER_SIZE; k++) {
        if(strArr[k] == '\0') {
            nullCount++;
        }
        if(nullCount == count) break;
        if(strArr[k] == '\n' && lineCount <= maxLines) {
            printf("$");
            lineCount++;
        }
        printf("%c", strArr[k]);
        if(flagN == 1 && strArr[k] == '\n' && lineCount <= maxLines) {
            printf("\t%d\t", lineCount);
        }
    }
    printf("\n");
}

void numLines(char strArr[], int maxLines, int count) {
    int lineCount = 1;
    int nullCount = 0;

    printf("\t%d\t", lineCount);
    lineCount++;
    for(int k = 0; k < BUFFER_SIZE; k++) {
        if(strArr[k] == '\0') {
            nullCount++;
        }
        if(nullCount == count) break;
        printf("%c", strArr[k]);
        if(strArr[k] == '\n' && lineCount <= maxLines) {
            printf("\t%d\t", lineCount);
            lineCount++;
        }
    }
    printf("\n");
}

void squeezeLines(char strArr[]) {
    int nullCount = 0;
    int tempCount = 0;
    int charFlag = 0;
    int lineCount  = 0;
    int numOfLines = 1;
    
    if(flagN == 1) {
        printf("\t%d\t", numOfLines);
        numOfLines++;
    }
    for(int k = 0; k < BUFFER_SIZE; k++) {
        tempCount = k + 1;
        if(strArr[k] == '\n') {
            lineCount++;
            while(strArr[tempCount] != '\n') {
                if(strArr[tempCount] != ' ' && strArr[tempCount] != '\t' && strArr[tempCount] != '\n') {
                    charFlag = 1;
                    break;
                } 
                tempCount++;
            }
            if(flagE == 1 && lineCount == 1) {
                printf("$");
            }
        }  
        if(strArr[k] == '\0') {
            nullCount++;
        }
        if(nullCount == count) break;
        if(lineCount == 1 && charFlag == 1) {
            printf("\n");
        }
        if(charFlag == 0) {
            printf("%c", strArr[k]);            
            continue;
        }
        if(flagN == 1 && numOfLines < maxLines) { 
            printf("\t%d\t", numOfLines);
            numOfLines++;
        } 
        charFlag = 0;
        lineCount = 0;
    }
    printf("\n");
}