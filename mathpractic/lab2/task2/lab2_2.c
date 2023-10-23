#include<stdio.h>
#include<stdarg.h>
#include<math.h>


double geometricAverage(int n, ...){
    va_list args;
    va_start(args, n);

    double product = 1.0;
    for(int i = 0; i < n; i++){
        double num = va_arg(args, double);
        product *= num;
    }
    va_end(args);

    double geometric_mean_value = pow(product, 1.0 / n);
    return geometric_mean_value;
}

double power_recursive(double base, int exponent) {
    if (exponent == 0) {
        return 1;
    }
    if (exponent % 2 == 0) {
        double temp = power_recursive(base, exponent / 2);
        return temp * temp;
    } else {
        return base * power_recursive(base, exponent - 1);
    }
}

int main() {
    double avg_geo = geometricAverage(3, 2.0, 8.0, 32.0);
    printf("arithmetic mean: %lf\n", avg_geo);

    double base = 2.5;
    int exponent = 3;
    double result = power_recursive(base, exponent);
    printf("%lf to the power of %d = %lf\n", base, exponent, result);

    return 0;
}