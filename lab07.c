#include <stdio.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/stat.h>
#include <time.h>
#include <string.h>

char filename[100];
char currentFile[100];
char dir_name[100];
char subDir[100];
int subCheck = 0;
int subCount = 0;
char path[200];

void do_find();

void mode_to_string(int mode, char str[]) {
    strcpy(str,"----------");
    if(S_ISDIR(mode)) str[0]='d';
    if(S_ISCHR(mode)) str[0]='c';
    if(S_ISBLK(mode)) str[0]='b';
    if(S_ISLNK(mode)) str[0]='l';

    if(mode & S_IRUSR) str[1]='r';
    if(mode & S_IWUSR) str[2]='w';
    if(mode & S_IXUSR) str[3]='x';

    if(mode & S_IRGRP) str[4]='r';
    if(mode & S_IWGRP) str[5]='w';
    if(mode & S_IXGRP) str[6]='x';

    if(mode & S_IROTH) str[7]='r';
    if(mode & S_IWOTH) str[8]='w';
    if(mode & S_IXOTH) str[9]='x';
} // Obtains the file mode through bitmasking

void process_entry(char *dir_entry_name) {
    struct stat info;
    char modeString[11];
    if (subCheck == 1) {
      chdir(subDir);
    } else {
      chdir(dir_name);
    }

    if(stat(dir_entry_name, &info)==-1) {
        perror(dir_entry_name);
    } else {  
        if(strstr(currentFile, filename) && !S_ISDIR(info.st_mode)) {
            mode_to_string(info.st_mode, modeString);
            printf("%s (%lu/%s)\n", dir_entry_name, info.st_size, modeString);
        } else if(S_ISDIR(info.st_mode)) {
            if(*dir_entry_name != '.') {
                strcat(subDir, "/");
                strcat(subDir, dir_entry_name);
                printf("%s\n", subDir);
                subCheck = 1;
                subCount++;
                do_find(); // Enters subdirectory
            }
        }
    }
} // Outputs directory entries

void do_find() {
    struct stat info;
    int count = 0;
    DIR *dir_ptr;
    struct dirent *dirent_ptr;
    if (subCheck == 1) {
      dir_ptr = opendir(subDir); // Opens subdirectory
    } else {
      dir_ptr = opendir(dir_name); // Opens directory
    }

    if(dir_ptr == 0) {
        perror(dir_name); // If directory name doesn't exist, outputs an error
    } else {
        while((dirent_ptr = readdir(dir_ptr)) != 0) {
            strcpy(currentFile, dirent_ptr -> d_name);
            process_entry(currentFile);
        } // Reads until the end of directory
        closedir(dir_ptr);
        if (subCount > 1) {
          chdir("..");
          getcwd(path, sizeof(path));
          strcpy(subDir, path);
          subCount = 0;
        } else {
          strcpy(subDir, dir_name);
          subCheck = 0;
        }
    }
}

int main(int ac, char *av[]) {
    for(int i = 0; i < ac; i++) {
        if(ac == 3) {
            strcpy(&filename[0], av[1]);
            strcpy(&dir_name[0], av[i]);
        } else if(ac == 2) {
            strcpy(&filename[0], av[i]);
            strcpy(&dir_name[0], ".");
        }
    } // Obtains the filename and directory name (if specified)

    strcpy(subDir, dir_name);
    do_find();

    return 0;
}