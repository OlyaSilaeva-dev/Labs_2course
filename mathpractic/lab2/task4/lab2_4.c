#include <stdio.h>
#include <stdarg.h>
#include <math.h>

typedef struct{
    double x;
    double y;
} Point;

int orientation(Point a, Point b, Point c){
    double value = (b.y - a.y) * (c.x - b.x) - (b.x - a.x) * (c.y - b.y); 
    if (value == 0) 
        return 0;  // коллинеарные точки
    return (value > 0) ? 1: 2;
}

int isConvexPolygon(int num, ...){
    int cntArgs = 0;
    va_list args;
    va_start(args, num);

    Point a, b, c;
    int direction = 0;

    a = va_arg(args, Point);
    b = va_arg(args, Point);
    c = va_arg(args, Point);
    cntArgs += 6;

    for (int i = 0; i < num; i++){
        int d = orientation(a, b, c);
        if (d == 0) 
            continue;
        if (direction == 0) 
            direction = d;
        else if (direction != d) {
            va_end(args);
            return 0;
        }
        a = b;
        b = c;
        if(cntArgs > num - 2){
            break;
        }
        c = va_arg(args, Point);
        cntArgs += 2;
    }

    va_end(args);
    return 1;
}

double polynomial_value(double point, int degree, ...) {
    va_list coefficients;
    va_start(coefficients, degree);

    double result = 0;
    for (int i = degree; i >= 0; i--) {
        double coeff = va_arg(coefficients, double);
        result += coeff * pow(point, i);
    }

    va_end(coefficients);
    return result;
}

int main(){
    Point p1 = {0, 0};
    Point p2 = {0, 1};
    Point p3 = {1, 1};
    Point p4 = {1, 0};
    
    int result = isConvexPolygon(4, p3, p2, p1, p4);
    if(result){
        printf("The polygon is convex\n");
    } else {
        printf("The polygon is not convex\n");
    }

    double point = 2.5;
    int degree = 3;
    double coeff0 = 2, coeff1 = 4, coeff2 = 3, coeff3 = 1; 

    double result1 = polynomial_value(point, degree, coeff0, coeff1, coeff2, coeff3);
    printf("the value of a polynomial of degree %d at the point %f equal to %f\n", degree, point, result1);


    return 0;
}
