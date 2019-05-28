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

bool Query_Dq(FILE *txtFile, StList blockList, StList hydList, StList cTowerList, StList tLightList, char *buffer);

bool Query_Del(FILE *txtFile, StList blockList, StList hydList, StList cTowerList, StList tLightList, char *buffer);

bool Query_Cbq(FILE *txtFile, StList blockList, char *buffer);

bool Query_Crd(FILE *txtFile, StList blockList, StList hydList, StList cTowerList, StList tLightList, char *buffer);

bool Query_Trns(FILE *txtFile, StList blockList, StList hydList, StList cTowerList, StList tLightList, char *buffer);

#endif