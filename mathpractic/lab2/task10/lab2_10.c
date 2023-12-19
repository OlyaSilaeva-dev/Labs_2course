#include "../header.h"

double summ(double* array, int left, int right) {
    double res = 0.0;
    for (int i = left; i <= right; i++) {
        res += array[i];
    }
    return res;
}

int polynomial_decomposition(double epsilon, double a, double** coef_group, int degree, ...) {
    if (epsilon < 0.0 || degree < 0) {
        return WRONG_INPUT;
    }
    va_list args;
    va_start(args, degree);

    double* coefficients = (double*)calloc((degree+1), sizeof(double));
    if (coefficients == NULL) {
        va_end(args);
        return NO_MEMORY;
    }

    double pow_a = 1;
    for (int i = 0; i <= degree; i++) {
        coefficients[i] = va_arg(args, double) * pow_a;
        pow_a *= a;
    }
    va_end(args);

    *coef_group = (double*)calloc((degree+1), sizeof(double));
    if(coef_group == NULL) {
        free(coefficients);
        return NO_MEMORY;
    }

    (*coef_group)[0] = summ(coefficients, 0, degree);

    int factorial = 1;
    for (int i = 1; i <= degree;i++) {
        factorial *= i;
        if (errno == ERANGE) {
            free(coefficients);
            return OVERFLOWED;
        }
        (*coef_group)[i] = 0.0;
        for (int j = i; j <= degree; j++) {
            coefficients[j] = coefficients[j] * (j - i + 1) / a;
            (*coef_group)[i] += coefficients[j];
        }
        (*coef_group)[i] /= factorial;
    }
    free(coefficients);
    return OK;
}

int main() {
    double* result;
    double k1 = -2.0;
    double k2 = 1.0;
    double k3 = -3.0;
    double k4 = 0.0;
    double k5 = 1.0;
    double epsilon = 0.00001;
    double a = 3.0;
    double degree = 4;
    int ans = polynomial_decomposition(epsilon, a, &result, degree, k1, k2, k3, k4, k5);
    if (ans != OK) {
        print_err(ans);
        free(result);
        return ans;
    }

    for (int i = 0; i <= degree; i++) {
        printf("%lf ", result[i]);
    }
    
    free(result);
    return OK;
}