#ifndef __qry3__
#define __qry3__
#include <stdio.h>
#include "quadtree.h"
#include "lista.h"

void cv(QuadTree qt[11], int n, char cep[], char face, int num);

void soc(FILE* svg, FILE* txt, QuadTree qt[11], int k, char cep[], char face, int num, Lista extraFig);

void ci(FILE* svg, FILE* txt, QuadTree qt[11], double x, double y, double r, Lista extraFig);

#endif