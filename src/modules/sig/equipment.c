#include "equipment.h"

typedef struct equip_t {
    char id[16];
    double x;
    double y;
    char cFill[24];
    char cStroke[24];
    char wStroke[16];
    bool highlighted;
} *EquipPtr;

Equip Equip_Create(char id[], double x, double y, char cFill[], char cStroke[], char wStroke[]) {
    EquipPtr equip = malloc(sizeof(struct equip_t));
    strcpy(equip->id, id);
    equip->x = x;
    equip->y = y;
    equip->highlighted = false;
    strcpy(equip->cFill, cFill);
    strcpy(equip->cStroke, cStroke);
    strcpy(equip->wStroke, wStroke);
    return equip;
}

char *Equip_GetID(Equip equipVoid) {
    EquipPtr equip = (EquipPtr) equipVoid;
    return equip->id;
}

char *Equip_GetCFill(Equip equipVoid) {
    EquipPtr equip = (EquipPtr) equipVoid;
    return equip->cFill;
}

char *Equip_GetCStroke(Equip equipVoid) {
    EquipPtr equip = (EquipPtr) equipVoid;
    return equip->cStroke;
}

char *Equip_GetWStroke(Equip equipVoid) {
    EquipPtr equip = (EquipPtr) equipVoid;
    return equip->wStroke;
}

double Equip_GetX(Equip equipVoid) {
    EquipPtr equip = (EquipPtr) equipVoid;
    return equip->x;
}

void Equip_SetX(Equip equipVoid, double x) {
    EquipPtr equip = (EquipPtr) equipVoid;
    equip->x = x;
}

double Equip_GetY(Equip equipVoid) {
    EquipPtr equip = (EquipPtr) equipVoid;
    return equip->y;
}

void Equip_SetY(Equip equipVoid, double y) {
    EquipPtr equip = (EquipPtr) equipVoid;
    equip->y = y;
}

bool Equip_GetHighlighted(Equip equipVoid) {
    EquipPtr equip = (EquipPtr) equipVoid;
    return equip->highlighted;
}

void Equip_SetHighlighted(Equip equipVoid, bool highlighted) {
    EquipPtr equip = (EquipPtr) equipVoid;
    equip->highlighted = highlighted;
}

void Equip_Destroy(Equip equipVoid) {
    EquipPtr equip = (EquipPtr) equipVoid;
    free(equip);
}