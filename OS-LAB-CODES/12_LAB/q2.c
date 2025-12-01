/*
Write a program that will list all files in a current directory and all files in subsequent subdirectories.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>

void list_files_recursive(const char *basePath) {
    char path[1000];
    struct dirent *dp;
    DIR *dir = opendir(basePath);


    if (!dir) {
        fprintf(stderr, "Unable to open directory: %s\n", basePath);
        return;
    }

    while ((dp = readdir(dir)) != NULL) {
        if (strcmp(dp->d_name, ".") == 0 || strcmp(dp->d_name, "..") == 0)
            continue;

        snprintf(path, sizeof(path), "%s/%s", basePath, dp->d_name);

        printf("%s\n", path);

        struct stat statbuf;
        if (stat(path, &statbuf) == 0 && S_ISDIR(statbuf.st_mode)) {
            list_files_recursive(path);
        }
    }

    closedir(dir);
}

int main() {
    char cwd[1024];

    if (getcwd(cwd, sizeof(cwd)) == NULL) {
        perror("getcwd() error");
        return EXIT_FAILURE;
    }

    printf("Files recursively listed from directory: %s\n\n", cwd);

    list_files_recursive(cwd);

    return 0;
}

/*
SAMPLE OUTPUTS
--------------

If your directory looks like this:

/home/user/lab
/home/user/lab/main.c
/home/user/lab/docs
/home/user/lab/docs/readme.txt
/home/user/lab/images
/home/user/lab/images/logo.png

Running the program will output:

Files recursively listed from directory: /home/user/lab

/home/user/lab/main.c
/home/user/lab/docs
/home/user/lab/docs/readme.txt
/home/user/lab/images
/home/user/lab/images/logo.png
*/

/*

Files recursively listed from directory: /home/V_CSE_A1/Desktop/230905010/OS_230905010/SampleL12Q2

/home/V_CSE_A1/Desktop/230905010/OS_230905010/SampleL12Q2/Folder2
/home/V_CSE_A1/Desktop/230905010/OS_230905010/SampleL12Q2/Folder2/damn.pdf
/home/V_CSE_A1/Desktop/230905010/OS_230905010/SampleL12Q2/Folder2/code.cpp
/home/V_CSE_A1/Desktop/230905010/OS_230905010/SampleL12Q2/Folder2/cat.jpg
/home/V_CSE_A1/Desktop/230905010/OS_230905010/SampleL12Q2/q2
/home/V_CSE_A1/Desktop/230905010/OS_230905010/SampleL12Q2/Folder1
/home/V_CSE_A1/Desktop/230905010/OS_230905010/SampleL12Q2/Folder1/hehe.exe
/home/V_CSE_A1/Desktop/230905010/OS_230905010/SampleL12Q2/Folder1/hello.c
/home/V_CSE_A1/Desktop/230905010/OS_230905010/SampleL12Q2/Folder1/profile.png
/home/V_CSE_A1/Desktop/230905010/OS_230905010/SampleL12Q2/q2.c

*/
