#include<stdio.h>

int main(int argc,char* argv[]){
    if(argc != 3){
       printf("wrong input");
       return 1;
    }

    FILE* file_input = fopen(argv[1], "r");
    FILE* file_output = fopen(argv[2], "w");
    if(file_input == NULL || file_output == NULL){
        printf("file is NULL");
        return 2;
    }

    char ch;
    while(!feof(file_input)){
        ch = getc(file_input);
        putc(ch, file_output);
    }
    fclose(file_input);
    fclose(file_output);
    return 0;
}