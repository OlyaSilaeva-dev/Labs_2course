#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#define SIZE 100

int my_strlen(char *str)
{
    int cnt = 0;
    while (str[cnt] != '\0')
    {
        cnt++;
    }
    return cnt;
}

void my_strcpy(char *str1, char *str2)
{
    int index = 0;
    while (str2[index] != '\0' && str1[index] != '\0')
    {
        str1[index] = str2[index];
        index++;
    }
    str1[index] = '\0';
}

void my_strcat(char *str1, char *str2)
{
    int len1 = my_strlen(str1);
    int len2 = my_strlen(str2);
    for (int i = 0; i < len2; i++)
    {
        str1[len1 + i] = str2[i];
    }
    str1[len1 + len2] = '\0';
}

int string_to_int(char *str, int *res)
{
    int index = 0;
    while (str[index] != '\0')
    {
        if (isdigit(str[index]))
        {
            *res = *res * 10 + (int)(str[index] - '0');
        }
        else
        {
            return 1;
        }
        index++;
    }
    return 0;
}

int reverseString(char *str)
{
    int len = my_strlen(str);
    for (int i = 0; i < len / 2; i++)
    {
        char temp = str[i];
        str[i] = str[len - i - 1];
        str[len - i - 1] = temp;
    }
    return 0;
}

int everySecondUpperCase(char *str)
{
    int len = my_strlen(str);
    for (int i = 1; i < len; i += 2)
    {
        if (str[i] >= 'a' && str[i] <= 'z')
        {
            str[i] += 'A' - 'a';
        }
    }
    return 0;
}

int groupSameElements(char *str)
{
    int len = my_strlen(str);
    char *a = malloc(sizeof(char) * (len + 1));
    int j = 0;

    for (int i = 0; i < len; i++)
    {
        if (isdigit(str[i]))
        {
            a[j++] = str[i];
        }
    }
    for (int i = 0; i < len; i++)
    {
        if (isalpha(str[i]))
        {
            a[j++] = str[i];
        }
    }
    for (int i = 0; i < len; i++)
    {
        if (!isalnum(str[i]))
        {
            a[j++] = str[i];
        }
    }

    a[j] = '\0';
    my_strcpy(str, a);
    free(a);
    return 0;
}

int stringConcatenation(int argc, char *argv[], char *res)
{
    int seed = 0;
    int ans = string_to_int(argv[2], &seed);
    if (ans == 1)
    {
        return 1;
    }

    srand(seed);
    for (int i = 3; i < argc; i++)
    {
        int index = rand() % (argc - 2) + 2;
        if (index < argc && index > 2)
        {
            my_strcat(res, argv[index]);
        }
    }

    return 0;
}

int testArgs(int argc, char *argv[])
{
    if(argc < 3) {
        return 1;
    }

    if (argc > 3 && argv[1][1] != 'c') {
        return 1;
    }

    if (my_strlen(argv[1]) != 2 && argv[1][0] != '-') {
        return 2;
    }
    return 0;
}

int main(int argc, char *argv[])
{
    int error = testArgs(argc, argv);
    if (error != 0)
    {
        printf("Wrong argc");
        return error;
    }

    char flag = argv[1][1];

    switch (flag)
    {
    case 'l':
        printf("%d\n", my_strlen(argv[2]));
        break;
    case 'r':
        reverseString(argv[2]);
        printf("%s\n", argv[2]);
        break;

    case 'u':
        everySecondUpperCase(argv[2]);
        printf("%s\n", argv[2]);
        break;
    case 'n':
        groupSameElements(argv[2]);
        printf("%s\n", argv[2]);
        break;
    case 'c':
        char *res = malloc(sizeof(char) * SIZE); // Adjust the size as needed
        *res = '\0';
        int ans = stringConcatenation(argc, argv, res);
        if(ans == 1){
            printf("error: str_to_int\n");
            return 2;
        }

        printf("res = %s\n", res);
        free(res);
        break;

    default:
        printf("Wrong flag\n");
        break;
    }

    return 0;
}
