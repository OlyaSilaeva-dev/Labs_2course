#include<stdio.h>

int main(int argc,char* argv[]){
    if(argc != 2){
       printf("wrong input");
       return 1;
    }
    char* fn = argv[1];
    printf("file name: %s \n", fn);

    FILE *file;
    file = fopen(fn, "wb");
    if(file == NULL){
        printf("Write file is NULL");
        return 2;
    }

    unsigned char data[] = {3, 1, 4, 1, 5, 9, 2, 6, 5, 3, 5};

    fwrite(data, 1, sizeof(data), file);
    fclose(file);

    file = fopen(fn, "rb");
    if(file == NULL){
        printf("Read file is NULL");
        return 2;
    }

    unsigned char b1;
    while (!feof(file)) {
        if (fread(&b1, 1, 1, file) != 1) {
             break;
        }
        printf("%02X ", (int)b1);
        
        printf("_ptr(%p),_cnt(%d),_base(%p),_flag(%d),_file(%d),_charbuf(%d),_bufsiz(%d),_tmpfname(%p) \n", 
           file->_ptr, 
           file->_cnt,
           file->_base,
           file->_flag,
           file->_file,
           file->_charbuf,
           file->_bufsiz,
           file->_tmpfname
         );
   
    }
    fclose(file);

    file = fopen(fn /*argv[1]*/, "rb");
    if(file == NULL){
        printf("Read(2) file is NULL");
    }
    fseek(file, 3, SEEK_SET);
    size_t count = 4;
    unsigned char vd[4];
    if( fread(vd, 1, count, file) == count){
        for(int i = 0; i < count; i++){
            printf("%02X ", vd[i]);
        }
    }

    fclose(file);
    return 0;
}