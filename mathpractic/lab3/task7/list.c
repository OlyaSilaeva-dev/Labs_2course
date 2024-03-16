#include "../header.h"

typedef struct Liver {
    char* surname;
    char* name;
    char* midname;
    struct tm dateBirth;
    char gender;
    double income;
} Liver;

int cpyLiver(Liver* val1, Liver* val2) {
    val1->dateBirth = val2->dateBirth;
    val1->gender = val2->gender;
    val1->income = val2->income;
    val1->surname = strdup(val2->surname);
    val1->name = strdup(val2->name);
    val1->midname = strdup(val2->midname);

    if(val1->surname == NULL || val1->name == NULL || val1->midname == NULL) {
        return NO_MEMORY;
    }

    return OK;
}

void freeLiver(Liver* liver) {
    free(liver->surname);
    free(liver->name);
    free(liver->midname);
}

bool less(Liver val1, Liver val2) {
    if (val1.dateBirth.tm_year != val2.dateBirth.tm_year) {
        return val1.dateBirth.tm_year > val2.dateBirth.tm_year;
    }
    if (val1.dateBirth.tm_mon != val2.dateBirth.tm_mon) {
        return val1.dateBirth.tm_mon > val2.dateBirth.tm_mon;
    }
    return val1.dateBirth.tm_mday > val2.dateBirth.tm_mday;
}

bool Equal(Liver val1, Liver val2) {
    return (val1.dateBirth.tm_year == val2.dateBirth.tm_year) &&
            (val1.dateBirth.tm_mon == val2.dateBirth.tm_mon) &&
            (val1.dateBirth.tm_mday == val2.dateBirth.tm_mday) &&
            (val1.gender == val2.gender) &&
            (val1.income == val2.income) &&
            (strcmp(val1.name, val2.name) == 0) &&
            (strcmp(val1.surname, val2.surname) == 0) &&
            (strcmp(val1.midname, val2.midname) == 0);
}


typedef struct Node {
    Liver value;
    struct Node *next;
} Node;

Node* createNode(Liver val) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    if (newNode == NULL) {
        exit(EXIT_FAILURE);
    }
    cpyLiver(&(newNode->value), &val);
    newNode->next = NULL;
    return newNode;
}

// Функция для вставки элемента в упорядоченный список
void insert(Node** head, Liver val) {
    Node* newNode = createNode(val);
    if (*head == NULL || less(val, (*head)->value)) {
        newNode->next = *head;
        *head = newNode;
    } else {
        Node* current = *head;
        while (current->next != NULL && less(current->next->value, val)) {
            current = current->next;
        }
        newNode->next = current->next;
        current->next = newNode;
    }
}

//поиск элемента в списке
Node* search(Node* head, Liver val, int* index) {
    (*index) = 0;
    while (head != NULL) {
        if (Equal(head->value, val)) {
            (*index)++;
            return head;
        } else if (head->value.dateBirth.tm_year < val.dateBirth.tm_year) {
            return NULL;
        }
        head = head->next;
        (*index)++;
    }
    return NULL;
}

//удаление элемента из списка 
int delete(Node** head, Liver val) {
    Node* temp;
    if (*head == NULL) {
        return LIST_IS_EMPTY;
    }
    if (Equal((*head)->value, val)) {
        temp = *head;
        *head = (*head)->next;
        freeLiver(&(temp->value));
        free(temp);
        return OK;
    }
    Node* current = *head;
    while (current->next != NULL && !Equal(current->next->value, val)) {
        current = current->next;
    }
    if (current->next == NULL) {
        return ELEMENT_NOT_FOUND;
    } else {
        temp = current->next;
        current->next = current->next->next;
        freeLiver(&(temp->value));
        free(temp);
    }
    return OK;
}

//печать списка
void printList(Node* head) {
    while (head != NULL) {
        printf("%s %s %s %d:%d:%d %c %lf\n", head->value.surname, head->value.name, head->value.midname, 
            head->value.dateBirth.tm_mday, head->value.dateBirth.tm_mon, head->value.dateBirth.tm_year, 
            head->value.gender, head->value.income);
        head = head->next;
    }
}


// Освобождение памяти, выделенной под упорядоченный список
void deleteList(Node** head) {
    Node* current = *head;
    Node* next;
    while (current != NULL) {
        next = current->next;
        freeLiver(&(current->value));
        free(current);
        current = next;
    }
    *head = NULL;
}
