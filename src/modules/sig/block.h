#ifndef BLOCK_H
#define BLOCK_H

#include <stdlib.h>
#include <string.h>

typedef void *Block;

Block Block_Create(char cep[], double x, double y, double w, double h, char cFill[], char cStroke[], char wStroke[]);

char *Block_GetCep(Block block);

char *Block_GetCFill(Block block);

char *Block_GetCStroke(Block block);

void Block_SetCStroke(Block block, char *cStroke);

char *Block_GetWStroke(Block block);

double Block_GetX(Block block);

void Block_SetX(Block block, double x);

double Block_GetY(Block block);

void Block_SetY(Block block, double y);

double Block_GetW(Block block);

double Block_GetH(Block block);

void Block_Destroy(Block block);

#endif
