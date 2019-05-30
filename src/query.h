#ifndef QUERY_H
#define QUERY_H

#include <stdio.h>
#include <stdbool.h>
#include "modules/data_structures/static_list.h"
#include "modules/sig/block.h"
#include "modules/sig/equipment.h"
#include "modules/sig/geometry.h"
#include "modules/sig/object.h"
#include "modules/sig/svg.h"
#include "lists.h"

bool Query_Dq(FILE *txtFile, char metric[], char id[], double dist);

bool Query_Del(FILE *txtFile, char id[]);

bool Query_Cbq(FILE *txtFile, double x, double y, double r, char cStrk[]);

bool Query_Crd(FILE *txtFile, char id[]);

bool Query_Trns(FILE *txtFile, double x, double y, double w, double h, double dx, double dy);

#endif