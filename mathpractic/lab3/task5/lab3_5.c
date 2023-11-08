#include <stdio.h>
#include <inttypes.h>
#include <stdlib.h>
#include <string.h>
#define MAX_STUDENT 500
#define STRING_SIZE 100

typedef struct Student
{
    uint32_t id;
    char name[STRING_SIZE];
    char surname[STRING_SIZE];
    char group[STRING_SIZE];
    double grades[5];
} Student;

int findID(Student *students, int size, uint32_t id, Student *res, int *index)
{
    int j = 0;
    for (int i = 0; i < size; i++)
    {
        if (students[i].id == id)
        {
            res[j] = students[i];
            j++;
        }
    }
    *index = j;
    return 0;
}

int findSurname(Student *students, int size, char *sname, Student *res, int *index)
{
    int j = 0;
    for (int i = 0; i < size; i++)
    {
        if (strcmp(students[i].surname, sname) == 0)
        {
            res[j] = students[i];
            j++;
        }
    }
    *index = j;
    return 0;
}

int findName(Student *students, int size, char *name, Student *res, int *index)
{
    int j = 0;
    for (int i = 0; i < size; i++)
    {
        if (strcmp(students[i].name, name) == 0)
        {
            res[j] = students[i];
            j++;
        }
    }
    *index = j;
    return 0;
}

int findGroup(Student *students, int size, char *group, Student *res, int *index)
{
    int j = 0;
    for (int i = 0; i < size; i++)
    {
        if (strcmp(students[i].group, group) == 0)
        {
            res[j] = students[i];
            j++;
        }
    }
    *index = j;
    return 0;
}

int cmpid(const void *a, const void *b)
{
    Student *stu1 = (Student *)a;
    Student *stu2 = (Student *)b;
    return stu1->id - stu2->id;
}

int cmpsur(const void *a, const void *b)
{
    Student *stu1 = (Student *)a;
    Student *stu2 = (Student *)b;
    return strcmp(stu1->surname, stu2->surname);
}

int cmpname(const void *a, const void *b)
{
    Student *stu1 = (Student *)a;
    Student *stu2 = (Student *)b;
    return strcmp(stu1->name, stu2->name);
}

int cmpgroup(const void *a, const void *b)
{
    Student *stu1 = (Student *)a;
    Student *stu2 = (Student *)b;
    return strcmp(stu1->group, stu2->group);
}

int average_mark(double marks[], int index, double *res)
{
    int cnt = 5;
    double summ = 0;
    for (int i = 0; i < cnt; i++)
    {
        summ += marks[i];
    }
    double avMark = summ / cnt;
    *res = avMark;
    return 0;
}

int find_all_students(Student *students, int mas_size, FILE *file_output, char flag)
{
    for (int j = 0; j < mas_size; j++)
    {
        Student *res = (Student *)malloc(sizeof(Student) * MAX_STUDENT);
        if (res == NULL)
        {
            printf("memory allocation error");
            fclose(file_output);
            return 3;
        }
        int index = 0;
        if (flag == 'i')
        {
            findID(students, mas_size, students[j].id, res, &index);
        }
        else if (flag == 's')
        {
            findSurname(students, mas_size, students[j].surname, res, &index);
        }
        else if (flag == 'n')
        {
            findName(students, mas_size, students[j].name, res, &index);
        }
        else
        {
            findGroup(students, mas_size, students[j].group, res, &index);
        }
        for (int i = 0; i < index; i++)
        {
            fprintf(file_output, "%u %s %s %s", res[i].id, res[i].name, res[i].surname, res[i].group);
            for (int k = 0; k < 5; k++)
            {
                fprintf(file_output, " %lf", res[i].grades[k]);
            }
            fprintf(file_output, "\n");
        }
        free(res);
    }
    return 0;
}


int main(int argc, char *argv[])
{
    if (argc < 2 || argc > 3)
    {
        printf("Wrong argc");
        return 1;
    }

    FILE *file_input = fopen(argv[1], "r");
    if (file_input == NULL)
    {
        printf("file_input is NULL");
        return 2;
    }

    Student *infa = (Student *)malloc(sizeof(Student) * MAX_STUDENT);
    if (infa == NULL)
    {
        printf("memory allocation error");
        fclose(file_input);
        return 3;
    }

    int i = 0;
    while (fscanf(file_input, "%u %s %s %s %lf %lf %lf %lf %lf", &infa[i].id, infa[i].name, infa[i].surname, infa[i].group, &infa[i].grades[0], &infa[i].grades[1], &infa[i].grades[2], &infa[i].grades[3], &infa[i].grades[4]) == 9)
    {
        if (i >= MAX_STUDENT)
        {
            printf("too much students");
            fclose(file_input);
            return 4;
        }
        i++;
    }
    int mas_size = i;

    fclose(file_input);

    /////

    FILE *file_output;
    if (argc == 3)
    {
        file_output = fopen(argv[2], "w");
        if (file_output == NULL)
        {
            printf("file is NULL");
            return 2;
        }
    }
    else
    {
        file_output = fopen("file_output.txt", "w");
        if (file_output == NULL)
        {
            printf("file is NULL");
            return 2;
        }
    }

    int ans1 = find_all_students(infa, mas_size, file_output, 'i'); //search by ID
    int ans2 = find_all_students(infa, mas_size, file_output, 's'); //search by surname
    int ans3 = find_all_students(infa, mas_size, file_output, 'n'); //search by name
    int ans4 = find_all_students(infa, mas_size, file_output, 'g'); //search by group

    if (ans1 == 3 || ans2 == 3 || ans3 == 3 || ans4 == 3)
    {
        printf("memory allocation error");
        fclose(file_output);
        return 3;
    }

    if (ans1 == 4 || ans2 == 4 || ans3 == 4 || ans4 == 4)
    {
        printf("file output error");
        fclose(file_output);
        return 3;
    }

    qsort(infa, mas_size, sizeof(Student), cmpid);
    for (i = 0; i < mas_size; i++)
    {
        fprintf(file_output, "%u %s %s %s", infa[i].id, infa[i].name, infa[i].surname, infa[i].group);
        for (int k = 0; k < 5; k++)
        {
            fprintf(file_output, " %lf", infa[i].grades[k]);
        }
        fprintf(file_output, "\n");
    }

    qsort(infa, mas_size, sizeof(Student), cmpsur);
    for (i = 0; i < mas_size; i++)
    {
        fprintf(file_output, "%u %s %s %s", infa[i].id, infa[i].name, infa[i].surname, infa[i].group);
        for (int k = 0; k < 5; k++)
        {
            fprintf(file_output, " %lf", infa[i].grades[k]);
        }
        fprintf(file_output, "\n");
    }

    qsort(infa, mas_size, sizeof(Student), cmpname);
    for (i = 0; i < mas_size; i++)
    {
        fprintf(file_output, "%u %s %s %s", infa[i].id, infa[i].name, infa[i].surname, infa[i].group);
        for (int k = 0; k < 5; k++)
        {
            fprintf(file_output, " %lf", infa[i].grades[k]);
        }
        fprintf(file_output, "\n");
    }

    qsort(infa, mas_size, sizeof(Student), cmpgroup);
    for (i = 0; i < mas_size; i++)
    {
        fprintf(file_output, "%u %s %s %s", infa[i].id, infa[i].name, infa[i].surname, infa[i].group);
        for (int k = 0; k < 5; k++)
        {
            fprintf(file_output, " %lf", infa[i].grades[k]);
        }
        fprintf(file_output, "\n");
    }

    free(infa);
    fclose(file_output);
    return 0;
}
