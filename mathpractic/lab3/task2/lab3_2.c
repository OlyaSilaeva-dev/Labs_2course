#include "../header.h"

void print_vector(double* vector, int dim) {
    for (int i = 0; i < dim; i++) {
        printf("%lf ", vector[i]);

    }
    printf("\n");
}

int norm_of_chebyshev(double* result, double* vector, int dim) {
    if(dim < 1) {
        return WRONG_PARAMETR;
    }
    double maxi = vector[0];

    for (int i = 0; i < dim; i++) {
        if (vector[i] > maxi) {
            maxi = vector[i];
        }
    }
    *result = maxi;
    return OK;
}

int p_norm(double* result, double* vector, int dim, int p) {
    if (p < 1 || dim <= 0) {
        return WRONG_PARAMETR;
    }
    double sum = 0.0;

    for (int i = 0; i < dim; i++) {
        sum += pow(fabs(vector[i]), p);
    }
    *result = pow(sum, 1.0/ p);

    return OK;
}



int a_norm(double* result, double* vector, int dim) {
    if (dim <= 0) {
        return WRONG_PARAMETR;
    }
    int unite_matrix[dim][dim];

    for (int i = 0; i < dim; i++) {
        for(int j = 0; j < dim; j++) {
            if (i == j) {
                unite_matrix[i][j] = 1;
            }
            else {
                unite_matrix[i][j] = 0;
            }
        }
    }

    double res[dim];
    double sum = 0.0;

    for (int i = 0; i < dim; i++) {
        for (int j = 0; j < dim; j++) {
            sum += unite_matrix[i][j] * vector[j];
        }
        res[i] = sum;
    }
    double pow_res = 0.0;
    for (int i = 0; i < dim; i++) {
        pow_res += pow(res[i], 2);
    }
    *result = sqrt(pow_res);
    return OK;
}

int super_function
(
    double* result, double** result_vector, int dim, int p,
    int (*norm_of_chebyshev)(double*, double*, int),
    int (*p_norm)(double*, double*, int, int),
    int (*a_norm)(double*, double*, int),
    int count, ...
)
{

    va_list(ptr);
    va_start(ptr, count);

    double maxi_res[3];
    maxi_res[0] = INT_MIN;
    maxi_res[1] = INT_MIN;
    maxi_res[2] = INT_MIN;

    for (int i = 0; i < count; i++) {
        double* vector = va_arg(ptr, double*);
        
        double result_[3];
        
        if (norm_of_chebyshev(&(result_[0]), vector, dim) != OK) {
            return WRONG_PARAMETR;
        }

        if (p_norm(&(result_[1]), vector, dim, p) != OK) {
            return WRONG_PARAMETR;
        }

        if (a_norm(&(result_[2]), vector, dim) != OK) {
            return WRONG_PARAMETR;
        }
        for (int j = 0; j < 3; j++) {
            if(result_[j] > maxi_res[j]) {
                maxi_res[j] = result_[j];
                result_vector[j] = vector;//vector
                result[j] = maxi_res[j];//answer value
            }
        }
    }
    return OK;
}

int main() {
    int p = 1;
    int dim = 3;
    double* vector1;
    double* vector2;
    double* vector3;

    vector1 = (double*)malloc(sizeof(double) * dim);
    if (vector1 == NULL) {
        print_err(NO_MEMORY);
        return NO_MEMORY;
    }
    vector1[0] = 0.2;
    vector1[1] = 1.0;
    vector1[2] = 2.2;

    vector2 = (double*)malloc(sizeof(double) * dim);
    if (vector2 == NULL) {
        print_err(NO_MEMORY);
        return NO_MEMORY;
    }
    vector2[0] = 1.1;
    vector2[1] = 0.5;
    vector2[2] = 3.4;

    vector3 = (double*)malloc(sizeof(double) * dim);
    if (vector3 == NULL) {
        print_err(NO_MEMORY);
        return NO_MEMORY;
    }
    vector3[0] = 1.2;
    vector3[1] = 0.9;
    vector3[2] = 3.0;

    double result[3];
    double* result_vector[3];
    int count = 3;

    super_function(result, result_vector, dim, p, norm_of_chebyshev, p_norm, a_norm, count, vector1, vector2, vector3);

    for(int i = 0; i < count; i++) {
        printf("For norm number %d: ", i + 1);
        print_vector(result_vector[i], dim);
        printf("value of vector: %lf\n", result[i]);
    }
    free(vector1);
    free(vector2);
    free(vector3);
    return OK;
}
