#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#define BUFSIZE 255
#define MAXFILES 10

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

int search_in_file(const char *substring, const char *file_path, char res[MAXFILES][BUFSIZE])
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
            printf("File: %s - Line: %d, Position: %ld\n", file_path, line_num, (pos - line) + 1);
            pos += 1;
            found = 1;
        }
    }

    if (found)
    {
        strcpy(res[0], file_path);
        return 0;
    }

    fclose(file);
    return 0;
}

int search_in_files(const char *substring, char res[MAXFILES][BUFSIZE], ...)
{
    va_list file_paths;
    const char *current_path;
    int i = 0;

    va_start(file_paths, res);
    while ((current_path = va_arg(file_paths, const char *)) != NULL)
    {
        int ans_sif = search_in_file(substring, current_path, res);
        if (ans_sif != 0)
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
    const char *substring = "string"; // Замените на вашу строку поиска
    char res[MAXFILES][BUFSIZE];

    int ans = search_in_files(substring, res, "file3.txt", "file1.txt", "file2.txt", NULL);
    if (ans != 0)
    {
        printf("file_opening_error");
        return 1;
    }

    printf("Files where the string is found: \n");
    for (int i = 0; res[i][0] != '\0'; i++)
    {
        printf("%s\n", res[i]);
    }

    return 0;
}
