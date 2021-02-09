#ifndef __qry1__
#define __qry1__
#include <stdio.h>
#include "quadtree.h"
#include "retangulo.h"
#include "circulo.h"
#include "texto.h"

void pnt(FILE* txt, QuadTree quadtrees[11], char id[], char corb[], char corp[]);
//Pinta a borda e o preenchimento da forma/texto de acordo com os parametros passados

void delf(FILE* txt, QuadTree quadtrees[11], char id[]);
//Deleta a forma/texto de identificador id

void pontoInterno(FILE* svg, FILE* txt, QuadTree quadtrees[11], char id[], double xPonto,double yPonto);
//Verifica se o ponto é interno a figura e desenha no svg o ponto e a linha

void intersecao(FILE* svg, FILE* txt, QuadTree quadtrees[11], char id1[], char id2[]);
//Seleciona as figuras para realizar o comando i?

void retanguloIntCirculo(Circulo c, Retangulo r, char idc[], char idr[], FILE* txt, FILE* svg);
//Executa comando i? quando as figuras forem um circulo e um retangulo

void circuloInt(Circulo c1, char idc1[], Circulo c2, char idc2[], FILE* txt, FILE* svg);
//Executa comando i? quando as figuras forem dois circulos

void retanguloInt(Retangulo r1, char idr1[], Retangulo r2, char idr2[], FILE* txt, FILE* svg);
//Executa comando i? quando as figuras forem dois retangulos

#endif