#include <stdio.h>
#include <stdlib.h>

typedef struct Liver {
    char* surname;
    char* name;
    char* midname;
    struct tm dateBirth;
    char gender;
    double income;
} Liver;

typedef struct Node {
    Liver value;
    struct Node *next;
} Node;

Node *head = NULL;

// добавить узел
void push(Node **head, Liver data) {
    Node *tmp = (Node*)malloc(sizeof(Node));
    if (!tmp) {
        exit(-1); // handle memory allocation failure
    }
    tmp->value = data;
    tmp->next = (*head);
    (*head) = tmp;
}

// удалить элемент из списка
Liver pop(Node **head) {
    if (!head || !(*head)) {
        exit(-1); // handle invalid head pointer
    }
    Node *prev = (*head);
    Liver val = prev->value;
    (*head) = (*head)->next;
    free(prev);
    return val;
}

// поиск n-го элемента
Node* getNth(Node* head, int n) {
    int cnt = 0;
    while(cnt < n && head) {
        head = head->next;
        cnt++;
    }
    return head;
}

// поиск последнего элемента
Node* getLast(Node *head) {
    if (head == NULL) {
        return NULL;
    }
    while (head->next) {
        head = head->next;
    }
    return head;
}

// добавление элемента в конец
void pushBack(Node *head, Liver value) {
    Node *last = getLast(head);
    Node *tmp = (Node*) malloc(sizeof(Node));
    if (!tmp) {
        exit(-1); // handle memory allocation failure
    }
    tmp->value = value;
    tmp->next = NULL;
    last->next = tmp;
}

// удаление последнего элемента
Liver popBack(Node **head) {
    if (!head || !(*head) || !(*head)->next) {
        exit(-1); // handle invalid head or empty list
    }
    Node *curNode = NULL;
    Node *predNode = NULL;
    curNode = *head;
    while (curNode->next) {
        predNode = curNode;
        curNode = curNode->next;
    }
    Liver val = curNode->value;
    if (predNode == NULL) {
        free(*head);
        *head = NULL;
    } else {
        free(predNode->next);
        predNode->next = NULL;
    }
    return val;
}

// вставка на n-е место элемента
void insert(Node *head, unsigned n, Liver val) {
    unsigned i = 0;
    Node *tmp = NULL;
    while (i < n && head->next) {
        head = head->next;
        i++;
    }
    tmp = (Node*) malloc(sizeof(Node));
    if (!tmp) {
        exit(-1); // handle memory allocation failure
    }
    tmp->value = val;
    if (head->next) {
        tmp->next = head->next;
    } else {
        tmp->next = NULL;
    }
    head->next = tmp;
}

// удаление n-го элемента
Liver deleteNth(Node **head, int n) {
    if (!head || !(*head) || n < 0) {
        exit(-1); // handle invalid head pointer or negative index
    }
    if (n == 0) {
        return pop(head);
    } else {
        Node *prev = getNth(*head, n-1);
        if (!prev || !prev->next) {
            exit(-1); // handle invalid index
        }
        Node *elm = prev->next;
        Liver val = elm->value;
        prev->next = elm->next;
        free(elm);
        return val;
    }
}

// удаление списка
void deleteList(Node **head) {
    if (!head || !(*head)) {
        exit(-1); // handle invalid head pointer
    }
    Node* prev = NULL;
    while ((*head)->next) {
        prev = (*head);
        (*head) = (*head)->next;
        free(prev);
    }
    free(*head);
}

// печать списка
void printList(const Node *head) {
    while (head) {
        printf("%d ", head->value);
        head = head->next;
    }
    printf("\n");
}

// void fromArray(Node **head, int *arr, size_t size) {
//     if (!arr || size == 0) {
//         return;
//     }
//     for (size_t i = size - 1; i > 0; --i) {
//         push(head, arr[i]);
//     }
//     push(head, arr[0]);
// }

// int* toArray(const Node *head) {
//     int len = 0;
//     const Node *temp = head;
//     while (temp) {
//         len++;
//         temp = temp->next;
//     }

//     void* values = (void*) malloc(len * sizeof(void*));
//     if (!values) {
//         exit(-1); // handle memory allocation failure
//     }

//     for (int i = 0; i < len; i++) {
//         values[i] = head->value;
//         head = head->next;
//     }
//     return values;
// }

// int main() {
//     Node* head = NULL;
//     int arr[] = {1,2,3,4,5,6,7,8,9,10};
//     fromArray(&head, arr, 10);

//     printList(head);

//     insert(head, 4, 333);
//     printList(head);

//     pushBack(head, 11);
//     pushBack(head, 12);
//     pushBack(head, 13);
//     pushBack(head, 14);
//     printList(head);

//     printf("%d\n", pop(&head));
//     printf("%d\n", popBack(&head));

//     printList(head);

//     deleteNth(&head, 4);
//     printList(head);

//     deleteList(&head);

//     return 0;
// }
