#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>

#define EPS 1e-8

double CalculateEBySeries(double eps) {
    double cur = 1.0, prev;
    long n = 1;
    double n_fact = 1.0;
    
    do {
        prev = cur;
        n_fact /= (1.0 * n++);
        cur += n_fact;
    } while (fabs(cur - prev) > eps);
    
    return cur;
}

double CalculateEByEquation(double eps) {
    double left = 2.0, right = 3.0, mid;
    double cur, prev;

    do {
        prev = mid;
        mid = (left + right) / 2;
        cur = log(mid);

        if (cur >= 1) {
            right = mid;
        } else {
            left = mid;
        }
    } while (fabs(prev - mid) > eps);

    return mid;
}

double CalculateEByLimit(double eps, double* res) {
    double cur = 0.0, prev;
    long long n = 1;

    do {
        prev = cur;
        cur = pow(1.0 + 1.0 / n, n);

        if (n < LLONG_MAX / 2) {
            n *= 2;
        } else {
            return 1;
        }
    } while (fabs(cur - prev) > eps);

    *res = cur;
    return 0;
}

double CalculatePiBySeries(double eps) {
    const double eps1 = eps / 2.0;
    double cur = 1.0;
    long n = 1;
    double sum = cur;
    do {
        n++;
        cur = 1.0 * (n % 2 == 0 ? -1 : 1) / (2 * n - 1);
        sum += cur;
    } while (fabs(cur) > eps1);
    return 4 * sum;
}

double CalculatePiByEquation(double eps) {
    double left = 3.0, right = 3.5, mid;
    double cur;

    do {
        mid = (left + right) / 2;
        cur = sin(mid);

        if (cur > 0.0) {
            left = mid;
        } else {
            right = mid;
        }
    } while (fabs(left - right) > eps);

    return mid;
}

double CalculatePiByLimit(double eps) {
    const double eps1 = eps / 10;
    double cur = pow(2, 4) / pow(2, 2), prev;
    long n = 1;
    do {
        prev = cur;
        cur = prev * n;
        cur /= pow(2 * n + 1, 2);
        cur *= pow(2, 4) * pow(n + 1, 3);
        cur /= pow(2 * n + 2, 2);
        n++;
    } while (fabs(cur - prev) > eps1);
    return cur;
 }

double CalculateLn2BySeries(double eps) {
    double cur = 1.0;
    long n = 1;
    double sum = 0.0;

    do {
        cur = ((n % 2) == 0 ? -1 : 1) / (double)n;
        sum += cur;
        n++;
    } while (fabs(cur) > eps);

    return sum;
}

double CalculateLn2ByEquation(double eps) {
    double left = 0, right = 1, mid;
    double cur;

    do {
        mid = (left + right) / 2;
        cur = exp(mid);

        if (cur > 2) {
            right = mid;
        } else {
            left = mid;
        }
    } while (fabs(left - right) > eps);

    return mid;
}

double CalculateLn2ByLimit(double eps, double* res) {
    double cur = 0.0, prev;
    long long n = 1;

    do {
        prev = cur;
        cur = n * (pow(2.0, 1.0 / n) - 1.0);

        if (n < LLONG_MAX / 2) {
            n *= 2;
        } else {
            return 1;
        }
    } while (fabs(cur - prev) > eps);

    *res = cur;
    return 0;
}

double CalculateSqrt2BySeries(double eps) {
    double power = pow(2, -2);
    double cur = pow(2, power), res_prev;
    double res = cur;
    do {
        res_prev = res;
        power /= 2;
        cur = pow(2, power);
        res *= cur;
    } while (fabs(res - res_prev) > eps);
    
    return res;
}

double CalculateSqrt2ByEquation(double eps) {
    double left = 1.0, right = 2.0, mid;
    double cur;

    do {
        mid = (left + right) / 2;
        cur = mid * mid;

        if (cur > 2.0) {
            right = mid;
        } else {
            left = mid;
        }
    } while (fabs(left - right) > eps);

    return mid;
}

double CalculateSqrt2ByLimit(double eps) {
    double cur = 0.5, prev;

    do {
        prev = cur;
        cur = prev - (prev * prev - 2.0) / (2.0 * prev);
    } while (fabs(cur - prev) > eps);

    return cur;
}

double CalculateGammaBySeries(double eps) {
    const double eps1 = eps / 100;
    const double PI = 3.14159265359;
    double cur = -pow(PI, 2) / 6 + 1.0 / pow(floor(sqrt(2)), 2) - 1.0 / 2;
    long k = 2;
    double sum = 0;
    double sum1 = cur;
    double sum1_prev;
    do {
        k++;
        cur = 1.0 / pow(floor(sqrt(k)), 2) - 1.0 / k;
        sum1 += cur;
        if (cur == 0) {
        sum += sum1;
        if (fabs(sum1 + sum1_prev) < eps1) {
            break;
        }
        sum1_prev = sum1;
        sum1 = 0;
        }
    } while (1);
    return sum;
}

int IsPrime(double num) {

    if (num <= 1) {
        return 0; 
    }
    
    for (int i = 2; i <= sqrt(num); i++) {
        if (fmod(num, i) == 0) {
            return 0; 
        }
    }
    
    return 1; 
}

int CalculateGammaByEquation(double eps, double* res) {
    double cur = 0.0, prev = 0.0;
    long t = 2;
    double ln_t = log(t);
    double pr = 1.0;

    while (1) {
        if (IsPrime(t)) {
            pr *= (t - 1.0) / t;
        }
        cur = -log(log(t) * pr);
        if (fabs(cur - prev) < eps) {
            break;
        }
        prev = cur;
        t++;
    }

    *res = cur;
    return 0;
}


int CalculateGammaByLimit(double eps, double* res) {
    double cur = 1 - log(1), prev;
    long n = 1;
    do {
        prev = cur;
        n *= 2;
        double sum = 1 - log(n);
        for (int i = 2; i <= n; i++) {
            sum += 1.0 / i;
        }
        cur = sum;
    } while (fabs(cur - prev) > eps);

    *res = cur;
    return 0;
 }

int main(int argc, char* argv[]) {
    if (argc != 2) {
        printf("Invalid input format.\n");
        return 1;
    }

    char* endptr = NULL;
    double eps = strtod(argv[1], &endptr);
    
    if (*endptr != '\0') {
        printf("Invalid value\n");
        return 2;
    } else if(eps < EPS){
        printf("Too small value");
        return 2;
    }

    double e_series = CalculateEBySeries(eps);
    printf("Number e (series sum): %.16lf\n", e_series);

    double e_equation = CalculateEByEquation(eps);
    printf("Number e (equation): %.16lf\n", e_equation);

    double e_limit_res;
    int e_limit = CalculateEByLimit(eps, &e_limit_res);

    if (e_limit == 0) {
        printf("Number e (limit): %.16lf\n", e_limit_res);
    } else {
        printf("Error calculating number e (limit).\n");
    }

    double pi_series = CalculatePiBySeries(eps);
    printf("Number pi (series sum): %.16lf\n", pi_series);

    double pi_equation = CalculatePiByEquation(eps);
    printf("Number pi (equation): %.16lf\n", pi_equation);

    double pi_limit = CalculatePiByLimit(eps);
    printf("Number pi (limit): %.16lf\n", pi_limit);

    double ln2_series = CalculateLn2BySeries(eps);
    printf("ln(2) (series sum): %.16lf\n", ln2_series);

    double ln2_equation = CalculateLn2ByEquation(eps);
    printf("ln(2) (equation): %.16lf\n", ln2_equation);

    double ln2_limit_res;
    int ln2_limit = CalculateLn2ByLimit(eps, &ln2_limit_res);
    
    if (ln2_limit == 0) {
        printf("ln(2) (limit): %.16lf\n", ln2_limit_res);
    } else {
        printf("Error calculating ln(2) (limit).\n");
    }

    double sqrt2_series = CalculateSqrt2BySeries(eps);
    printf("Square root of 2 (series sum): %.16lf\n", sqrt2_series);

    double sqrt2_equation = CalculateSqrt2ByEquation(eps);
    printf("Square root of 2 (equation): %.16lf\n", sqrt2_equation);

    double sqrt2_limit = CalculateSqrt2ByLimit(eps);
    printf("Square root of 2 (limit): %.16lf\n", sqrt2_limit);

    double gamma_series = CalculateGammaBySeries(eps);
    printf("Gamma (series sum): %.16lf\n", gamma_series);
    
    double gamma_equation_res;
    int gamma_equation = CalculateGammaByEquation(eps, &gamma_equation_res);
    if (gamma_equation == 0) {
        printf("Gamma (equation): %.16lf\n", gamma_equation_res);
    } else {
        printf("Error calculating Gamma (equation).\n");
    }


    double gamma_limit_res;
    int gamma_limit = CalculateGammaByLimit(eps, &gamma_limit_res);
    if (gamma_limit == 0) {
        printf("Gamma (limit): %.16lf\n", gamma_limit_res);
    } else {
        printf("Error calculating Gamma (limit).\n");
    }

    return 0;
}
