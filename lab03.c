#include <fcntl.h>
#include <stdio.h>
#include <time.h>
#include <utmp.h>
#include <unistd.h>

int main () {
    struct tm *loginTime;
    char timeStr[100];    
    struct utmp currentRecord;
    int utmpFile;
    int utmpLength = sizeof(currentRecord);

    utmpFile = open(UTMP_FILE, O_RDONLY);
    while(read(utmpFile, &currentRecord, utmpLength) == utmpLength) {
        time_t  t = currentRecord.ut_time; // Sets the time as the user's login time
        loginTime = gmtime(&t); // Converts the seconds from 't' into date-time notation
        strftime(timeStr, sizeof(timeStr), "%b %d %H:%M", loginTime); // Formats the time
        printf("%s %s %s\n", currentRecord.ut_user, currentRecord.ut_line, timeStr);
    } // Reads through utmp file and outputs the user's information until the end is reached
    close(utmpFile);

    return 0;
}