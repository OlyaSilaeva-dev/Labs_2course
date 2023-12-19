#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/stat.h>
#include <string.h>

typedef enum status_code {
    OK = 0,
    WRONG_ARGC = 1,
    LS_ERROR = 2,
} status_code;

void print_err(status_code st) {
    switch (st) {
    case WRONG_ARGC:
        printf("wrong argc");
        break;
    case LS_ERROR:
        printf("ls execution error");
        break;
    default:
        break;
    }
}

status_code list_files(const char *directory) {
    DIR *dir;
    struct dirent *entry;

    if ((dir = opendir(directory)) == NULL) {
        return LS_ERROR;
    }

    while ((entry = readdir(dir)) != NULL) {
        char path[BUFSIZ];
        struct stat statbuf;

        snprintf(path, sizeof(path), "%s/%s", directory, entry->d_name);

        if (stat(path, &statbuf) == -1) {
            return LS_ERROR;
        }

        if (S_ISDIR(statbuf.st_mode)) {
            printf("\033[1;34m%s\033[0m\n", entry->d_name);  // голубой для директорий
        } else if (statbuf.st_mode & S_IXUSR) {
            printf("\033[1;32m%s\033[0m\n", entry->d_name);  // Зеленый для исполняемых файлов
        } else {
            printf("%s %ld\n", entry->d_name, statbuf.st_blocks * statbuf.st_blksize); // Первый дисковый адрес приближённо
        }
    }

    closedir(dir);
    return OK;
}

status_code main(int argc, char *argv[]) {
    if (argc < 2) {
        print_err(WRONG_ARGC);
        return WRONG_ARGC;
    }

    for (int i = 1; i < argc; ++i) {
        status_code ans = list_files(argv[i]);
        if (ans != OK) {
            print_err(ans);
            return ans;
        }
    }

    return OK;
}
