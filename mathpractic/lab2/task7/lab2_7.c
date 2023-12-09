#include "../header.h"

#define MAX_ITERATIONS 1000

double foo(double x) {
    return 0.5 * x + 0.1;
}

int RootByDichotomy(double left, double right, double eps, double (*foo)(double), double* res){
    double cur, resc, resl, resr;
    int iterations = 0;

    while(fabs(right - left) > eps && iterations < MAX_ITERATIONS){
        cur = (right + left) / 2.0;
        resc = foo(cur);
        if(fabs(resc) < eps) {
            break;
        }
        resl = foo(left);
        resr = foo(right);
        if(resl * resc < 0) {
            right = cur; 
        }
        if(resr * resc < 0){
            left = cur;
        }   
        iterations++;
    }
    
    if(iterations == MAX_ITERATIONS) {
        return TOO_MUCH_ITERATIONS;
    }

    *res = cur;
    return OK;
}

int main(){
    double right = -5.0;
    double left = 5.0;
    double epsilon = 0.00000001;
    double result;
    int ans = RootByDichotomy(left, right, epsilon, foo, &result);
    if(ans != OK) {
        printf("too much iterations");
        return ans;
    }
    printf("%lf", result);
    return OK;
}