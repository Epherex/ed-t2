#include "query.h"

typedef struct BBParameters {
    FILE *file;
    char *color;
} BBParameters;

typedef struct InfosCbq {
    double dInfos[3];
    char *cStrk;
    FILE *txtFile;
} InfosCbq;

typedef struct InfosTrns {
    double x, y, w, h, dx, dy;
    FILE *txtFile;
} InfosTrns;

/*void insertBoundingBoxElement(void *o, void *parametersVoid) {
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
}*/

bool compareBlockToCep(void *block, void *cepVoid) {
    char *cep = (char *) cepVoid;
    return (strcmp(Block_GetCep(block), cep) == 0);
}

bool compareEquipToId(void *eq, void *idVoid) {
    char *id = (char *) idVoid;
    return (strcmp(Equip_GetID(eq), id) == 0);
}

bool blockInDistanceL1(void *block, void *dVoid) {
    double *d = (double *) dVoid;
    bool topLeftCornerInside = manhattanDistance(d[0], d[1], Block_GetX(block), Block_GetY(block)) <= d[2];
    bool topRightCornerInside = manhattanDistance(d[0], d[1], Block_GetX(block) + Block_GetW(block), Block_GetY(block)) <= d[2];
    bool bottomLeftCornerInside = manhattanDistance(d[0], d[1], Block_GetX(block), Block_GetY(block) + Block_GetH(block)) <= d[2];
    bool bottomRightCornerInside = manhattanDistance(d[0], d[1], Block_GetX(block) + Block_GetW(block), 
        Block_GetY(block) + Block_GetH(block)) <= d[2];
    return topLeftCornerInside && topRightCornerInside && bottomLeftCornerInside && bottomRightCornerInside;
}

bool blockInDistanceL2(void *block, void *dVoid) {
    double *d = (double *) dVoid;
    bool topLeftCornerInside = euclideanDistance(d[0], d[1], Block_GetX(block), Block_GetY(block)) <= d[2];
    bool topRightCornerInside = euclideanDistance(d[0], d[1], Block_GetX(block) + Block_GetW(block), Block_GetY(block)) <= d[2];
    bool bottomLeftCornerInside = euclideanDistance(d[0], d[1], Block_GetX(block), Block_GetY(block) + Block_GetH(block)) <= d[2];
    bool bottomRightCornerInside = euclideanDistance(d[0], d[1], Block_GetX(block) + Block_GetW(block), 
        Block_GetY(block) + Block_GetH(block)) <= d[2];
    return topLeftCornerInside && topRightCornerInside && bottomLeftCornerInside && bottomRightCornerInside;
}

void putBlockInfo(void *block, void *fileVoid) {
    FILE *file = (FILE *) fileVoid;
    fprintf(fileVoid, "%s ", Block_GetCep(block));
}

void changeBlockColor(void *block, void *infosVoid) {
    InfosCbq *infos = (InfosCbq *) infosVoid;
    if(blockInDistanceL2(block, (void *) infos->dInfos)) {
        fprintf(infos->txtFile, "%s ", Block_GetCep(block));
        Block_SetCStroke(block, infos->cStrk);
    }
}

void translateBlock(void *block, void *infosVoid) {
    InfosTrns *infos = (InfosTrns *) infosVoid;
    if(Block_GetX(block) >= infos->x && Block_GetX(block) + Block_GetW(block) <= infos->x + infos->w &&
          Block_GetY(block) >= infos->y && Block_GetY(block) + Block_GetH(block) <= infos->y + infos->h) {
        fprintf(infos->txtFile, "\n%s:"
                                "\n\tPosição anterior: (%.2lf, %.2lf)"
                                "\n\tNova posição: (%.2lf, %.2lf)",
                                Block_GetCep(block), Block_GetX(block), Block_GetY(block),
                                Block_GetX(block) + infos->dx, Block_GetY(block) + infos->dy);
        Block_SetX(block, Block_GetX(block) + infos->dx);
        Block_SetY(block, Block_GetY(block) + infos->dy);
    }
}

void translateEquip(void *equip, void *infosVoid) {
    InfosTrns *infos = (InfosTrns *) infosVoid;
    if(Equip_GetX(equip) >= infos->x && Equip_GetX(equip) <= infos->x + infos->w &&
          Equip_GetY(equip) >= infos->y && Equip_GetY(equip) <= infos->y + infos->h) {
        fprintf(infos->txtFile, "\n%s:"
                                "\n\tPosição anterior: (%.2lf, %.2lf)"
                                "\n\tNova posição: (%.2lf, %.2lf)",
                                Equip_GetID(equip), Equip_GetX(equip), Equip_GetY(equip),
                                Equip_GetX(equip) + infos->dx, Equip_GetY(equip) + infos->dy);
        Equip_SetX(equip, Equip_GetX(equip) + infos->dx);
        Equip_SetY(equip, Equip_GetY(equip) + infos->dy);
    }
}

bool Query_Dq(FILE *txtFile, StList blockList, StList hydList, StList cTowerList, StList tLightList, char *buffer) {
    char metric[8], id[16];
    double dist;
    sscanf(buffer + 3, "%7s %15s %lf", metric, id, &dist);
    
    Equip e = StList_Find(hydList, compareEquipToId, id);
    if(e == NULL)
        e = StList_Find(cTowerList, compareEquipToId, id);
    if(e == NULL)
        e = StList_Find(tLightList, compareEquipToId, id);
    if(e == NULL) {
        printf("Erro: Elemento não encontrado: %s!\n", id);
        return true;
    }

    fputs(buffer, txtFile);
    fprintf(txtFile, "Equipamento ID: %s\n", Equip_GetID(e));

    bool (*_blockInDistance)(void *, void *);
    if(strcmp(metric, "L1") == 0) {
        _blockInDistance = blockInDistanceL1;
    } else if(strcmp(metric, "L2") == 0) {
        _blockInDistance = blockInDistanceL2;
    } else {
        printf("Métrica não reconhecida: %s\n", metric);
        fprintf(txtFile, "Métrica não reconhecida: %s\n\n", metric);
        return true;
    }

    Equip_SetHighlighted(e, true);
    fprintf(txtFile, "Quadras removidas: ");

    int p;
    double dInfos[3] = {Equip_GetX(e), Equip_GetY(e), dist};
    while(p = StList_FindPos(blockList, _blockInDistance, (void *) dInfos), p != -1) {
        Block b = StList_Get(blockList, p);
        fprintf(txtFile, "%s ", Block_GetCep(b));
        StList_RemoveAt(blockList, p);
        Block_Destroy(b);
    }

    fprintf(txtFile, "\n\n");
    return true;
}

bool Query_Del(FILE *txtFile, StList blockList, StList hydList, StList cTowerList, StList tLightList, char *buffer) {
    char id[16];
    sscanf(buffer + 4, "%15s", id);

    fputs(buffer, txtFile);

    int p = StList_FindPos(blockList, compareBlockToCep, id);
    if(p != -1) {
        Block b = StList_Get(blockList, p);
        fprintf(txtFile, "Informações da quadra removida:\n"
                            "CEP: %s\nPos: (%.2lf, %.2lf)\n"
                            "Largura: %.2lf\nAltura: %.2lf\n\n",
                            Block_GetCep(b), Block_GetX(b), Block_GetY(b), 
                            Block_GetW(b), Block_GetH(b));
        StList_RemoveAt(blockList, p);
        Block_Destroy(b);
        return true;
    }

    p = StList_FindPos(hydList, compareEquipToId, id);
    if(p != -1) {
        Equip e = StList_Get(hydList, p);
        fprintf(txtFile, "Informações do hidrante removido:\n"
                            "ID: %s\nPos: (%.2lf, %.2lf)\n\n",
                            Equip_GetID(e), Equip_GetX(e), Equip_GetY(e));
        StList_RemoveAt(hydList, p);
        Equip_Destroy(e);
        return true;
    }

    p = StList_FindPos(cTowerList, compareEquipToId, id);
    if(p != -1) {
        Equip e = StList_Get(cTowerList, p);
        fprintf(txtFile, "Informações da rádio-base removida:\n"
                            "ID: %s\nPos: (%.2lf, %.2lf)\n\n",
                            Equip_GetID(e), Equip_GetX(e), Equip_GetY(e));
        StList_RemoveAt(cTowerList, p);
        Equip_Destroy(e);
        return true;
    }

    p = StList_FindPos(tLightList, compareEquipToId, id);
    if(p != -1) {
        Equip e = StList_Get(tLightList, p);
        fprintf(txtFile, "Informações do semáforo removido:\n"
                            "ID: %s\nPos: (%.2lf, %.2lf)\n\n",
                            Equip_GetID(e), Equip_GetX(e), Equip_GetY(e));
        StList_RemoveAt(tLightList, p);
        Equip_Destroy(e);
        return true;
    }

    fprintf(txtFile, "Elemento não encontrado\n\n");
    printf("Erro: Elemento não encontrado: %s!\n", id);
    return true;
}

bool Query_Cbq(FILE *txtFile, StList blockList, char *buffer) {
    double x, y, r;
    char cStrk[24];
    sscanf(buffer + 4, "%lf %lf %lf %23s", &x, &y, &r, cStrk);

    fputs(buffer, txtFile);
    fprintf(txtFile, "Quadras que tiveram as bordas alteradas: ");

    InfosCbq infos = {{x, y, r}, cStrk, txtFile};
    StList_Execute(blockList, changeBlockColor, &infos);

    fprintf(txtFile, "\n\n");
    return true;
}

bool Query_Crd(FILE *txtFile, StList blockList, StList hydList, StList cTowerList, StList tLightList, char *buffer) {
    char id[16];
    sscanf(buffer + 5, "%15s", id);

    fputs(buffer, txtFile);

    char eqType[24] = "";
    double x, y;

    Equip e;
    Block b;
    if(b = StList_Find(blockList, compareBlockToCep, id), b != NULL) {
        strcpy(eqType, "Quadra");
        x = Block_GetX(b);
        y = Block_GetY(b);
    } else if(e = StList_Find(hydList, compareEquipToId, id), e != NULL) {
        strcpy(eqType, "Hidrante");
        x = Equip_GetX(e);
        y = Equip_GetY(e);
    } else if(e = StList_Find(cTowerList, compareEquipToId, id), e != NULL) {
        strcpy(eqType, "Rádio-base");
        x = Equip_GetX(e);
        y = Equip_GetY(e);
    } else if(e = StList_Find(tLightList, compareEquipToId, id), e != NULL) {
        strcpy(eqType, "Semáforo");
        x = Equip_GetX(e);
        y = Equip_GetY(e);
    }

    if(eqType[0] != '\0') {
        fprintf(txtFile, "Espécie do equipamento: %s\n"
                            "Pos: (%.2lf, %.2lf)\n\n",
                            eqType, x, y);
    } else {
        fprintf(txtFile, "Elemento não encontrado\n\n");
        printf("Erro: Elemento não encontrado: %s!\n", id);
    }
    return true;
}

bool Query_Trns(FILE *txtFile, StList blockList, StList hydList, StList cTowerList, StList tLightList, char *buffer) {
    InfosTrns infos;
    sscanf(buffer + 5, "%lf %lf %lf %lf %lf %lf", 
            &infos.x, &infos.y, &infos.w, &infos.h, &infos.dx, &infos.dy);
    
    infos.txtFile = txtFile;

    fputs(buffer, txtFile);
    fprintf(txtFile, "Equipamentos movidos:");
    
    StList_Execute(blockList,  translateBlock, (void *) &infos);
    StList_Execute(hydList,    translateEquip, (void *) &infos);
    StList_Execute(cTowerList, translateEquip, (void *) &infos); 
    StList_Execute(tLightList, translateEquip, (void *) &infos);
    
    fprintf(txtFile, "\n\n");

    return true;
}