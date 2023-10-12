#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define MIN_EPS 1e-8
#define DBL_EPSILON 2.2204460492503131e-16

int func_a(double x, double *result) {
    if(x == 0){
        return 1;
    }
    *result = log(1 + x) / x;
    return 0;
}

int func_b(double x, double *result) {
    *result = exp(-(x * x) / 2.0);
    return 0;
}

int func_c(double x, double *result) {
    if(x == 1.0){
        return 1;
    }
    *result = log(1.0 / (1.0 - x));
    return 0;
}

int func_d(double x, double *result) {
    *result = pow(x, x);
    return 0;
}

double trapezoidal_integral(int (*func)(double, double*), double a, double b, int n) {
    double step = (b - a) / (double)n;
    double integral = 0.0;
    
    for (int i = 1; i < n; i++) {
        double x = a + i * step;
        double res1;
        if(func(x, &res1) == 0){
            integral += res1;
        } else {
            continue;        
        }
    }
    double res2, res3;
    if(func(a, &res2) == 0 && func(b, &res3) == 0){
        integral += (res2 + res3) / 2.0;
    }
    integral *= step;
    
    return integral;
}

int integral(int (*func)(double, double*), double eps, double* res) {
    double prev;
    int n = 1;
    double step = 1.0;
    double cur = trapezoidal_integral(func, 0.0, 1.0, n);

    do {
        prev = cur;
        n *= 2;
        cur = trapezoidal_integral(func, 0.0, 1.0, n);
    } while (fabs(cur - prev) > eps);

    *res = cur;
    return 0;
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        printf("Wrong argc");
        return 1;
    }

    char* endptr;
    double eps = strtod(argv[1], &endptr);
    if(*endptr != '\0'){
        printf("Invalid input");
        return 2;
    }
    
    if(eps < MIN_EPS){
        printf("Too small value");
        return 3;
    }

    double res_a;
    double res_b;
    double res_c;
    double res_d;

    int ans_a = integral(func_a, eps, &res_a);
    int ans_b = integral(func_b, eps, &res_b);
    int ans_c = integral(func_c, eps, &res_c);
    int ans_d = integral(func_d, eps, &res_d);

    printf("Result for Function A: %.16lf\n", res_a);
    printf("Result for Function B: %.16lf\n", res_b);
    printf("Result for Function C: %.16lf\n", res_c);
    printf("Result for Function D: %.16lf\n", res_d);

    return 0;
}
