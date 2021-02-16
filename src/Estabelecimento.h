#ifndef __Estabelecimento__
#define __Estabelecimento__
#include "ponto.h"
#include "quadra.h"

typedef void* Estabelecimento; 

Estabelecimento createEstabelecimento(Quadra quad, char cnpj[], char cpf[], char nome[], char codt[], char face, int num);

char* getCepEstabelecimento(Estabelecimento estabelecimento);

char* getCpfEstabelecimento(Estabelecimento estabelecimento);

char* getCnpjEstabelecimento(Estabelecimento estabelecimento);

char* getNomeEstabelecimento(Estabelecimento estabelecimento);

char* getCodtEstabelecimento(Estabelecimento estabelecimento);

Ponto getPontoEstabelecimento(Estabelecimento estabelecimento);

char getFaceEstabelecimento(Estabelecimento estabelecimento);

int getNumEstabelecimento(Estabelecimento estabelecimento);

void swapEstabelecimento(Estabelecimento e1, Estabelecimento e2);

void desalocarEstabelecimento(Estabelecimento estabelecimento);

#endif