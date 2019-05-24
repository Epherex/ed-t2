#include "circle.h"

typedef struct circle_t {
    double x, y, radius;
} *CirclePtr;

Circle Circle_Create(double radius, double x, double y) {
    CirclePtr newCircle = malloc(sizeof(struct circle_t));
    newCircle->radius = radius;
    newCircle->x = x;
    newCircle->y = y;
    return newCircle;
}

double Circle_GetRadius(Circle circleVoid) {
    CirclePtr circle = (CirclePtr) circleVoid;
    return circle->radius;
}

double Circle_GetX(Circle circleVoid) {
    CirclePtr circle = (CirclePtr) circleVoid;
    return circle->x;
}

double Circle_GetY(Circle circleVoid) {
    CirclePtr circle = (CirclePtr) circleVoid;
    return circle->y;
}

void Circle_SetRadius(Circle circleVoid, double radius) {
    CirclePtr circle = (CirclePtr) circleVoid;
    circle->radius = radius;
}

void Circle_SetX(Circle circleVoid, double x) {
    CirclePtr circle = (CirclePtr) circleVoid;
    circle->x = x;
}

void Circle_SetY(Circle circleVoid, double y) {
    CirclePtr circle = (CirclePtr) circleVoid;
    circle->y = y;
}

void Circle_Destroy(Circle circleVoid) {
    Circle circle = (Circle) circleVoid;
    free(circle);
}