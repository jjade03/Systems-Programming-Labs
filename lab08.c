#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <utmp.h>
#include <time.h>

#define BUFSIZE 1024

int main(int ac, char *av[]) {
    struct tm *currentTime;
    time_t timeConvert;
    char timeStr[100];
    int utmpFile;
    struct utmp userRecords;
    int fileLength = sizeof(userRecords);
    char temp[100];
    char buf[BUFSIZE];
    char hiMessage[100];
    char userPts[100];
    int count = 0;
    char condensedName[100];
    char currentUser[100];

    if(ac != 2) {
        printf("Usage: %s username\n", av[0]);
    } else {
        FILE *fp = popen("w", "r");
        strncpy(condensedName, av[1], 8);
        while(fgets(buf, sizeof(buf), fp)) {
            if(strstr(buf, condensedName)) {
                count++;
            }
        }
        pclose(fp);

        strcpy(currentUser, getlogin());
        if(!strstr(av[1], currentUser)) {
            printf("Invalid username: %s\n", av[1]);
        } else {
            sprintf(userPts,"/dev/pts/%d", count);
            int fd = open(userPts, O_WRONLY);
            // Checks if the file exists
            if(fd == -1) {
                perror(av[1]);
            } else {
                utmpFile = open(UTMP_FILE, O_RDONLY);
                if(read(utmpFile, &userRecords, fileLength) == fileLength) {
                time(&timeConvert);
                currentTime = localtime(&timeConvert);
                strftime(timeStr, sizeof(timeStr), "%I:%M", currentTime);

                sprintf(hiMessage, "\n\rMessage from %s@%s on %s at %s:\n", userRecords.ut_user, 
                    userRecords.ut_host, userRecords.ut_line, timeStr);
                }
                close(utmpFile);

                char byeMessage[] = "EOF";
                write(fd, hiMessage, strlen(hiMessage));
                while(fgets(buf, BUFSIZE, stdin) != 0) {
                    if(write(fd, buf, strlen(buf)) == -1) {
                        break;
                    }
                }

                write(fd, byeMessage, strlen(byeMessage));
                close(fd);
            }
        }
    }
    
    return 0;
}