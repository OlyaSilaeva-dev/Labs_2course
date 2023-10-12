#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int sum_a(double x, double eps, double* res) {
    long n = 0;
    double cur = 1;
    double sum = cur;
    double prev;
    do {
        n++;
        prev = cur;
        cur = prev * x / n;
        sum += cur;
    } while (fabs(cur) > eps);
    *res = sum;
    return 0;
}

int sum_b(double x, double eps, double* res) {
    long n = 0;
    double cur = 1;
    double sum = cur;
    double prev;
    do {
        prev = cur;
        cur = -prev * x * x / ((2 * n + 2) * (2 * n + 1));
        sum += cur;
        n++;
    } while (fabs(cur) > eps);
    *res = sum;
    return 0;
}

int sum_c(double x, double eps, double* res) {
    if (fabs(x) >= 1) {
        return 1; 
    }

    long n = 0;
    double cur = 1;
    double sum = cur;
    double prev;
    do {
        prev = cur;
        cur = prev * x * x * x * 27 * (n + 1) * (n + 1) * (n + 1) / ((3 * n + 3) * (3 * n + 2) * (3 * n + 1));
        sum += cur;
        n++;
    } while (fabs(cur) > eps);
    *res = sum;
    return 0;
}

int sum_d(double x, double eps, double* res) {
    if (fabs(x) >= 1) {
        return 1; 
    }
    long n = 0;
    double cur = x;
    double sum = cur;
    double prev;
    do {
        prev = cur;
        n++;
        cur *= -(2 * n - 1) * x * x / (2 * n);
        sum += cur;
    } while (fabs(cur) > eps);
    *res = sum;
    return 0;
}

int check_num_lf(char* slovo) {
    char* endptr = NULL;
    double slovo1 = strtod(slovo, &endptr);
    if (*endptr != '\0') {
        return 1; 
    }
    return 0; 
}

int main(int argc, char* argv[]) {
    if (argc != 3) {
        printf("Wrong argc");
        return 1; 
    }

    if (check_num_lf(argv[1]) == 1 || check_num_lf(argv[2]) == 1) {
        printf("Invalid input");
        return 2; 
    }

    double x = strtod(argv[1], NULL);
    double eps = strtod(argv[2], NULL);

    if(eps == 0){
        printf("Wrong epsilon");
        return 2;
    }

    double res_a;
    double res_b;
    double res_c;
    double res_d;

    int ans_a = sum_a(x, eps, &res_a);
    int ans_b = sum_b(x, eps, &res_b);
    int ans_c = sum_c(x, eps, &res_c);
    int ans_d = sum_d(x, eps, &res_d);

    if (ans_a == 0) {
        printf("%.16lf\n", res_a);
    }

    if (ans_b == 0) {
        printf("%.16lf\n", res_b);
    }

    if (ans_c == 0) {
        printf("%.16lf\n", res_c);
    } else {
        printf("incorrect argument");
    }

    if (ans_d == 0) {
        printf("%.16lf\n", res_d);
    } else {
        printf("incorrect argument");
    }

    return 0;
}
