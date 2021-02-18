#ifndef __endereco__
#define __endereco__
#include "ponto.h"
#include "quadra.h"

typedef void* Endereco; 

Endereco createEndereco(Quadra quad, char cpf[], char face, int num, char compl[]);

void setCepEndereco(Endereco endereco, char cep[]);

char* getCepEndereco(Endereco endereco);

void setCpfEndereco(Endereco endereco, char cpf[]);

char* getCpfEndereco(Endereco endereco);

void setPontoEndereco(Endereco endereco, Ponto p);

Ponto getPontoEndereco(Endereco endereco);

void setFaceEndereco(Endereco endereco, char face);

char getFaceEndereco(Endereco endereco);

void setNumEndereco(Endereco endereco, int num);

int getNumEndereco(Endereco endereco);

void setComplementoEndereco(Endereco endereco, char compl[]);

char* getComplementoEndereco(Endereco endereco);

void swapEndereco(Endereco e1, Endereco e2);

void desalocarEndereco(Endereco endereco);

#endif