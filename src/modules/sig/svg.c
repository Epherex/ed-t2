#include "svg.h"

void putSVGStart(FILE *file) {
    fputs("<svg>\n", file);
}

void putSVGCircle(FILE *file, Circle circ, char color1[], char color2[], char stroke[]) {
    fprintf(file, "<circle cx=\"%lf\" cy=\"%lf\" r=\"%lf\" stroke=\"%s\" stroke-width=\"%s\" fill=\"%s\" fill-opacity=\"%.2lf\"/>\n",
            Circle_GetX(circ),
            Circle_GetY(circ),
            Circle_GetRadius(circ),
            color1,
            stroke,
            color2,
            POLY_OPACITY);
}

void putSVGRectangle(FILE *file, Rectangle rect, char color1[], char color2[], char stroke[]) {
    fprintf(file, "<rect x=\"%lf\" y=\"%lf\" width=\"%lf\" height=\"%lf\" stroke=\"%s\" stroke-width=\"%s\" fill=\"%s\" fill-opacity=\"%.2lf\"/>\n",
            Rectangle_GetX(rect),
            Rectangle_GetY(rect),
            Rectangle_GetWidth(rect),
            Rectangle_GetHeight(rect),
            color1,
            stroke,
            color2,
            POLY_OPACITY);
}

void putSVGEllipse(FILE *file, double cx, double cy, double rx, double ry, char color1[], char color2[]) {
    fprintf(file, "<ellipse cx=\"%lf\" cy=\"%lf\" rx=\"%lf\" ry=\"%lf\" style=\"fill:%s;stroke:%s;stroke-width:%s\"/>\n",
            cx,
            cy,
            rx,
            ry,
            color2,
            color1,
            STROKE_WIDTH);
}

void putSVGText(FILE *file, double x, double y, char text[]) {
    fprintf(file, "<text x=\"%lf\" y=\"%lf\" fill=\"%s\" %s>%s</text>\n",
            x, y, TEXT_FILL_COLOR, TEXT_EXTRA, text);
}

void putSVGBox(FILE *file, double x, double y, double width, double height, bool gaps) {
    fprintf(file, "<rect x=\"%lf\" y=\"%lf\" width=\"%lf\" height=\"%lf\" stroke=\"black\" fill=\"none\" stroke-width=\"%s\" stroke-dasharray=\"%s\"/>\n",
            x,
            y,
            width,
            height,
            STROKE_WIDTH,
            gaps ? "5" : "0");
}

void putSVGPoint(FILE *file, double x, double y, bool inside) {
    fprintf(file, "<circle cx=\"%lf\" cy=\"%lf\" r=\"%lf\" fill=\"%s\"/>\n",
            x,
            y,
            POINT_RADIUS,
            inside ? "green" : "red");
}

void putSVGLine(FILE *file, double x1, double y1, double x2, double y2) {
    fprintf(file, "<line x1=\"%lf\" y1=\"%lf\" x2=\"%lf\" y2=\"%lf\" style=\"stroke:rgb(0,0,0);stroke-width:%s\"/>\n",
            x1,
            y1,
            x2,
            y2,
            STROKE_WIDTH);
}

void putSVGBlock(Block b, void *fileVoid) {
    FILE *file = (FILE *) fileVoid;
    fprintf(file, "<rect x=\"%lf\" y=\"%lf\" width=\"%lf\" height=\"%lf\" stroke=\"%s\" stroke-width=\"%s\" fill=\"%s\"/>\n",
            Block_GetX(b),
            Block_GetY(b),
            Block_GetW(b),
            Block_GetH(b),
            Block_GetCStroke(b),
            Block_GetWStroke(b),
            Block_GetCFill(b));
    fprintf(file, "<text x=\"%lf\" y=\"%lf\" fill=\"%s\" font-size=\"8\" font-family=\"Arial\" style=\"stroke:#000;stroke-width:0.2px;\">%s</text>\n",
            Block_GetX(b) + 5,
            Block_GetY(b) + 13,
            TEXT_FILL_COLOR,
            Block_GetCep(b));
}

void putSVGHightlight(FILE *file, double x, double y) {
    fprintf(file, "<circle cx=\"%lf\" cy=\"%lf\" r=\"%lf\" stroke=\"%s\" stroke-width=\"%s\" fill=\"%s\" fill-opacity=\"0.5\"/>\n",
            x,
            y,
            HYDRANT_RADIUS + 2,
            HIGHLIGHT_CSTROKE,
            HIGHLIGHT_WSTROKE,
            HIGHLIGHT_CFILL);
}

void putSVGHydrant(Equip h, void *fileVoid) {
    FILE *file = (FILE *) fileVoid;
    if(Equip_GetHighlighted(h))
        putSVGHightlight(file, Equip_GetX(h), Equip_GetY(h));
    fprintf(file, "<circle cx=\"%lf\" cy=\"%lf\" r=\"%lf\" stroke=\"%s\" stroke-width=\"%s\" fill=\"%s\"/>\n",
            Equip_GetX(h),
            Equip_GetY(h),
            HYDRANT_RADIUS,
            Equip_GetCStroke(h),
            Equip_GetWStroke(h),
            Equip_GetCFill(h));
    fprintf(file, "<text x=\"%lf\" y=\"%lf\" fill=\"%s\" font-size=\"7\" style=\"stroke:#000;stroke-width:0.2px;\">H</text>\n",
            Equip_GetX(h) - HYDRANT_RADIUS/2,
            Equip_GetY(h) + HYDRANT_RADIUS/2,
            TEXT_FILL_COLOR);
}

void putSVGTrafficLight(Equip t, void *fileVoid) {
    FILE *file = (FILE *) fileVoid;
    if(Equip_GetHighlighted(t))
        putSVGHightlight(file, Equip_GetX(t), Equip_GetY(t));
    fprintf(file, "<circle cx=\"%lf\" cy=\"%lf\" r=\"%lf\" stroke=\"%s\" stroke-width=\"%s\" fill=\"%s\"/>\n",
            Equip_GetX(t),
            Equip_GetY(t),
            HYDRANT_RADIUS,
            Equip_GetCStroke(t),
            Equip_GetWStroke(t),
            Equip_GetCFill(t));
    fprintf(file, "<text x=\"%lf\" y=\"%lf\" fill=\"%s\" font-size=\"7\" style=\"stroke:#000;stroke-width:0.2px;\">S</text>\n",
            Equip_GetX(t) - HYDRANT_RADIUS/2 + 0.5,
            Equip_GetY(t) + HYDRANT_RADIUS/2,
            TEXT_FILL_COLOR);
}

void putSVGCellTower(Equip c, void *fileVoid) {
    FILE *file = (FILE *) fileVoid;
    if(Equip_GetHighlighted(c))
        putSVGHightlight(file, Equip_GetX(c), Equip_GetY(c));
    fprintf(file, "<circle cx=\"%lf\" cy=\"%lf\" r=\"%lf\" stroke=\"%s\" stroke-width=\"%s\" fill=\"%s\"/>\n",
            Equip_GetX(c),
            Equip_GetY(c),
            HYDRANT_RADIUS,
            Equip_GetCStroke(c),
            Equip_GetWStroke(c),
            Equip_GetCFill(c));
    fprintf(file, "<text x=\"%lf\" y=\"%lf\" fill=\"%s\" font-size=\"7\" style=\"stroke:#000;stroke-width:0.2px;\">T</text>\n",
            Equip_GetX(c) - HYDRANT_RADIUS/2,
            Equip_GetY(c) + HYDRANT_RADIUS/2,
            TEXT_FILL_COLOR);
}

void putSVGEnd(FILE *file) {
    fputs("</svg>\n", file);
}