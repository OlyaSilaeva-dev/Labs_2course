#include "list.c"
#include "../header.h"

int inputLivers(FILE* file_input, Node* head, size_t* size) {
    file_input = fopen("test.txt", "r");
    if (file_input == NULL) {
        return NO_MEMORY;
    }
    Liver person;

    int i = 0;
    while (fscanf(file_input, "%s %s %s %d:%d:%d %c %lf", person.surname, person.name, person.midname, 
    person.dateBirth.tm_mday, person.dateBirth.tm_mon, person.dateBirth.tm_year, person.gender, person.income) == 8) {
        push(head, person);
        i++;
    }
    *size = i;
    return OK;
}


int main () {
    Node* head = NULL;
    if (inputLivers != OK) {
        return NO_MEMORY;
    }

    


    deleteList(head);
}