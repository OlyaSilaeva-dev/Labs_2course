#include "header.h"




//
//LoadElements(){
//};

int main(){
    char* choice = (char*)malloc(sizeof(char) * BUFSIZ);
    if(choice == NULL){
        return NO_MEMORY;
    }

    if(scanf("%s", choice) != 1) {
        free(choice);
        return WRONG_INPUT;
    }

    char *token;
    token = strtok(choice, " ");
    // printf("%s", token);
    
    if (strcmp(choice, "Load")) {
        LoadElements();
    } else if(strcmp(choice, "Save")) {
    
    } else if(strcmp(choice, "Rand")) {
    
    } else if(strcmp(choice, "Concat")) {
    
    } else if(strcmp(choice, "Free")) {
    
    } else if(strcmp(choice, "Remove")) {

    } else if(strcmp(choice, "Copy")) {
    
    } else if(strcmp(choice, "Sort")) {
    
    } else if(strcmp(choice, "Shuffle")) {
    
    } else if(strcmp(choice, "Stats")) {
    
    } else if(strcmp(choice, "Print")) {

    } else {
        return WRONG_INPUT;
    }
    

    free(choice);
}