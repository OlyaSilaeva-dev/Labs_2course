#include<stdio.h>

int mySum(int a, int b){
    int carry = 0;
    while(b != 0){
        carry = a & b;
        a = a ^ b;
        b = (carry << 1);
    }
    return a;
}

int myDiff(int a, int b){
    int borrow = 0;
    while(b != 0){
        borrow = ((~a) & b);
        a = a ^ b;
        b = (borrow << 1);
    }
    return a;

}

char int2char(int num){
    if (num >= 0 && num <= 9){
        return  mySum((int)'0', num);
    }

    return mySum((int)'A',myDiff(num, 10));
}

int conversion(int num, int r, char* res, int* size){
    int b;
    int index = 0;
    int k = 1;

    for(int m = 0; m < r; m++){
        k = k | (1 << m);
    }
    res[0] = int2char(num & k) ;

    int num1 = num;
    while(1){
        index++;
        num1 = num1 >> r;
        if(num1 == 0){
            break;
        }
        b = num1 & k;
        res[index] = int2char(b);
    }
    *size = index;
}


int main(){
    int num, system, size;
    char res[BUFSIZ];
    if(scanf("%d%d", &num, &system) != 2){
        printf("Wrong input");
        return 1;
    }

    if(system > 5 || system < 1){
        printf("Wrong system");
        return 2;
    }

    conversion(num, system, res, &size);

    for(int i = size - 1; i >= 0; i--){
        printf("%c", res[i]);
    }

    return 0;
}