#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <limits.h>

#define MaxUsers 100

struct user {
    char log[7];
    int pass;
    int sanc;
};

struct user base[100];
int userCount = 0;


// Объявление функций
int loginInBase(char login[]);
int registration(char login[], int password);
int authenticate(char login[], int password);
int scanLog(char login[]);
int scanPass(int* password);
int registrationUser();
int authorizationUser();
void Howmuch(char timeStr[], char flag[], int* timeValue);

int loginInBase(char login[]){
    for (int i = 0; i < userCount; i++) {
        if (strcmp(base[i].log, login) == 0) {
            return 1;
        }
    }
    return 0;
}

int registration(char login[], int password){
    if (userCount >= MaxUsers) {
        return 1;
    }
    if (loginInBase(login)) {
        return 2;
    }
    strcpy(base[userCount].log, login);
    base[userCount].pass = password;
    base[userCount].sanc = 0;
    userCount++;
    return 0;
}

int authenticate(char login[], int password) {
    for (int i = 0; i < userCount; i++) {
        if (strcmp(base[i].log, login) == 0 && base[i].pass == password) {
            return i;
        }
    }
    return -1;
}


int scanPass(int* password) {
    char passwordStr[7];
    if (fgets(passwordStr, 7, stdin) == NULL) {
        printf("Wrong input");
        return 1;
    }
    getchar();
    char* endptr;
    long pass = strtol(passwordStr, &endptr, 10);
    if (*endptr != '\0') {
        printf("%s", password);
        printf("Wrong password");
        return 1;
    }
    if (pass > 100000 || pass < 0) {
        printf("Password out of range");
        return 1;
    }
    *password = (int)pass;
    return 0;
}

int registrationUser(int* userCount, struct user base[]) {
    char login[8];
    int pin;
    printf("Login: ");
    if (fgets(login, sizeof(login), stdin) == NULL) {
        printf("Failed to read login.\n");
        return 1;
    }
    printf("%s", login);
    getchar();
    printf("Password: ");
    if (scanPass(&pin) == 1) {
        return 2;
    }
    printf("%d", pin);
    getchar();

    int result = registration(login, pin);
    if (result == 1) {
        printf("Base is full\n");
    } else if (result == 2) {
        printf("Login already in use\n");
    }
    return 0;
}

int authorizationUser(int* userCount, struct user base[]) {
    int authorizedUser = -1;
    char login[7];
    int pin;
    printf("Login: ");
    if (fgets(login, 7, stdin) == NULL) {
        printf("Wrong login");
        return 1;
    }

    printf("\nPassword: ");
    if (scanPass(&pin) == 1) {
        return 1;
    }
    authorizedUser = authenticate(login, pin);
    if (authorizedUser >= 0) {
        return 1;
    } else {
        printf("Invalid credentials. Please try again.\n");
    }
    return 0;
}

void Howmuch(char timeStr[], char flag[], int* timeValue) {
    // Реализация функции Howmuch
}

int main() {
    
    printf("Welcome!\n");

    

    while(1){

        int authorizedUser = 0;
        while (1) {
            char c;
            printf("For registration, enter (R), for authorization, enter (A), for exit, enter (E)\n");
            scanf(" %c", &c);
            switch (c) {
                case 'R':
                    if (registrationUser(&userCount, base) == 0) {
                        printf("Registration successful\n");
                        break;
                    }
                case 'A':
                    if (authorizationUser(&userCount, base) == 1) {
                        authorizedUser = 1;
                        printf("Authorization successful\n");
                        break;
                    }
                case 'E':
                    return 0;
            }
        }

        int limit = 3; 
        int remainingRequests = 3;
        int flag1 = 1;
        while (flag1 == 1) {
            if (remainingRequests <= 0) {
                printf("The maximum number of requests has been exceeded. Exit to the authorization menu.\n");
                break;
            }

            printf("\nSelect the command:\n");
            printf("1. Time\n");
            printf("2. Date\n");
            printf("3. Howmuch <time> <flag>\n");
            printf("4. Logout\n");
            printf("5. Sanctions username <number>\n");
            printf("6. Exit the program\n");
            char choice;
            scanf("%c", &choice);

            switch (choice) {
                case '1':
                    // Реализация команды Time
                    break;
                case '2':
                    // Реализация команды Date
                    break;
                case '3':
                    // Реализация команды Howmuch
                    break;
                case '4':
                    return 0;
                    break;
                case '5':
                    // Реализация команды Sanctions
                    break;
                case '6':
                    printf("Exit\n");
                    return 0;
                default:
                    flag1 = 0;
                    break;
            }
        }
    }
    
    return 0;
}
