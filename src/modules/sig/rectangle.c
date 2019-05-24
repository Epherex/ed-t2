#include "rectangle.h"

typedef struct rectangle_t {
    double width, height, x, y;
} *RectanglePtr;

Rectangle Rectangle_Create(double width, double height, double x, double y) {
    RectanglePtr newRect = malloc(sizeof(struct rectangle_t));
    newRect->width = width;
    newRect->height = height;
    newRect->x = x;
    newRect->y = y;
    return newRect;
}

double Rectangle_GetWidth(Rectangle rectangleVoid) {
    RectanglePtr rectangle = (RectanglePtr) rectangleVoid;
    return rectangle->width;
}

double Rectangle_GetHeight(Rectangle rectangleVoid) {
    RectanglePtr rectangle = (RectanglePtr) rectangleVoid;
    return rectangle->height;
}

double Rectangle_GetX(Rectangle rectangleVoid) {
    RectanglePtr rectangle = (RectanglePtr) rectangleVoid;
    return rectangle->x;
}

double Rectangle_GetY(Rectangle rectangleVoid) {
    RectanglePtr rectangle = (RectanglePtr) rectangleVoid;
    return rectangle->y;
}

void Rectangle_SetWidth(Rectangle rectangleVoid, double width) {
    RectanglePtr rectangle = (RectanglePtr) rectangleVoid;
    rectangle->width = width;
}

void Rectangle_SetHeight(Rectangle rectangleVoid, double height) {
    RectanglePtr rectangle = (RectanglePtr) rectangleVoid;
    rectangle->height = height;
}

void Rectangle_SetX(Rectangle rectangleVoid, double x) {
    RectanglePtr rectangle = (RectanglePtr) rectangleVoid;
    rectangle->x = x;
}

void Rectangle_SetY(Rectangle rectangleVoid, double y) {
    RectanglePtr rectangle = (RectanglePtr) rectangleVoid;
    rectangle->y = y;
}

void Rectangle_Destroy(Rectangle rectangleVoid) {
    RectanglePtr rectangle = (RectanglePtr) rectangleVoid;
    free(rectangle);
}