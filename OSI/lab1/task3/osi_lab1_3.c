#include<stdio.h>

int main(int argc,char* argv[]){
    if(argc != 3){
       printf("wrong input");
       return 1;
    }

    FILE* file_input = fopen(argv[1], "rb");
    if(file_input == NULL){
        printf("file1 is NULL");
        return 2;
    }

    FILE* file_output = fopen(argv[2], "wb");
    if(file_output == NULL){
        printf("file2 is NULL");
        fclose(file_input);
        return 2;
    }

    char ch;
    char buffer[1024];
    size_t bytes_read;

    while((bytes_read = fread(buffer, 1, sizeof(buffer), file_input)) > 0){
        fwrite(buffer, 1, bytes_read, file_output);
    }

    fclose(file_input);
    fclose(file_output);
    return 0;
}