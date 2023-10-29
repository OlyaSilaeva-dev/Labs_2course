#include <stdio.h>
#include <stdarg.h>

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

    return 0;
}
