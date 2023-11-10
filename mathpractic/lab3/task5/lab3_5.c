#include <stdio.h>
#include <inttypes.h>
#include <stdlib.h>
#include <string.h>
#define MAX_STUDENT 500
#define STRING_SIZE 100

typedef struct Student {
    uint32_t id;
    char name[STRING_SIZE];
    char surname[STRING_SIZE];
    char group[STRING_SIZE];
    double grades[5];
} Student;


int findByID(Student *students, int size, uint32_t id, Student *res, int *index) {
    int j = 0;
    for (int i = 0; i < size; i++) {
        if (students[i].id == id) {
            res[j] = students[i];
            j++;
        }
    }
    *index = j;
    return 0;
}

int findBySurname(Student *students, int size, char *sname, Student *res, int *index) {
    int j = 0;
    for (int i = 0; i < size; i++) {
        if (strcmp(students[i].surname, sname) == 0) {
            res[j] = students[i];
            j++;
        }
    }
    *index = j;
    return 0;
}

int findByName(Student *students, int size, char *name, Student *res, int *index) {
    int j = 0;
    for (int i = 0; i < size; i++) {
        if (strcmp(students[i].name, name) == 0) {
            res[j] = students[i];
            j++;
        }
    }
    *index = j;
    return 0;
}

int findByGroup(Student *students, int size, char *group, Student *res, int *index) {
    int j = 0;
    for (int i = 0; i < size; i++) {
        if (strcmp(students[i].group, group) == 0) {
            res[j] = students[i];
            j++;
        }
    }
    *index = j;
    return 0;
}

int cmpid(const void *a, const void *b) {
    Student *stu1 = (Student *)a;
    Student *stu2 = (Student *)b;
    return stu1->id - stu2->id;
}

int cmpsur(const void *a, const void *b) {
    Student *stu1 = (Student *)a;
    Student *stu2 = (Student *)b;
    return strcmp(stu1->surname, stu2->surname);
}

int cmpname(const void *a, const void *b) {
    Student *stu1 = (Student *)a;
    Student *stu2 = (Student *)b;
    return strcmp(stu1->name, stu2->name);
}

int cmpgroup(const void *a, const void *b) {
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


int inputStudents(char* argv[], Student * infa, int* mas_size) {
    if (infa == NULL){ return 2; }

    FILE *file_input = fopen(argv[1], "r");
    if (file_input == NULL){ return 3; }

    int i = 0;
    while (fscanf(file_input, "%u %s %s %s %lf %lf %lf %lf %lf", &infa[i].id, &infa[i].name, &infa[i].surname, &infa[i].group, &infa[i].grades[0], &infa[i].grades[1], &infa[i].grades[2], &infa[i].grades[3], &infa[i].grades[4]) == 9) {
        if (i >= MAX_STUDENT) {
            fclose(file_input);
            return 4;
        }
        i++;
    }
    *mas_size = i;

    fclose(file_input);
    return 0;
}

int fileOutput(FILE** file_output, int argc, char* argv[]) {
    // printf("file opening");
    if (argc == 3) {
        *file_output = fopen(argv[2], "w");
        if (*file_output == NULL) {
            return 2;
        }
    } else {
        *file_output = fopen("file_output.txt", "w");
        if (*file_output == NULL) {
            return 2;
        }
    }
    // printf("file open");
    return 0;
}

int printfStudents(FILE* file_output, int mas_size, Student* infa) {
    for (int i = 0; i < mas_size; i++) {
        fprintf(file_output, "%u %s %s %s", infa[i].id, infa[i].name, infa[i].surname, infa[i].group);
        printf("[%d] %u %s %s %s", i, infa[i].id, infa[i].name, infa[i].surname, infa[i].group);
        for (int k = 0; k < 5; k++) {
            fprintf(file_output, " %lf", infa[i].grades[k]);
            printf(" %lf", infa[i].grades[k]);
        }
        fprintf(file_output, "\n");
        printf("\n");
    }
    return 0;
}

int averangeScore(FILE* file_output, int mas_size, Student* infa){
    Student* res = (Student*)malloc(sizeof(Student) * MAX_STUDENT);
    double* averanges = (double*)malloc(sizeof(double) * mas_size);
    if(res == NULL || averanges == NULL){
        return 2;
    }

    double sum = 0;
    for(int i = 0; i < mas_size; i++){
        double sum1 = 0;
        for(int k = 0; k < 5; k++){
            sum1 += infa[i].grades[k];
        }
        averanges[i] = sum1 / 5.0;
        sum += sum1;
      
    }
   
    double averange = sum / (double)(mas_size * 5);    
    
    int j = 0;
    for(int i = 0; i < mas_size; i++){
        if(averanges[i] > averange){
            res[j] = infa[i];
            j++;
        }
    }
    
    printfStudents(file_output, j, res);

    free(res);
    free(averanges);

    return 0;
}

int searchBySomething(FILE* file_output, int mas_size, Student* students, char flag){
    Student *res = (Student *)malloc(sizeof(Student) * MAX_STUDENT);
    if (res == NULL) {
        return 2;
    }

    int ind = 0;
    switch (flag){ 
        case 'i':
            printf("enter the id: ");
            int id;
            if(scanf("%u", &id) != 1){
                printf("Wrong id");
                return 5;
            }
            int ans1 = findByID(students, mas_size, id, res, &ind); //search by ID
            printfStudents(file_output, ind, res);
            break;

        case 's':
            printf("enter the sername: ");
            char sername[STRING_SIZE];
            
            if(scanf("%s", &sername) != 1){
                printf("Wrong sername");
                return 5;
            }
            printf("%s", sername);

            int ans2 = findBySurname(students, mas_size, sername, res, &ind); //search by ID
            printfStudents(file_output, ind, res);
            
            break;

        case 'n':
            printf("enter the name: ");
            char name[STRING_SIZE];

            if(scanf("%s", name) != 1){
                printf("Wrong name");
                return 5;
            }
            
            int ans3 = findByName(students, mas_size, name, res, &ind); //search by ID
            printfStudents(file_output, ind, res);
            break;

        case 'g':
            printf("enter the group: ");
            char group[STRING_SIZE];
            if(scanf("%s", group) != 1){
                printf("Wrong group");
                return 5;
            }
            int ans4 = findByGroup(students, mas_size, group, res, &ind); //search by ID
            printfStudents(file_output, ind, res);
            break;
        default:
            return 6;
    }

    free(res);
    return 0;
}


int sortBySomething(FILE* file_output, int mas_size, Student* students, char flag){
        
    switch (flag){
        case 'i':
            fprintf(file_output, "sort by ID:\n");
            qsort(students, mas_size, sizeof(Student), cmpid);
            break;
        case 's':
            fprintf(file_output, "\nsort by surname:\n");
            qsort(students, mas_size, sizeof(Student), cmpsur);
            break;
        case 'n':
            fprintf(file_output, "\nsort by name:\n");
            qsort(students, mas_size, sizeof(Student), cmpname);
            break;
        case 'g':
            fprintf(file_output, "\nsort by group:\n");
            qsort(students, mas_size, sizeof(Student), cmpgroup);
            break;
        
        default:
            return 6;
    }
    printfStudents(file_output, mas_size, students);

    return 0;
}

int main(int argc, char *argv[]) {
    if (argc < 2 || argc > 3) {
        printf("Wrong argc");
        return 1;
    }

    Student *students = (Student *)malloc(sizeof(Student) * MAX_STUDENT);
    int mas_size;
    int ans_input = inputStudents(argv, students, &mas_size);
    if(ans_input != 0) {
        free(students);
        if(ans_input == 2) {
            printf("memory allocation error");
        } else if (ans_input == 3) {
            printf("file_input is NULL");
        } else {
            printf("too much students");
        }
        return ans_input;
    }

    FILE *file_output;
    int ans_output = fileOutput(&file_output, argc, argv);
    
    if(ans_output != 0){
        free(students);
        printf("file_output is NULL");
        return ans_output;
    }


    printf("Select a function: \n");
    printf("f: find students\n");
    printf("s: sort students\n");
    printf("a: the average score is above the average for all students\n");
    
    char flag1;
    int resscanf = scanf("%c", &flag1);
    if(resscanf != 1){
        printf("Wrong input");
        fclose(file_output);
        free(students);
        return 6;
    }

    if (flag1 == 'f') {
        printf("i: search by id\n");
        printf("s: search by surname\n");
        printf("n: search by name\n");
        printf("g: search by group\n");
        getchar();

        char flag2;
        int ans = scanf("%c", &flag2);
        if(ans != 1){
            printf("Wrong input");
            fclose(file_output);
            free(students);
            return 6;
        }
        int resSearch = searchBySomething(file_output, mas_size, students, flag2);
        if(resSearch != 0) {
            if(resSearch == 2) {
                printf("memory allocation error");
            } else if (resSearch == 5) {
                printf("Wrong input");
            } else {
                printf("Wrong flag");
            }
            free(students);
            fclose(file_output);
            return resSearch; 
        }
    } else if (flag1 == 's'){
        printf("i: sort by id\n");
        printf("s: sort by surname\n");
        printf("n: sort by name\n");
        printf("g: sort by group\n");
        getchar();

        char flag2;
        int ans = scanf("%c", &flag2);
        if(ans != 1){
            printf("Wrong flag");
            fclose(file_output);
            free(students);
            return 6;
        }
        int resSort = sortBySomething(file_output, mas_size, students, flag2);
        if(resSort == 6){
            printf("Wrong flag");
            free(students);
            fclose(file_output);
            return resSort;
        }
   } else if(flag1 == 'a'){
        int resAver = averangeScore(file_output, mas_size, students);
        if(resAver != 0){
            printf("memory allocation error");
            free(students);
            fclose(file_output);
        }
   } else {
        printf("Wrong flag");
        free(students);
        fclose(file_output);
        return 6;
    }

    free(students);
    fclose(file_output);
    return 0;
}
