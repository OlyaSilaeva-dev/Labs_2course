#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

#define BUFFER 1000

int searchInFile(const char *filePath, const char *searchString) {
    FILE *current_file = fopen(filePath, "r");
    if (current_file == NULL) {
        printf("file %s is NULL\n", filePath);
        return 1;
    }

    char *line = NULL;
    size_t len = 0;
    ssize_t read;
    int line_number = 0;
    int found = 0;

    while ((read = getline(&line, &len, current_file)) != -1) {
        line_number++;
        if (strstr(line, searchString) != NULL) {
            printf("Найдено вхождение в файле %s\n", filePath);
            found = 1;
        }
    }

    free(line);
    fclose(current_file);

    return found ? 0 : -1;
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Wrong argc");
        return 1;
    }

    const char *searchString = "asdasdasd";
    FILE *file = fopen(argv[1], "r");
    if (file == NULL) {
        perror("file is NULL");
        return 2;
    }

    char currentFile[BUFFER];
    while (fgets(currentFile, BUFFER, file) != NULL) {
        currentFile[strcspn(currentFile, "\n")] = 0;
        pid_t pid = fork();

        if (pid < 0) {
            perror("Error create process");
            return 3;
        } else if (pid == 0) {
            int result = searchInFile(currentFile, searchString);
            exit(result);
        }
    }

    int status;
    while (wait(&status) > 0) {
        if (WIFEXITED(status)) {
            int exit_status = WEXITSTATUS(status);
            if (exit_status == -1) {
                printf("В файле %s нет вхождений строки\n", currentFile);
            }
        }
    }

    fclose(file);
    return 0;
}
