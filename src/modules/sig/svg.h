#ifndef SVG_H
#define SVG_H

#include <stdio.h>
#include <stdbool.h>
#include "circle.h"
#include "rectangle.h"

#define STROKE_WIDTH "2"
#define TEXT_FILL_COLOR "black"
#define POINT_RADIUS 2.0

void putSVGStart(FILE *file);

void putSVGCircle(FILE *file, Circle circ, char color1[], char color2[], int stroke);

void putSVGRectangle(FILE *file, Rectangle rect, char color1[], char color2[], int stroke);

void putSVGEllipse(FILE *file, double cx, double cy, double rx, double ry, char color1[], char color2[]);

void putSVGText(FILE *file, double x, double y, char text[]);

void putSVGPoint(FILE *file, double x, double y, bool inside);

void putSVGBox(FILE *file, double x, double y, double width, double height, bool gaps);

void putSVGLine(FILE *file, double x1, double y1, double x2, double y2);

void putSVGEnd(FILE *file);

#endif