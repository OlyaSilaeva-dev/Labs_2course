#include "BinaryHeap.c"


// typedef struct application {
    
// } application;

status_codes to_stol(char* str, int* num);


status_codes main(int argc, char** argv) {
    if(argc < 4) {
        return WRONG_INPUT;
    }

    int max_priority;
    to_stol(argv[1], &max_priority);
    
    
    // FILE* model_parameters = fopen(argv[2], "r");
    // if (model_parameters == NULL) {
    //     return FILE_NOT_OPEN;
    // }
  
    // for(int i = 3; i < argc; i++){
    //     FILE* application_file = fopen(argv[i], "r");
        

    //     fclose(application_file);
    // }

    // fclose(model_parameters);
    return OK;
}

status_codes to_stol(char* str, int* num){
    char* endptr;
    int num1 = stol(str, &endptr);
    if (endptr != NULL) {
        return WRONG_INPUT;
    }
    (*num) = num1;
    return OK;
}
