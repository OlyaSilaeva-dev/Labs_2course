#include<stdio.h>
#include<stdarg.h>
#include<math.h>
#include<string.h>
#define BUFSIZE 255

int searchInFile(const char *substring, const char *filePath) {
    FILE *file = fopen(filePath, "r");
    if (file == NULL) {
        return 1;
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
    return 0;
}

int searchInFiles(const char *substring, ...) {
    va_list filePaths;
    const char *currentPath;

    va_start(filePaths, substring);
    while ((currentPath = va_arg(filePaths, const char *)) != NULL) {
        int ansSIF = searchInFile(substring, currentPath);
        if(ansSIF != 0){
            return ansSIF;
        }
    }
    va_end(filePaths);
    return 0;
}

int main(){
    const char *substring = "blablabla";

    int ans = searchInFiles(substring, "file1.txt", "file2.txt", NULL);
    if(ans != 0){
        printf("file_opening_error");
        return 1;
    }
    return 0;
}