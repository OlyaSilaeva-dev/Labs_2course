#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    char *str;
    int length;
} String;

String createString(const char *value) {
    String newString;
    newString.length = strlen(value);
    newString.str = (char *)malloc((newString.length + 1) * sizeof(char));
    if (newString.str != NULL) {
        strcpy(newString.str, value);
    }
    return newString;
}

void deleteString(String *str) {
    if (str->str != NULL) {
        free(str->str);
        str->str = NULL;
    }
    str->length = 0;
}

int compareString(const String *str1, const String *str2) {
    if (str1->length != str2->length) {
        return str1->length - str2->length;
    }
    return strcmp(str1->str, str2->str);
}

int isEqualString(const String *str1, const String *str2) {
    return strcmp(str1->str, str2->str) == 0;
}

int copyString(String *destination, const String *source) {
    if (destination->str != NULL) {
        free(destination->str);
        destination->str = NULL;
    }
    destination->length = source->length;
    destination->str = (char *)malloc((destination->length + 1) * sizeof(char));
    if (destination->str == NULL) {
        return 1;
    }
    strcpy(destination->str, source->str);
    return 0;
}

 int copyStringToHeap(const String *source, String *newString) {
    newString->length = source->length;
    newString->str = (char *)malloc((newString->length + 1) * sizeof(char));
    if (newString->str == NULL) {
        return 1;
    }
    strcpy(newString->str, source->str);    
    return 0;
}

int concatenateString(String *str1, const String *str2) {
    char* tmp = (char *)realloc(str1->str, (str1->length + str2->length + 1) * sizeof(char));
    if(tmp == NULL){
        return 1;    
    }
    strcat(str1->str, str2->str);
    str1->length += str2->length;
    return 0;
}

// int main() {
//     String str1 = createString("Hello");
//     String str2 = createString("World");

//     printf("String 1: %s\n", str1.str);
//     printf("String 2: %s\n", str2.str);

//     int compare = compareString(&str1, &str2);
//     if(compare > 0){
//         printf("str1 > str2\n");
//     } else if(compare < 0){
//         printf("str1 < str2\n");
//     } else {
//         printf("str1 = str2\n");
//     }
    
//     if (isEqualString(&str1, &str2)) {
//         printf("strings are equal\n");
//     } else {
//         printf("stirngs are not equal\n");
//     }
    
//     if(concatenateString(&str1, &str2) == 0){
//         printf("String 1 after concatenation: %s\n", str1.str);
//     } else {
//         printf("memory allocation error");
//         return 1;
//     }
    
//     if(copyString(&str1, &str2) == 1){
//         printf("memory allocation error");
//     } else {
//         printf("String 1 after copying: %s\n", str1.str);
//     }
    
//     String str3;
//     if(copyStringToHeap(&str2, &str3) == 1){
//         printf("memory allocation error");
//     } else {
//         printf("String 3: %s\n", str3.str);
//     }
//     deleteString(&str1);
//     deleteString(&str2);
//     deleteString(&str3);

//     return 0;
// }
