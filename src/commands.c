#include <string.h>
#include "commands.h"
#include "file_util.h"

typedef struct BBParameters {
    FILE *file;
    char *color;
} BBParameters;

typedef struct Lists {
    StList objList;
    StList blockList;
    StList hydList;
    StList cTowerList;
    StList tLightList;
} Lists;

bool processQuery(FILE *queryFile, FILE *outputFile, FILE *txtFile, Lists *lists, char outputDir[], char svgFileName[]);

void initializeLists(Lists *lists, int nx, int nb, int nh, int nc, int nt) {
    lists->objList = StList_Create(nx);
    lists->blockList = StList_Create(nb);
    lists->hydList = StList_Create(nh);
    lists->cTowerList = StList_Create(nc);
    lists->tLightList = StList_Create(nt);
}

void resizeLists(Lists *lists, int nx, int nb, int nh, int nc, int nt) {
    StList_Resize(lists->objList, nx);
    StList_Resize(lists->blockList, nb);
    StList_Resize(lists->hydList, nh);
    StList_Resize(lists->cTowerList, nc);
    StList_Resize(lists->tLightList, nt);
}

void destroyLists(Lists *lists) {
    StList_Destroy(lists->objList, Object_Destroy);
    StList_Destroy(lists->blockList, Block_Destroy);
    StList_Destroy(lists->hydList, Equip_Destroy);
    StList_Destroy(lists->cTowerList, Equip_Destroy);
    StList_Destroy(lists->tLightList, Equip_Destroy);
}

void writeObject(Object o, void *param) {
    FILE *svgFile = (FILE *) param;
    if(Object_GetType(o) == OBJ_CIRC) {
        putSVGCircle(svgFile, Object_GetContent(o), Object_GetColor1(o), Object_GetColor2(o), Object_GetStroke(o));
    } else if(Object_GetType(o) == OBJ_RECT) {
        putSVGRectangle(svgFile, Object_GetContent(o), Object_GetColor1(o), Object_GetColor2(o), Object_GetStroke(o));
    } else if(Object_GetType(o) == OBJ_TEXT) {
        putSVGText(svgFile, atof(Object_GetColor1(o)), atof(Object_GetColor2(o)), Object_GetContent(o));
    }
}

void writeSVG(FILE *outputSVGFile, Lists *lists, bool svgTag) {
    if(svgTag)
        putSVGStart(outputSVGFile);
    StList_Execute(lists->objList, writeObject, outputSVGFile);
    StList_Execute(lists->blockList, putSVGBlock, outputSVGFile);
    StList_Execute(lists->hydList, putSVGHydrant, outputSVGFile);
    StList_Execute(lists->tLightList, putSVGTrafficLight, outputSVGFile);
    StList_Execute(lists->cTowerList, putSVGCellTower, outputSVGFile);
    if(svgTag)
        putSVGEnd(outputSVGFile);
}

bool processGeometry(FILE *entryFile, Lists *lists);

void processAll(FILE *entryFile, FILE *outputSVGFile, FILE *outputQryFile, FILE *queryFile, FILE *txtFile, char outputDir[], char svgFileName[]) {
    Lists lists = {NULL, NULL, NULL, NULL, NULL};
    
    processGeometry(entryFile, &lists);
    writeSVG(outputSVGFile, &lists, true);
    
    if(queryFile != NULL) {
        processQuery(queryFile, outputQryFile, txtFile, &lists, outputDir, svgFileName);
        writeSVG(outputQryFile, &lists, false);
        putSVGEnd(outputQryFile);
    }

    destroyLists(&lists);
}

bool processGeometry(FILE *entryFile, Lists *lists) {
    int nx = DEFAULT_MAXIMUM;
    int nb = DEFAULT_MAXIMUM;
    int nh = DEFAULT_MAXIMUM;
    int nc = DEFAULT_MAXIMUM;
    int nt = DEFAULT_MAXIMUM;

    initializeLists(lists, nx, nb, nh, nc, nt);

    char cFillBlock[24];
    char cStrkBlock[24];
    char wStrkBlock[16];

    char cFillHydrant[24];
    char cStrkHydrant[24];
    char wStrkHydrant[16];

    char cFillCellTower[24];
    char cStrkCellTower[24];
    char wStrkCellTower[16];

    char cFillTrafficLight[24];
    char cStrkTrafficLight[24];
    char wStrkTrafficLight[16];

    char wStrkCircle[16] = "2";
    char wStrkRectangle[16] = "2";

    char buffer[128];
    while(fgets(buffer, 100, entryFile) != NULL) {
        char type[16];
        sscanf(buffer, "%15s", type);
        if(strcmp(type, "nx") == 0) {
            int nx1, nb1, nh1, nc1, nt1;
            sscanf(buffer + 3, "%d %d %d %d %d", &nx1, &nb1, &nh1, &nc1, &nt1);

            if(nx != nx1 || nb != nb1 || nh != nh1 || nc != nc1 || nt != nt1) {
                nx = nx1;
                nb = nb1;
                nh = nh1;
                nc = nc1;
                nt = nt1;
                resizeLists(lists, nx, nb, nh, nc, nt);
            }
        } else if(strcmp(type, "c") == 0) {
            char id[8];
            double radius, x, y;
            char color1[32], color2[32];
            sscanf(buffer + 2, "%s %lf %lf %lf %s %s", id, &radius, &x, &y, color1, color2);

            Circle c = Circle_Create(radius, x, y);
            Object o = Object_Create(id, c, OBJ_CIRC, color1, color2, wStrkCircle);

            if(StList_Add(lists->objList, o) == false) {
                #ifdef __DEBUG__
                printf("Erro: Número máximo de elementos ultrapassado!\n");
                #endif
            }
        } else if(strcmp(type, "r") == 0) {
            char id[8];
            double width, height, x, y;
            char color1[32], color2[32];
            sscanf(buffer + 2, "%s %lf %lf %lf %lf %s %s", id, &width, &height, &x, &y, color1, color2);

            Rectangle r = Rectangle_Create(width, height, x, y);
            Object o = Object_Create(id, r, OBJ_RECT, color1, color2, wStrkRectangle);

            if(StList_Add(lists->objList, o) == false) {
                #ifdef __DEBUG__
                printf("Erro: Número máximo de elementos ultrapassado!\n");
                #endif
            }
        } else if(strcmp(type, "t") == 0) {
            char x[24], y[24];
            char *text = malloc(128 * sizeof(char));
            sscanf(buffer + 2, "%23s %23s %128[^\n]", x, y, text);

            Object o = Object_Create("text", text, OBJ_TEXT, x, y, "");

            if(StList_Add(lists->objList, o) == false) {
                #ifdef __DEBUG__
                printf("Erro: Número máximo de elementos ultrapassado!\n");
                #endif
            }
        } else if(strcmp(type, "cq") == 0) {
            sscanf(buffer + 3, "%23s %23s %15s", cFillBlock, cStrkBlock, wStrkBlock);
        } else if(strcmp(type, "ch") == 0) {
            sscanf(buffer + 3, "%23s %23s %15s", cFillHydrant, cStrkHydrant, wStrkHydrant);
        } else if(strcmp(type, "cr") == 0) {
            sscanf(buffer + 3, "%23s %23s %15s", cFillCellTower, cStrkCellTower, wStrkCellTower);
        } else if(strcmp(type, "cs") == 0) {
            sscanf(buffer + 3, "%23s %23s %15s", cFillTrafficLight, cStrkTrafficLight, wStrkTrafficLight);
        } else if(strcmp(type, "q") == 0) {
            char cep[16];
            double x, y, w, h;
            sscanf(buffer + 2, "%15s %lf %lf %lf %lf", cep, &x, &y, &w, &h);

            Block block = Block_Create(cep, x, y, w, h, cFillBlock, cStrkBlock, wStrkBlock);
            
            if(StList_Add(lists->blockList, block) == false) {
                #ifdef __DEBUG__
                printf("Erro: Número máximo de elementos ultrapassado!\n");
                #endif
            }
        } else if(strcmp(type, "h") == 0) {
            char id[16];
            double x, y;
            sscanf(buffer + 2, "%15s %lf %lf", id, &x, &y);
            
            Equip hydrant = Equip_Create(id, x, y, cFillHydrant, cStrkHydrant, wStrkHydrant);
            
            if(StList_Add(lists->hydList, hydrant) == false) {
                #ifdef __DEBUG__
                printf("Erro: Número máximo de elementos ultrapassado!\n");
                #endif
            }
        } else if(strcmp(type, "s") == 0) {
            char id[16];
            double x, y;
            sscanf(buffer + 2, "%15s %lf %lf", id, &x, &y);
            
            Equip trLight = Equip_Create(id, x, y, cFillTrafficLight, cStrkTrafficLight, wStrkTrafficLight);
            
            if(StList_Add(lists->tLightList, trLight) == false) {
                #ifdef __DEBUG__
                printf("Erro: Número máximo de elementos ultrapassado!\n");
                #endif
            }
        } else if(strcmp(type, "rb") == 0) {
            char id[16];
            double x, y;
            sscanf(buffer + 2, "%15s %lf %lf", id, &x, &y);
            
            Equip cellTower = Equip_Create(id, x, y, cFillCellTower, cStrkCellTower, wStrkCellTower);
            
            if(StList_Add(lists->cTowerList, cellTower) == false) {
                #ifdef __DEBUG__
                printf("Erro: Número máximo de elementos ultrapassado!\n");
                #endif
            }
        }
    }
    return true;
}

void insertBoundingBoxElement(void *o, void *parametersVoid) {
    BBParameters *params = (BBParameters*) parametersVoid;
    FILE *file = params->file;
    if(Object_GetType(o) == OBJ_CIRC) {
        putSVGCircle(file, Object_GetContent(o), Object_GetColor1(o), Object_GetColor2(o), Object_GetStroke(o));
        
        Rectangle surroundingRect = Rectangle_Create(0, 0, 0, 0);
        getSurroundingRect(o, surroundingRect);
        
        putSVGRectangle(file, surroundingRect, params->color, "none", "2");
        Rectangle_Destroy(surroundingRect);
    } else if(Object_GetType(o) == OBJ_RECT) {
        putSVGRectangle(file, Object_GetContent(o), Object_GetColor1(o), Object_GetColor2(o), Object_GetStroke(o));
        Rectangle rect = Object_GetContent(o);
        double cx, cy;
        getCenter(o, &cx, &cy);
        double rx = Rectangle_GetWidth(rect)/2, ry = Rectangle_GetHeight(rect)/2;
        putSVGEllipse(file, cx, cy, rx, ry, params->color, "none");
    }
}

bool compareObjectToId(void *obj, void *idVoid) {
    char *id = (char *) idVoid;
    return (strcmp(Object_GetId(obj), id) == 0);
}

bool processQuery(FILE *queryFile, FILE *outputFile, FILE *txtFile, Lists *lists, char outputDir[], char svgFileName[]) {
    putSVGStart(outputFile);
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
            Object a = StList_Find(lists->objList, compareObjectToId, idA);
            Object b = StList_Find(lists->objList, compareObjectToId, idB);
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
            Object o = StList_Find(lists->objList, compareObjectToId, id);
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
            Object a = StList_Find(lists->objList, compareObjectToId, j);
            Object b = StList_Find(lists->objList, compareObjectToId, k);
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
            StList_Execute(lists->objList, insertBoundingBoxElement, &params);
            putSVGEnd(bbFile);
            fclose(bbFile);
        } else if(strcmp(type, "dq") == 0) {
            if(!Query_Dq(txtFile, lists->blockList, lists->hydList, lists->cTowerList, lists->tLightList, buffer))
                return false;
        } else if(strcmp(type, "del") == 0) {
            if(!Query_Del(txtFile, lists->blockList, lists->hydList, lists->cTowerList, lists->tLightList, buffer))
                return false;
        } else if(strcmp(type, "cbq") == 0) {
            if(!Query_Cbq(txtFile, lists->blockList, buffer))
                return false;
        } else if(strcmp(type, "crd?") == 0) {
            if(!Query_Crd(txtFile, lists->blockList, lists->hydList, lists->cTowerList, lists->tLightList, buffer))
                return false;
        } else if(strcmp(type, "trns") == 0) {
            if(!Query_Trns(txtFile, lists->blockList, lists->hydList, lists->cTowerList, lists->tLightList, buffer))
                return false;
        }
    }
    return true;
}