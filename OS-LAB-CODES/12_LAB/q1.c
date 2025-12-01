/*
Write a C program to emulate the ls -l UNIX command that prints all files in a current directory and lists access privileges, etc. DO NOT simply exec ls -l from the program.
*/

#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/stat.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>
#include <unistd.h>
#include <string.h>

void print_permissions(mode_t mode) {
    printf((S_ISDIR(mode)) ? "d" : "-");
    printf((mode & S_IRUSR) ? "r" : "-");
    printf((mode & S_IWUSR) ? "w" : "-");
    printf((mode & S_IXUSR) ? "x" : "-");
    printf((mode & S_IRGRP) ? "r" : "-");
    printf((mode & S_IWGRP) ? "w" : "-");
    printf((mode & S_IXGRP) ? "x" : "-");
    printf((mode & S_IROTH) ? "r" : "-");
    printf((mode & S_IWOTH) ? "w" : "-");
    printf((mode & S_IXOTH) ? "x" : "-");
}

int main() {
    DIR *dir;
    struct dirent *entry;
    struct stat st;

    char cwd[1024];
    if (getcwd(cwd, sizeof(cwd)) == NULL) {
        perror("getcwd error");
        return EXIT_FAILURE;
    }

    printf("Listing of directory: %s\n\n", cwd);

    dir = opendir(cwd);
    if (dir == NULL) {
        perror("opendir error");
        return EXIT_FAILURE;
    }

    while ((entry = readdir(dir)) != NULL) {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
            continue;

        if (stat(entry->d_name, &st) == -1) {
            perror("stat error");
            continue;
        }

        print_permissions(st.st_mode);

        struct passwd *pw = getpwuid(st.st_uid);
        struct group *gr = getgrgid(st.st_gid);

        char timebuf[80];
        struct tm *tm_info = localtime(&st.st_mtime);
        strftime(timebuf, sizeof(timebuf), "%b %d %H:%M", tm_info);

        printf(" %2ld %s %s %8ld %s %s\n",
               (long) st.st_nlink,
               (pw != NULL) ? pw->pw_name : "?",
               (gr != NULL) ? gr->gr_name : "?",
               (long) st.st_size,
               timebuf,
               entry->d_name);
    }

    closedir(dir);
    return 0;
}

/*
SAMPLE OUTPUTS
--------------

If the current directory contains:
main.c
lab12/
notes.txt

Then running the compiled program might output something like:
Listing of directory: /home/student/lab12

drwxr-xr-x  2 student student     4096 Oct 17 13:22 lab12
-rw-r--r--  1 student student     2048 Oct 17 13:20 main.c
-rw-r--r--  1 student student      512 Oct 16 18:04 notes.txt
*/

/*

Listing of directory: /home/V_CSE_A1/Desktop/230905010/OS_230905010/Lab12

-rw-r--r--  1 V_CSE_A1 V_CSE_A1        0 Oct 21 09:17 q4.c
-rwx------  1 V_CSE_A1 V_CSE_A1     2774 Oct 21 09:30 q1.c
-rwxr-xr-x  1 V_CSE_A1 V_CSE_A1    16568 Oct 21 09:31 q1
-rw-r--r--  1 V_CSE_A1 V_CSE_A1        0 Oct 21 09:17 q3.c
-rw-r--r--  1 V_CSE_A1 V_CSE_A1     1896 Oct 21 09:26 q2.c

*/
