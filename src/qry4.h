#ifndef __qry4__
#define __qry4__

#include "tabelaEspalhamento.h"
#include "lista.h"
#include "quadtree.h"

void dm(FILE *svg, FILE *txt, HashTable ht[4], char cpf[], Lista extraFig);

void de(FILE *txt, QuadTree qt[11], char pcnpj[]);

#endif