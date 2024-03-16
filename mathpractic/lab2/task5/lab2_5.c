#include "../header.h"
enum status_code{
    WRONG_ROMAN_NUMBER = 1,
};


int ArabicToRoman(int num, char* str1[]) {
    char str[10];
    int arabic[] = {1,4,5,9,10,40,50,90,100,400,500,900,1000};
    char* roman[] = {"I","IV","V","IX","X","XL","L","XC","C","CD","D","CM","M"};

    if (num <= 0 || num > 3999) {
        // printf("Неверное значение для римского числа.\n");
        return WRONG_ROMAN_NUMBER;
    }

    int i = 12;
    str[0] = 0;
    while(num!=0)
    {
        while(num>=arabic[i])
        {
            num -= arabic[i];
            strcat(str, roman[i]);
        }
        i--;
    }
    
    sprintf(str, "\n");
    strcpy(*str1, str);
    return OK;
}

int overfprintf(FILE* stream, const char *format, ...) {
    va_list args;
    va_start(args, format);

    while(*format != '\0') {
        if(*format == '%') {
            format++;

            switch(*format){
                case 'd':
                {
                    int tmp = va_arg(args, int);
                    fprintf(stream, "%d", tmp);
                    break;
                }
                case 'f':
                {
                    double tmp = va_arg(args, double);
                    fprintf(stream, "%f", tmp);
                    break;
                }
                case 's':
                {
                    const char* tmp = va_arg(args, char*);
                    fprintf(stream, "%s", tmp);
                    break;
                }
                case 'R':
                {
                    format++;
                    if(*format == 'o'){
                        int tmp = va_arg(args, int);
                        char rome[100];
                        ArabicToRoman(tmp, &rome);
                        fprintf(stream, "%s", rome);
                    }
                }
                case 'Z':
                {
                    format++;
                    if(*format == 'r'){
                        
                    }
                
                }
            }
        } else {
            putc(*format, stream);
        }
        format++;
    }
    va_end(args);
}

int main(){
    overfprintf(stdout, " %Ro", 1);





    return 0;
}