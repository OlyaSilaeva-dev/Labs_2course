#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#define BUFSIZE 255
#define MAXFILES 10

typedef struct SearchResult {
    char file[BUFSIZE];
    int line;
    long position;
} SearchResult;

void my_strcpy(char *str1, const char *str2)
{
    int index = 0;
    while (str2[index] != '\0')
    {
        str1[index] = str2[index];
        index++;
    }
    str1[index] = '\0';
}

const char *str_str(const char *str, const char *substr)
{
    const char *str_ptr = NULL;
    const char *substr_ptr = NULL;

    if (str == NULL || substr == NULL)
    {
        return NULL;
    }

    while (*str)
    {
        if (*str == *substr)
        {
            int trig = 1;
            str_ptr = str;
            substr_ptr = substr;

            while (*str_ptr && *substr_ptr)
            {
                trig = trig && (*str_ptr == *substr_ptr);
                if (!trig)
                {
                    break;
                }
                ++str_ptr;
                ++substr_ptr;
            }

            if (trig && !*substr_ptr)
            {
                return str;
            }
        }
        ++str;
    }
    return NULL;
}

int search_in_file(const char *substring, const char *file_path, SearchResult *result, int *index)
{
    FILE *file = fopen(file_path, "r");
    if (file == NULL)
    {
        return 1;
    }

    char line[BUFSIZE];
    int line_num = 0;
    int found = 0;

    while (fgets(line, sizeof(line), file))
    {
        line_num++;
        const char *pos = line;
        while ((pos = str_str(pos, substring)) != NULL)
        {
            result[*index].line = line_num;
            result[*index].position = (pos - line) + 1;
            my_strcpy(result[*index].file, file_path);
            (*index)++;
            found = 1;
            pos++;
        }
    }

    fclose(file);
    return found ? 0 : -1;
}

int search_in_files(const char *substring, SearchResult *res, int* index, ...)
{
    va_list file_paths;
    const char *current_path;
    int i = 0;

    va_start(file_paths, index);
    while ((current_path = va_arg(file_paths, const char *)) != NULL)
    {
        int ans_sif = search_in_file(substring, current_path, res, index);
        if (ans_sif == 1)
        {
            return ans_sif;
        }
        i++;
    }
    va_end(file_paths);
    return 0;
}

int main()
{
    const char *substring = "1111"; // Замените на вашу подстроку.
    SearchResult res[MAXFILES];
    int index = 0;

    int ans = search_in_files(substring, res, &index, "file3.txt", "file1.txt", "file2.txt", NULL);
    if (ans == 1)
    {
        printf("file_opening_error\n");
        return 1;
    }

    printf("Files where the string is found: \n");
    for (int i = 0; i < index; i++)
    {
        printf("File: %s - Line: %d, Position: %ld\n", res[i].file, res[i].line, res[i].position);
    }

    return 0;
}
