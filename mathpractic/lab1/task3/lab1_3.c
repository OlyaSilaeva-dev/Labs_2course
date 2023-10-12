#include<stdio.h>
#include<string.h>
#include<math.h>
#include<stdlib.h>
#include<limits.h>

int CalcResult_q(double eps, double a, double b, double c, double *res){ 
    if (fabs(b) > sqrt(LONG_LONG_MAX) || fabs(a * c) > (LONG_LONG_MAX / 4)){
        return 3; 
    }
    
    if (LONG_LONG_MAX - (b * b) < -4 * a * c){
        return 3; 
    }

    double discr = b * b - 4 * a * c;

    if (a < eps && a > -eps){
        if (b < eps && b > -eps){
            return 1; 
        }
        double x = -c / b;
        if (fabs(x) > HUGE_VAL) {
            return 3; 
        }
        res[0] = x;
    } else if (discr < -eps) {
        return 2; 
    } else if (fabs(discr) < eps) {
        if (a > (LONG_LONG_MAX / 2)) {
            return 3; 
        }
        double x = -b / (2 * a);
        if (fabs(x) > HUGE_VAL) {
            return 3; 
        }
        res[0] = x;
    } else {
        double x1 = (-b + sqrt(discr)) / (2 * a);
        double x2 = (-b - sqrt(discr)) / (2 * a);
        if (fabs(x1) > HUGE_VAL || fabs(x2) > HUGE_VAL) {
            return 3; 
        }
        res[0] = x1;
        res[1] = x2;
    }
    
    return 0; 
}

int CalcResult_m(long long a, long long b, int *res){
    if (a == 0 || b == 0) {
        return 1; 
    }

    if (a > (LLONG_MAX / b)) {
        return 2; 
    }

    if (a % b == 0){
        *res = 0; 
    } else {
        *res = 1; 
    }

    return 0; 
}

int CalcResult_t(double eps, double a, double b, double c, int *res){
    if (a <= 0 || b <= 0 || c <= 0){
        return 1; 
    }

    if (a > HUGE_VAL || b > HUGE_VAL || c > HUGE_VAL) {
        return 2; 
    } else if ((fabs(a * a - (b * b + c * c)) < eps) && (LONG_LONG_MAX - b * b > c * c)  ||
               (fabs(b * b - (a * a + c * c)) < eps) && (LONG_LONG_MAX - a * a > c * c)||
               (fabs(c * c - (a * a + b * b)) < eps) && (LONG_LONG_MAX - a * a > b * b)) {
        *res = 0; 
    } else {
        *res = 2; 
    }

    return 0;    
}


int check_num(char *slovo){
    char *endptr = NULL;
    double slovo1 = strtod(slovo, &endptr);
    if (*endptr != '\0') {
        return 1;
    }
    return 0;
}

int check_num_llint(char *slovo){
    char *endptr = NULL;
    long long slovo1 = strtoll(slovo, &endptr, 10);
    if (*endptr != '\0') {
        return 1;
    }
    return 0;
}

int main(int argc, char* argv[]){
    if (argc < 2 || argc > 6){
        printf("Invalid input format.\n");
        return 1;
    }

    if(argv[1][0] != '-' && argv[1][0] != '/' || strlen(argv[1]) != 2){
        printf("Invalid flag\n");
        return 1;
    }
    char flag = argv[1][1];

    switch (flag)
    {

    case 'q':
        if(argc != 6){
            printf("Invalid input format.\n");
            return 1;
        }

        for(int i = 2; i < argc; i++){
            if(check_num(argv[i]) != 0){
                printf("The argument is not a number");
                return 2;
            }
        }
        double eps1 = strtod(argv[2], NULL);
        double eps = fabs(eps1);
        double a = strtod(argv[3], NULL);
        double b = strtod(argv[4], NULL);
        double c = strtod(argv[5], NULL);

        double res[6][2];
        double answer[6];
        answer[0] = CalcResult_q(eps, a, b, c, res[0]);
        answer[1] = CalcResult_q(eps, a, c, b, res[1]);
        answer[2] = CalcResult_q(eps, b, a, c, res[2]);
        answer[3] = CalcResult_q(eps, b, c, a, res[3]);
        answer[4] = CalcResult_q(eps, c, a, b, res[4]);
        answer[5] = CalcResult_q(eps, c, b, a, res[5]);

        for(int i = 0; i < 6; i++){
            if(answer[i] == 1){
                printf("wrong answer\n");
            } else if(answer[i] == 2){
                printf("the discriminant is less than zero\n");
                continue;
            } else if(answer[i] == 3){
                printf("overflow double\n");
                continue;
            } else {
                printf("%lf %lf\n", res[i][0], res[i][1]);
            }
        }
        break;

    case 'm':
        if(argc != 4){
            printf("Invalid input format.\n");
            return 1;
        }

        for(int i = 2; i < argc; i++){
            if(check_num_llint(argv[i]) != 0){
                printf("The argument is not an integer number");
                return 2;
            }
        }

        char *endptr = NULL;
        long long a1 = strtoll(argv[2], &endptr, 10);
        long long b1 = strtoll(argv[3], &endptr, 10);
        int res1;
        int ans = CalcResult_m(a1, b1, &res1);

        if(ans == 2){
            printf("long long overflow");
        } else if(ans == 1){
            printf("Error: the first and second number cannot be zero\n");
            return 1;
        } else {
            if(res1 == 0){
                printf("the %lld is a multiple of %lld", a1, b1);
            } else {
                printf("the %lld is not a multiple of %lld", a1, b1);
            }
        }

        break;

    case 't':
        if(argc != 6){
            printf("Invalid input format.\n");
            return 1;
        }
        for(int i = 2; i < argc; i++){
            if(check_num(argv[i]) != 0){
                printf("The argument is not a double number");
                return 2;
            }
        }

        double epsilon1 = strtod(argv[2], NULL);
        double epsilon = fabs(epsilon1); 
        double side1 = strtod(argv[3], NULL);
        double side2 = strtod(argv[4], NULL);
        double side3 = strtod(argv[5], NULL);
        int res2;

        int ans1 =  CalcResult_t(epsilon, side1, side2, side3, &res2);
        if(ans1 == 0){
            if(res2 == 0){
                 printf("Yes, these are the sides of a right triangle.");
            } else {
                printf("No, these are not the sides of a right triangle.");
            }
        } else if(ans1 == 1){
            printf("Error: negative or zero side");
        } else {
            printf("double overflow");
        }

        break;

    default:
        printf("Invalid flag");
        return 1;
    }

    return 0;   
}