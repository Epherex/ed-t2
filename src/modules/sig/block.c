#include "block.h"

typedef struct block_t {
    char cep[16];
    double x;
    double y;
    double w;
    double h;
    char cFill[24];
    char cStroke[24];
    char wStroke[16];
} *BlockPtr;

Block Block_Create(char cep[], double x, double y, double w, double h, 
                   char cFill[], char cStroke[], char wStroke[]) {
    BlockPtr block = malloc(sizeof(struct block_t));
    strcpy(block->cep, cep);
    block->x = x;
    block->y = y;
    block->w = w;
    block->h = h;
    strcpy(block->cFill, cFill);
    strcpy(block->cStroke, cStroke);
    strcpy(block->wStroke, wStroke);
    return block;
}

char *Block_GetCep(Block blockVoid) {
    BlockPtr block = (BlockPtr) blockVoid;
    return block->cep;
}

char *Block_GetCFill(Block blockVoid) {
    BlockPtr block = (BlockPtr) blockVoid;
    return block->cFill;
}

char *Block_GetCStroke(Block blockVoid) {
    BlockPtr block = (BlockPtr) blockVoid;
    return block->cStroke;
}

void Block_SetCStroke(Block blockVoid, char *cStroke) {
    BlockPtr block = (BlockPtr) blockVoid;
    strcpy(block->cStroke, cStroke);
}

char *Block_GetWStroke(Block blockVoid) {
    BlockPtr block = (BlockPtr) blockVoid;
    return block->wStroke;
}

double Block_GetX(Block blockVoid) {
    BlockPtr block = (BlockPtr) blockVoid;
    return block->x;
}

void Block_SetX(Block blockVoid, double x) {
    BlockPtr block = (BlockPtr) blockVoid;
    block->x = x;
}

double Block_GetY(Block blockVoid) {
    BlockPtr block = (BlockPtr) blockVoid;
    return block->y;
}

void Block_SetY(Block blockVoid, double y) {
    BlockPtr block = (BlockPtr) blockVoid;
    block->y = y;
}

double Block_GetW(Block blockVoid) {
    BlockPtr block = (BlockPtr) blockVoid;
    return block->w;
}

double Block_GetH(Block blockVoid) {
    BlockPtr block = (BlockPtr) blockVoid;
    return block->h;
}

void Block_Destroy(Block blockVoid) {
    BlockPtr block = (BlockPtr) blockVoid;
    free(block);
}
