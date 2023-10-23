#include<stdio.h>
#include<stdarg.h>
#include<math.h>
#include<string.h>
#define BUFSIZE 255

void searchInFile(const char *substring, const char *filePath) {
    FILE *file = fopen(filePath, "r");
    if (file == NULL) {
        printf("Unable to open file: %s\n", filePath);
        return;
    }

    char line[BUFSIZE];
    int lineNum = 0;
    while (fgets(line, sizeof(line), file)) {
        lineNum++;
        char *pos = line;
        while ((pos = strstr(pos, substring)) != NULL) {
            printf("File: %s - Line: %d, Position: %ld\n", filePath, lineNum, (pos - line) + 1);
            pos += 1;
        }
    }
    fclose(file);
}

void searchInFiles(const char *substring, ...) {
    va_list filePaths;
    const char *currentPath;

    va_start(filePaths, substring);
    while ((currentPath = va_arg(filePaths, const char *)) != NULL) {
        searchInFile(substring, currentPath);
    }
    va_end(filePaths);
}

int main(){
    const char *substring = "blablabla";

    searchInFiles(substring, "file1.txt", "file2.txt", NULL);
}