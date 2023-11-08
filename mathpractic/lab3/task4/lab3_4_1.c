#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_SIZE 100
#define STRING_SIZE 50

typedef struct {
    char city[STRING_SIZE];
    char street[STRING_SIZE];
    unsigned int houseNumber;
    char building[STRING_SIZE];
    unsigned int apartmentNumber;
    char index[7];
} Address;

typedef struct {
    Address recipientAddress;
    float weight;
    char postIdentifier[15];
    char createdAt[20];
    char deliveryTime[20];
} Mail;

typedef struct {
    Address* currentPostOffice;
    Mail* mails;
    int mailCount;
} Post;

void addMail(Post* post) {
    if (post->mailCount == MAX_SIZE) {
        printf("Максимальное количество посылок достигнуто.\n");
        return;
    }

    Mail newMail;
    printf("Введите данные посылки:\n");
    printf("Город: ");
    scanf("%s", newMail.recipientAddress.city);
    printf("Улица: ");
    scanf("%s", newMail.recipientAddress.street);
    printf("Номер дома: ");
    scanf("%u", &newMail.recipientAddress.houseNumber);
    printf("Корпус: ");
    scanf("%s", newMail.recipientAddress.building);
    printf("Номер квартиры: ");
    scanf("%u", &newMail.recipientAddress.apartmentNumber);
    printf("Индекс получателя: ");
    scanf("%s", newMail.recipientAddress.index);
    printf("Вес посылки: ");
    scanf("%f", &newMail.weight);
    printf("Почтовый идентификатор: ");
    scanf("%s", newMail.postIdentifier);
    printf("Время создания (dd:MM:yyyy hh:mm:ss): ");
    scanf("%s", newMail.createdAt);
    strcpy(newMail.deliveryTime, "");

    post->mails[post->mailCount++] = newMail;
}

void deleteMail(Post* post, char* postIdentifier) {
    for (int i = 0; i < post->mailCount; i++) {
        if (strcmp(post->mails[i].postIdentifier, postIdentifier) == 0) {
            for (int j = i; j < post->mailCount - 1; j++) {
                post->mails[j] = post->mails[j + 1];
            }
            post->mailCount--;
            printf("Посылка с идентификатором %s удалена.\n", postIdentifier);
            return;
        }
    }
    printf("Посылка с указанным идентификатором не найдена.\n");
}

void printMailInformation(Mail* mail) {
    printf("Город: %s\n", mail->recipientAddress.city);
    printf("Улица: %s\n", mail->recipientAddress.street);
    printf("Номер дома: %u\n", mail->recipientAddress.houseNumber);
    printf("Корпус: %s\n", mail->recipientAddress.building);
    printf("Номер квартиры: %u\n", mail->recipientAddress.apartmentNumber);
    printf("Индекс получателя: %s\n", mail->recipientAddress.index);
    printf("Вес посылки: %.2f\n", mail->weight);
    printf("Почтовый идентификатор: %s\n", mail->postIdentifier);
    printf("Время создания: %s\n", mail->createdAt);
    if (strcmp(mail->deliveryTime, "") != 0) {
        printf("Время вручения: %s\n", mail->deliveryTime);
    }
}

void printDeliveredMails(Post* post) {
    printf("Доставленные отправления:\n");
    for (int i = 0; i < post->mailCount; i++) {
        if (strcmp(post->mails[i].deliveryTime, "") != 0) {
            printMailInformation(&post->mails[i]);
        }
    }
}

void printExpiredMails(Post* post) {
    time_t now;
    struct tm* timeinfo;
    time(&now);
    timeinfo = localtime(&now);

    printf("Истекшие отправления:\n");
    for (int i = 0; i < post->mailCount; i++) {
        struct tm mailTime;
        sscanf(post->mails[i].createdAt, "%d:%m:%Y %H:%M:%S", &mailTime);
        time_t mailTimestamp = mktime(&mailTime);

        if (difftime(now, mailTimestamp) > 0 && strcmp(post->mails[i].deliveryTime, "") == 0) {
            printMailInformation(&post->mails[i]);
        }
    }
}

int compareMails(const void* a, const void* b) {
    Mail* mailA = (Mail*)a;
    Mail* mailB = (Mail*)b;
    int indexComparison = strcmp(mailA->recipientAddress.index, mailB->recipientAddress.index);
    if (indexComparison == 0) {
        return strcmp(mailA->postIdentifier, mailB->postIdentifier);
    }
    return indexComparison;
}

void sortMails(Post* post) {
    qsort(post->mails, post->mailCount, sizeof(Mail), compareMails);
}

int main() {
    Post post;
    post.currentPostOffice = (Address*)malloc(sizeof(Address));

    post.mails = (Mail*)malloc(MAX_SIZE * sizeof(Mail));
    post.mailCount = 0;

    int option;
    while (1) {
        printf("\nВыберите опцию:\n");
        printf("1. Добавить отправление\n");
        printf("2. Удалить отправление по идентификатору\n");
        printf("3. Найти информацию об отправлении по идентификатору\n");
        printf("4. Показать все доставленные отправления\n");
        printf("5. Показать все истекшие отправления\n");
        printf("6. Выход\n");
        scanf("%d", &option);

        switch (option) {
            case 1:
                addMail(&post);
                sortMails(&post);
                break;
            case 2: {
                char identifier[15];
                printf("Введите идентификатор для удаления: ");
                scanf("%s", identifier);
                deleteMail(&post, identifier);
                sortMails(&post);
                break;
            }
            case 3: {
                char identifier[15];
                printf("Введите идентификатор для поиска: ");
                scanf("%s", identifier);
                for (int i = 0; i < post.mailCount; i++) {
                    if (strcmp(post.mails[i].postIdentifier, identifier) == 0) {
                        printf("Информация об отправлении с идентификатором %s:\n", identifier);
                        printMailInformation(&post.mails[i]);
                        break;
                    }
                    if (i == post.mailCount - 1) {
                        printf("Отправление с указанным идентификатором не найдено.\n");
                    }
                }
                break;
            }
            case 4:
                printDeliveredMails(&post);
                break;
            case 5:
                printExpiredMails(&post);
                break;
            case 6:
                printf("Выход из программы.\n");
                free(post.currentPostOffice);
                free(post.mails);
                return 0;
            default:
                printf("Неверная опция. Пожалуйста, выберите существующую опцию.\n");
                break;
        }
    }
}
