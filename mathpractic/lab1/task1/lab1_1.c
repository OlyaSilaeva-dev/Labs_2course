#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <locale.h>
#include <string.h>

int IsPrime(int n) {
    if (n <= 1) {
        return 0;
    }
    if (n == 2) {
        return 1;
    }
    int d = 2;
    while (d * d <= n) {
        if (n % d == 0) {
            return 2; 
        }
        d++;
    }
    return 1;
}

int Summ(long long *sum, long long n) {
    for (long long i = 1; i <= n; i++) {
        long long l = *sum;
        *sum += i; 
        if (l > *sum) {
            return 0;
        }
    }
    return 1;
}

int Fact(long long *n) {
    if (*n == 0 || *n == 1) {
        *n = 1;
    } else {
        long long result = 1;
        long long max = *n;
        for (long long i = 2; i <= max; i++) { 
            result *= i;
        }
        *n = result;
    }
    return 1;
}

int main(int argc, char *argv[]) {    
    int check = 0;

    if (argc != 3) {
        printf("Invalid format string!!!");
        return 1;
    }

    char *endptr;
    long long g = strtoll(argv[1], &endptr, 10);

    if (*endptr != '\0' || g < 0) {
        printf("Invalid format string!!!");
        return 1;
    }

    if (check == 0) {
        
        if (argv[2][0] == '-' || argv[2][0] == '/') { 

            char k = argv[2][1]; 
            switch (k) {
                case 'h':
                    int flag = 0;
                    if(g == 0){
                        printf("Error! You can't divide by zero");
                    } else {
                        for (int i = 1; i <= 100; i++) {
                            if (i % g == 0) {
                                printf("%d ", i);
                                flag = 1;
                            }
                        }
                        if (flag == 0) { 
                            printf("There are no numbers within 100 multiples of the specified");
                        }
                    }
                    break;

                case 'p':
                    if (IsPrime(g) == 1) {
                        printf("The number %lld is simple!", g); 
                    } else if(IsPrime(g) == 2){
                        printf("%lld is composite number", g);
                    } else {
                        printf("The number %lld is neither simple nor compound", g); 
                    }
                    break;

                case 's':
                    for (int i = 0; i < strlen(argv[1]); i++) {
                        printf("%c ", argv[1][i]);
                    }
                    break;

                case 'e':
                    if (g > 10) {
                        printf("The number must be <= 10"); 
                    } else {
                        for (int i = 1; i <= 10; i++) { 
                            long long base = i;
                            for (int j = 1; j <= g; j++) { 
                                base *= i;
                                printf("%lld ", base); 
                            }
                            printf("\n");
                        }
                    }
                    break;

                case 'a':
                    long long sum = 0;
                    if (Summ(&sum, g) == 1) {
                        printf("%lld", sum);
                    } else {
                        printf("overflow long long");
                    }
                    break;

                case 'f':
                    long long factor = g;
                    if (Fact(&factor) == 1) {
                        printf("%lld", factor);
                    } else {
                        printf("overflow long long");
                    }
                    break;

                default:
                    printf("Invalid format string!!!");
                    return 1;
            }
        } else {
            printf("Invalid format string!!!");
            return 1;
        } 
    } else {
        printf("Invalid format string!!!");
        return 1;
    }


    return 0;
}
