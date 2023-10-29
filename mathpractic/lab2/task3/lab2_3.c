#include<stdio.h>
#include<stdarg.h>
#include<math.h>
#define BUFSIZE 255

const char* strstr(const char *str, const char *substr){
    const char* str_ptr = NULL;
    const char* substr_ptr = NULL;

    if(str == NULL || substr == NULL){
        return NULL;
    }

    while(*str){
        if(*str == *substr){
            int trig = 1;
            str_ptr = str;
            substr_ptr = substr;

            while(*str_ptr && *substr_ptr){
                trig = trig && (*str_ptr == *substr_ptr);
                if(!trig){
                    break;
                }
                ++str_ptr;
                ++substr_ptr;
            }

            if(trig && !*substr_ptr){
                return str;
            }
        }
        ++str;
    }
    return NULL;
}

int searchInFile(const char *substring, const char *filePath) {
    FILE *file = fopen(filePath, "r");
    if (file == NULL) {
        return 1;
    }

    char line[BUFSIZE];
    int lineNum = 0;
    while (fgets(line, sizeof(line), file)) {
        lineNum++;
        const char *pos = line;
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
    const char *substring = "";

    int ans = searchInFiles(substring, "file3.txt", "file1.txt", "file2.txt", NULL);
    if(ans != 0){
        printf("file_opening_error");
        return 1;
    }
    return 0;
}