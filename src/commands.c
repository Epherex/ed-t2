#include <string.h>
#include "commands.h"
#include "file_util.h"

typedef struct BBParameters {
    FILE *file;
    char *color;
} BBParameters;

bool processGeometry(FILE *entryFile, FILE *outputFile, FILE *outputQryFile, StList objList) {
    int nx = DEFAULT_MAXIMUM;

    int cstroke = 2;
    int rstroke = 2;

    putSVGStart(outputFile);
    if(outputQryFile != NULL)
        putSVGStart(outputQryFile);

    char buffer[128];
    while(fgets(buffer, 100, entryFile) != NULL) {
        char type[16];
        sscanf(buffer, "%15s", type);
        if(strcmp(type, "nx") == 0) {
            sscanf(buffer + 3, "%d", &nx);
            StList_Resize(objList, nx);
        } else if(strcmp(type, "c") == 0) {
            char id[8];
            double radius, x, y;
            char color1[32], color2[32];
            sscanf(buffer + 2, "%s %lf %lf %lf %s %s", id, &radius, &x, &y, color1, color2);
            Circle c = Circle_Create(radius, x, y);
            Object o = Object_Create(id, c, OBJ_CIRC, color1, color2, cstroke);
            if(StList_Add(objList, o) == false) {
                #ifdef __DEBUG__
                printf("Erro: Número máximo de elementos ultrapassado!\n");
                #endif
            }
            putSVGCircle(outputFile, c, color1, color2, cstroke);
            if(outputQryFile != NULL)
                putSVGCircle(outputQryFile, c, color1, color2, cstroke);
        } else if(strcmp(type, "r") == 0) {
            char id[8];
            double width, height, x, y;
            char color1[32], color2[32];
            sscanf(buffer + 2, "%s %lf %lf %lf %lf %s %s", id, &width, &height, &x, &y, color1, color2);
            Rectangle r = Rectangle_Create(width, height, x, y);
            Object *o = Object_Create(id, r, OBJ_RECT, color1, color2, rstroke);
            if(StList_Add(objList, o) == false) {
                #ifdef __DEBUG__
                printf("Erro: Número máximo de elementos ultrapassado!\n");
                #endif
            }
            putSVGRectangle(outputFile, r, color1, color2, rstroke);
            if(outputQryFile != NULL)
                putSVGRectangle(outputQryFile, r, color1, color2, rstroke);
        } else if(strcmp(type, "t") == 0) {
            double x, y;
            char text[128];
            sscanf(buffer + 2, "%lf %lf %128[^\n]", &x, &y, text);
            putSVGText(outputFile, x, y, text);
            //if(outputQryFile != NULL)
            //    putSVGText(outputQryFile, x, y, text);
        }
    }
    putSVGEnd(outputFile);
    return true;
}

void insertSVG(void *o, void *parametersVoid) {
    BBParameters *params = (BBParameters*) parametersVoid;
    FILE *file = params->file;
    if(Object_GetType(o) == OBJ_CIRC) {
        putSVGCircle(file, Object_GetContent(o), Object_GetColor1(o), Object_GetColor2(o), Object_GetStroke(o));
        Rectangle surroundingRect = Rectangle_Create(0, 0, 0, 0);
        getSurroundingRect(o, surroundingRect);
        putSVGRectangle(file, surroundingRect, params->color, "none", 2);
        Rectangle_Destroy(surroundingRect);
    } else {
        putSVGRectangle(file, Object_GetContent(o), Object_GetColor1(o), Object_GetColor2(o), Object_GetStroke(o));
        Rectangle rect = Object_GetContent(o);
        double cx, cy;
        getCenter(o, &cx, &cy);
        double rx = Rectangle_GetWidth(rect)/2, ry = Rectangle_GetHeight(rect)/2;
        putSVGEllipse(file, cx, cy, rx, ry, params->color, "none");
    }
}

bool compareObjectToId(void *objVoid, void *idVoid) {
    Object obj = (Object) objVoid;
    char *id = (char *) idVoid;
    return (strcmp(Object_GetId(obj), id) == 0);
}

bool processQuery(FILE *queryFile, FILE *outputFile, FILE *txtFile, StList objList, char outputDir[], char svgFileName[]) {
    char buffer[128];
    while(fgets(buffer, 128, queryFile) != NULL) {
        int len = strlen(buffer);
        if(buffer[len - 1] != '\n') {
            buffer[len] = '\n';
            buffer[len + 1] = '\0';
        }
        char type[16];
        sscanf(buffer, "%15s", type);
        if(strcmp(type, "o?") == 0) {
            char idA[8], idB[8];
            sscanf(buffer + 3, "%7s %7s", idA, idB);
            Object a = StList_Find(objList, compareObjectToId, idA);
            Object b = StList_Find(objList, compareObjectToId, idB);
            if(a == NULL || b == NULL) {
                printf("Erro: Elemento não encontrado!\n");
                return false;
            }
            fputs(buffer, txtFile);
            bool overlaps = checkOverlap(a, b);
            if(overlaps) {
                fprintf(txtFile, "SIM\n\n");
            } else {
                fprintf(txtFile, "NAO\n\n");
            }
            Rectangle rectA = Rectangle_Create(0, 0, 0, 0);
            getSurroundingRect(a, rectA);
            Rectangle rectB = Rectangle_Create(0, 0, 0, 0);
            getSurroundingRect(b, rectB);
            double minX = Rectangle_GetX(rectA) < Rectangle_GetX(rectB) ? Rectangle_GetX(rectA) : Rectangle_GetX(rectB);
            double minY = Rectangle_GetY(rectA) < Rectangle_GetY(rectB) ? Rectangle_GetY(rectA) : Rectangle_GetY(rectB);
            double maxX = Rectangle_GetX(rectA) + Rectangle_GetWidth(rectA) < Rectangle_GetX(rectB) + Rectangle_GetWidth(rectB) 
                ? Rectangle_GetX(rectB) + Rectangle_GetWidth(rectB) : Rectangle_GetX(rectA) + Rectangle_GetWidth(rectA);
            double maxY = Rectangle_GetY(rectA) + Rectangle_GetHeight(rectA) < Rectangle_GetY(rectB) + Rectangle_GetHeight(rectB) 
                ? Rectangle_GetY(rectB) + Rectangle_GetHeight(rectB) : Rectangle_GetY(rectA) + Rectangle_GetHeight(rectA);
            putSVGBox(outputFile, minX, minY, maxX - minX, maxY - minY, !overlaps);
            Rectangle_Destroy(rectA);
            Rectangle_Destroy(rectB);
        } else if(strcmp(type, "i?") == 0) {
            char id[8];
            double x, y;
            sscanf(buffer + 3, "%7s %lf %lf", id, &x, &y);
            Object o = StList_Find(objList, compareObjectToId, id);
            if(o == NULL) {
                printf("Erro: Elemento não encontrado!\n");
                return false;
            }
            fputs(buffer, txtFile);
            bool inside = checkInside(o, x, y);
            if(inside) {
                fprintf(txtFile, "INTERNO\n\n");
            } else {
                fprintf(txtFile, "NAO INTERNO\n\n");
            }
            double centerX, centerY;
            getCenter(o, &centerX, &centerY);
            putSVGLine(outputFile, centerX, centerY, x, y);
            putSVGPoint(outputFile, x, y, inside);
        } else if(strcmp(type, "d?") == 0) {
            char j[8], k[8];
            sscanf(buffer + 3, "%7s %7s", j, k);
            double c1x, c1y, c2x, c2y;
            Object a = StList_Find(objList, compareObjectToId, j);
            Object b = StList_Find(objList, compareObjectToId, k);
            if(a == NULL || b == NULL) {
                printf("Erro: Elemento não encontrado!\n");
                return false;
            }
            getCenter(a, &c1x, &c1y);
            getCenter(b, &c2x, &c2y);
            double dist = euclideanDistance(c1x, c1y, c2x, c2y);
            fputs(buffer, txtFile);
            fprintf(txtFile, "%lf\n\n", dist);
            putSVGLine(outputFile, c1x, c1y, c2x, c2y);
            char distText[16];
            sprintf(distText, "%lf", dist);
            putSVGText(outputFile, c1x + (c2x - c1x) / 2, c1y + (c2y - c1y) / 2, distText);
        } else if(strcmp(type, "bb") == 0) {
            char nameWithSuffix[128];
            strcpy(nameWithSuffix, svgFileName);
            char suffix[32], color[16];
            sscanf(buffer + 3, "%s %s", suffix, color);
            addSuffix(nameWithSuffix, suffix);
            FILE *bbFile = openFile(outputDir, nameWithSuffix, "w");
            if(bbFile == NULL) {
                return false;
            }
            putSVGStart(bbFile);
            BBParameters params = {bbFile, color};
            StList_Execute(objList, insertSVG, &params);
            putSVGEnd(bbFile);
            fclose(bbFile);
        }
    }
    putSVGEnd(outputFile);
    return true;
}