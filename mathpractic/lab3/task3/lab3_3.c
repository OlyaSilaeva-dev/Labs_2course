#include<stdio.h>
#include<inttypes.h>
#include<stdlib.h>
#include<string.h>
#define MAX_EMPLOYEES 500

struct Employee
{
    uint32_t id;
    char name[100];
    char surname[100];
    double salary;
}Employee;

int check_name1(char* a, char* b){
    int i = 0;
    while(a[i] != '\0' && b[i] != '\0'){
        if(a[i] < b[i]){
            return -1;
        } else if(a[i] > b[i]){
            return 1;
        }
    i++;
    }
    return 0;
}

int cmp1 (const void *a, const void *b){
    struct Employee *emp1 = (struct Employee *)a;
    struct Employee *emp2 = (struct Employee *)b;

    if(emp1->salary != emp2->salary){
        return emp1->salary < emp2->salary;
    }

    int check_ser = check_name1(emp1->surname, emp2->surname);
    if(check_ser != 0){
        return check_ser;
    }

    int check_names = check_name1(emp1->name, emp2->name);
    if(check_names != 0){
        return check_names;
    }

    return emp1->id < emp2->id;
}

int check_name2(char* a, char* b){
    int i = 0;
    while(a[i] != '\0' && b[i] != '\0'){
        if(a[i] > b[i]){
            return -1;
        } else if(a[i] < b[i]){
            return 1;
        }
    i++;
    }
    return 0;
}

int cmp2 (const void *a, const void *b){
    struct Employee *emp1 = (struct Employee *)a;
    struct Employee *emp2 = (struct Employee *)b;

    if(emp1->salary != emp2->salary){
        return emp1->salary > emp2->salary;
    }

    int check_ser = check_name2(emp1->surname, emp2->surname);
    if(check_ser != 0){
        return check_ser;
    }

    int check_names = check_name2(emp1->name, emp2->name);
    if(check_names != 0){
        return check_names;
    }

    return emp1->id > emp2->id;
}

int checkArgs(int argc, char* argv[]){
    if(argc != 4){
        return 1;
    }
    
    if((argv[1][0] != '/' && argv[1][0] != '-') || strlen(argv[1]) != 2){
        return 2;
    }

    return 0;
}

int main(int argc, char * argv[]){
    int check = checkArgs(argc, argv);
    if(check != 0){
       if(check == 1){
            printf("Wrong argc");
       } else if(check == 2){
            printf("Wrong flag");
       }
       return check;
    }
    
    FILE* file_input = fopen(argv[2], "r");
    if(file_input == NULL){
        printf("file_input is NULL");
        return 3;
    }

    struct Employee * infa;
    infa = (struct Employee*)malloc(sizeof(struct Employee) * MAX_EMPLOYEES);
    if(infa == NULL){
        printf("memory allocation error");
        return 4;
    }


    int i = 0;
    while (fscanf(file_input, "%u %s %s %lf", &infa[i].id, infa[i].name, infa[i].surname, &infa[i].salary) == 4){   
        if(i >= MAX_EMPLOYEES){
            printf("too much employees");
            fclose(file_input);
            return 4;
        }
        i++;   
    }
    
    int sizeInf = i;
    switch (argv[1][1])
    {
    case 'a':
        qsort(infa, sizeInf, sizeof(struct Employee), cmp1);
        break;
    case 'b':
        qsort(infa, sizeInf, sizeof(struct Employee), cmp2);
        break;
    default:
        fclose(file_input);
        printf("Wrong flag");
        return 5;
    }
    FILE* file_output = fopen(argv[3], "w");
    if(file_output == NULL){
        printf("file_output is NULL");
        fclose(file_input);
        return 3;
    }
    for(int j = 0; j < sizeInf; j++){
        fprintf(file_output, "%u %s %s %lf\n", infa[j].id, infa[j].name, infa[j].surname, infa[j].salary);
    }
    
    free(infa);
    fclose(file_input);
    fclose(file_output);
    
    return 0;
}