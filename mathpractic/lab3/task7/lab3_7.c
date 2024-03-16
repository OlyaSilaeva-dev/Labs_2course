#include "list.c"
#define MAS_SIZE 100

void freePersonFIO(Liver* person) {
    free(person->surname);
    person->surname = NULL;
    free(person->name);
    person->name = NULL; 
    free(person->midname);
    person->midname = NULL;
}

int allocPersonFIO(Liver* person) {
    // Reallocate memory for the next iteration
    person->midname = (char*)malloc(sizeof(char) * MAS_SIZE);
    person->name = (char*)malloc(sizeof(char) * MAS_SIZE);
    person->surname = (char*)malloc(sizeof(char) * MAS_SIZE);
        
    if (person->midname == NULL || person->name == NULL || person->surname == NULL) {
        // Handle memory allocation failure
        freePersonFIO(person);
        return NO_MEMORY;
    }
    return OK;
}

int inputLivers(char** argv, Node** head, size_t* size) {
    FILE* file_input = fopen(argv[1], "r");
    if (file_input == NULL) {
        return NO_MEMORY;
    }

    Liver person;
    int i = 0;
    bool readResult = true;
    while (readResult) {      
        allocPersonFIO(&person);  
        if (fscanf(file_input, "%s %s %s %d:%d:%d %c %lf", person.surname, person.name, person.midname,
        &person.dateBirth.tm_mday, &person.dateBirth.tm_mon, &person.dateBirth.tm_year, &person.gender, &person.income) != 8) {
            readResult = false;
            freePersonFIO(&person);
            continue;
        }
        insert(head, person);
        i++;
        // Free memory for each iteration
        freePersonFIO(&person);
    }
            
    *size = i;
    fclose(file_input);
    return OK;
}



int enterThePerson(Liver* person) {
    if(allocPersonFIO(person) != OK) {
        return NO_MEMORY;
    };
    if (scanf("%s %s %s %d:%d:%d %c %lf", person->surname, person->name, person->midname,
        &person->dateBirth.tm_mday, &person->dateBirth.tm_mon, &person->dateBirth.tm_year, &person->gender, &person->income) != 8) {
        return WRONG_INPUT;
    }
    return OK;
}


int main (int argc, char** argv) {
    if(argc != 3) {
        print_err(INVALID_ARGC);
        return INVALID_ARGC;
    }
    
    Node* head = NULL;
    size_t size;
    if (inputLivers(argv, &head, &size) != OK) {
        print_err(NO_MEMORY);
        return NO_MEMORY;
    }

    printList(head);
  
    while (1) {
        printf("Select the operation\n");
        printf("    s: search for a resident\n");
        printf("    c: changing an existing resident\n");
        printf("    d: deleting  information about resident\n");
        printf("    a: adding information about residents\n");
        printf("    u: uploading data from a list to a file\n");
        printf("    e: exit the program\n");

        char flag;
        if(scanf("%c", &flag) != 1) {
            return WRONG_INPUT;    
        }
        
        Liver person;
        Node* res;
        int index;
        switch (flag)
        {
        case 's':
            if(enterThePerson(&person) != OK) {
                print_err(WRONG_INPUT);
                return WRONG_INPUT;
            };
            getchar();
            res = search(head, person, &index);
            if(res != NULL) {
                printf("%s\n%d\n", res->value.name, index);
            } else {
                printf("person not found");
            }
            freeLiver(&person);
            break;
        case 'c':
            if(enterThePerson(&person) != OK) {
                print_err(WRONG_INPUT);
                return WRONG_INPUT;
            };
            res = search(head, person, &index);
            if(res == NULL) {
                printf("person not found");
                break;
            }
            delete(&head, res->value);
            Liver person1;
            if(enterThePerson(&person1) != OK) {
                print_err(WRONG_INPUT);
                
            };
            getchar();
            insert(&head, person1);
            break;
        case 'd':
            if(enterThePerson(&person) != OK) {
                print_err(WRONG_INPUT);
                return WRONG_INPUT;
            };
            int ans = delete(&head, person);
            freeLiver(&person);
            if(ans != OK) {
                print_err(ans);
                deleteList(&head);
                return ans;
            }
            break;
        case 'a':
            if(enterThePerson(&person) != OK) {
                print_err(WRONG_INPUT);
                return WRONG_INPUT;
            };
            insert(&head, person);
            break;
        case 'u':
            printList(head); 
            break;
        case 'e':        
            deleteList(&head);
            return OK;
        default:
            printf("wrong flag\n");    
            break;
        }
    } 

    deleteList(&head);
    return OK;
}