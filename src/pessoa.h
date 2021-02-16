#ifndef __pessoa__
#define __pessoa__

typedef void* Pessoa; 

Pessoa criarPessoa(char cpf[], char nome[], char sobrenome[], char sexo, char dataNascimento[]);

char* getCpf(Pessoa pessoa);

char* getNome(Pessoa pessoa);

char* getSobrenome(Pessoa pessoa);

char getSexo(Pessoa pessoa);

char* getDataNascimento(Pessoa pessoa);

#endif