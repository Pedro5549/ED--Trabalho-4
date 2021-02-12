#ifndef __qry2__
#define __qry2__

#include "quadtree.h"
#include "lista.h"

void dq(FILE *svg, FILE *txt, QuadTree quadtrees[11], int flag, char id[], double r);

void del(FILE *svg, FILE *txt, QuadTree quadtrees[11], char cepid[], Lista extraFig);

void cbq(FILE *txt, QuadTree quadtrees[11], double x, double y, double r, char cstrk[]);

void crd(FILE *txt, QuadTree quadtrees[11], char cepid[]);

void car(FILE *svg, FILE *txt, QuadTree quadtrees[11], double x, double y, double w, double h, Lista extraFig);

#endif