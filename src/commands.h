#ifndef COMMANDS_H
#define COMMANDS_H

#include <stdio.h>
#include "modules/data_structures/static_list.h"
#include "modules/sig/object.h"
#include "modules/sig/svg.h"
#include "modules/sig/geometry.h"

#define DEFAULT_MAXIMUM 1000

bool processGeometry(FILE *entryFile, FILE *outputFile, FILE *outputQryFile, StList objList);

bool processQuery(FILE *queryFile, FILE *outputFile, FILE *txtFile, StList objList, char outputDir[], char svgFileName[]);

#endif