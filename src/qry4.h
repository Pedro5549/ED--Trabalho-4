#ifndef __qry4__
#define __qry4__

#include "tabelaEspalhamento.h"
#include "lista.h"
#include "quadtree.h"

void moradores(FILE *txt, QuadTree qt[11], HashTable ht[4], char cep[]);

void dm(FILE *svg, FILE *txt, HashTable ht[4], char cpf[], Lista extraFig);

void de(FILE *txt, QuadTree qt[11], char pcnpj[]);

void mud(FILE *svg, FILE *txt, QuadTree qt[11], HashTable ht[4], char cpf[], char cep[], char face, int num, char compl[], Lista extraFig);

void dmprbt(QuadTree qt[11], char t, char saida[], char sfx[]);

#endif