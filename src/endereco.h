#ifndef __endereco__
#define __endereco__
#include "ponto.h"
#include "quadra.h"

typedef void* Endereco; 

Endereco createEndereco(Quadra quad, char cpf[], char face, int num);

char* getCepEndereco(Endereco endereco);

char* getCpfEndereco(Endereco endereco);

Ponto getPontoEndereco(Endereco endereco);

char getFaceEndereco(Endereco endereco);

int getNumEndereco(Endereco endereco);

void swapEndereco(Endereco e1, Endereco e2);

void desalocarEndereco(Endereco endereco);

#endif