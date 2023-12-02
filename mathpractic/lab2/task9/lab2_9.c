#include "../header.h"

#define MAX_PRIMES 1000

int prime_factors(int num, int** pf1, int* size) {
    int* pf = (int*)malloc(sizeof(int) * MAX_PRIMES);
    if (pf == NULL) {
        return NO_MEMORY;
    }
    int d = 2;

    int ind = 0;
    while(d*d <= num){
        if(num % d == 0){
            pf[ind] = d;
            ind++;
            while (num % d == 0){
               num /= d;
            }
        }
        d++;
    }
    if(num > 1) {
        pf[ind] = num;
        ind++;
    }
    *pf1 = pf;
    *size = ind;
    return OK;
}

int has_finite_representation(bool** res, int base, int count, ...) {
    va_list args;
    va_start(args, count);
    *res = (bool*)malloc(sizeof(bool) * count);
    if (*res == NULL) {
        va_end(args);
        return NO_MEMORY;
    }

    int* pf_base; 
    int size_base = 0;
    int ans2 = prime_factors(base, &pf_base, &size_base);
    if(ans2 != OK) {
        free(res);
        va_end(args);
        return NO_MEMORY;
    }

    for (int i = 0; i < count; i++) {
        double cur = va_arg(args, double);

        double num = cur * 10;
        int den = 10;
        while (num - (int)num != 0) {
            num *= 10;
            den *= 10;
        }
        int num1 = (int)num;
        while (num1 % 2 == 0 && den % 2 == 0) {
                num1 /= 2;
                den /= 2;
        }
        while (num1 % 5 == 0 && den % 5 == 0) {
                num1 /= 5;
                den /= 5;
        }

        int* pf_den;
        int size_den = 0;
        int ans1 = prime_factors(den, &pf_den, &size_den);
        if(ans1 != OK) {
            free(res);
            va_end(args);
            return NO_MEMORY;
        }
        
        bool flag = true;
        for(int j = 0; j < size_den; j++){
            bool flag1 = false;
            for(int k = 0; k < size_base; k++) {
                if(pf_den[j] == pf_base[k]) {
                    flag1 = true;
                    break;
                }
            }
            if(!flag1) {
                flag = false;
                break;
            }
        }
        (*res)[i] = flag;
        free(pf_den);
    }

    free(pf_base);
    va_end(args);
    return OK;
}

int main() {
    double num1 = 0.25;
    double num2 = 0.333;
    double num3 = 0.4;
    double num4 = 0.5;
    int count = 4;
    bool* res;

    int ans = has_finite_representation(&res, 5, count, num1, num2, num3, num4);

    if(ans != OK) {
        printf("Error: %d", ans);
        return ans;
    } else {
        for(int i = 0; i < count; i++) {
            if (res[i]) {
                printf("num%d: Yes\n", i);
            } else {
                printf("num%d: No\n", i);
            }
        }
    
    }
    
    return OK;
}
