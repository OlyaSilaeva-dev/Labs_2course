#include "../header.h"

char* reverse(const char* str) {
    size_t len = strlen(str);
    char* str1 = (char*)malloc(sizeof(char) * (len + 1));
    if (str1 == NULL) {
        return NULL;
    }
    for (size_t i = 0; i < len; i++) {
        str1[i] = str[len - i - 1];
    }
    str1[len] = '\0';
    return str1;
}

int charToInt(char num) {
    if (isdigit(num)) {
        return num - '0';
    } else if (isalpha(num)) {
        return toupper(num) - 'A' + 10;
    } else {
        return 0;
    }
}

int sum_two_args(char* a, char* b, int base, char** tempResult) {
    char* revA = reverse(a);
    char* revB = reverse(b);

    size_t lena = strlen(revA);
    size_t lenb = strlen(revB);
    size_t len = lena > lenb ? lena : lenb;

    char* res = (char*)malloc(sizeof(char) * (len + 2));
    if (res == NULL) {
        free(revA);
        free(revB);
        return NO_MEMORY;
    }

    size_t i;
    int ost = 0;
    for (i = 0; i < lena || i < lenb; i++) {
        int num1 = (i < lena) ? charToInt(revA[i]) : 0;
        int num2 = (i < lenb) ? charToInt(revB[i]) : 0;

        int sum = num1 + num2 + ost;
        ost = sum / base;
        int tmp = sum % base; 
        if (0 <= tmp && tmp <= 9) {
            res[i] = tmp + '0';
        } else {
            res[i] = tmp - 10 + 'A';
        }
    }

    if (ost != 0) {
        if (0 <= ost && ost <= 9) {
            res[i++] = ost + '0';
        } else {
            res[i++] = ost - 10 + 'A';
        }
    }
    
    res[i] = '\0';
    char* res1 = reverse(res);
    *tempResult = res1;
    
    free(res);
    free(revA);
    free(revB);

    return OK;
}

int sums(char** res, int base, int count, ...) {
    va_list args;
    va_start(args, count);

    char* result = (char*)malloc(2 * sizeof(char));
    if (result == NULL) {
        va_end(args);
        return NO_MEMORY;
    }
    result[0] = '0';
    result[1] = '\0';

    for (int i = 0; i < count; i++) {
        char* current = va_arg(args, char*);
        char* tempResult;
        int ans = sum_two_args(result, current, base, &tempResult);

        if (ans != OK) {
            free(result);
            va_end(args);
            return ans;
        }

        free(result);
        result = tempResult;
    }

    va_end(args);
    *res = result;
    return OK;
}

int main() {
    char* num1 = "A";
    char* num2 = "A";
    char* num3 = "A";
    char* result;

    int ans = sums(&result, 11, 3, num1, num2, num3);

    if (ans == OK) {
        printf("Result: %s\n", result);
        free(result);
        return OK;
    } else {
        printf("Error: %d\n", ans);
        return ans;
    }
}
