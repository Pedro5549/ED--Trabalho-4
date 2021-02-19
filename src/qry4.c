#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "qry4.h"
#include "tabelaEspalhamento.h"
#include "ponto.h"
#include "lista.h"
#include "quadtree.h"
#include "endereco.h"
#include "pessoa.h"
#include "Estabelecimento.h"
#include "quadra.h"
#include "svg.h"

void moradores(FILE *txt, QuadTree qt[11], HashTable ht[4], char cep[]){
    Quadra q = getValor(ht[3], cep);
    if(q == NULL){
        fprintf(txt,"Quadra não existe\n");
        return;
    }
    double x = getX(getPontoQuad(q));
    double y = getY(getPontoQuad(q));
    Lista l = nosDentroRetanguloQt(qt[10], x, y, x + getWQuad(q), y + getHQuad(q));
    for(No node = getFirst(l); node != NULL; node = getNext(node)){
        Endereco e = getInfoQt(qt[10], getInfo(node));
        Pessoa p = getValor(ht[2], getCpfEndereco(e));
        fprintf(txt,"Informações pessoais:\nCpf : %s, Nome : %s, Sobrenome : %s, sexo : %c, data de nascimento : %s\n", getCpf(p), getNome(p), getSobrenome(p), getSexo(p), getDataNascimento(p));
        fprintf(txt, "Endereço:\nCEP : %s, face : %c, número: %d, complemento: %s\n\n", cep, getFaceEndereco(e), getNumEndereco(e), getComplementoEndereco(e));
    }
    removeList(l, NULL);
}

void dm(FILE *svg, FILE *txt, HashTable ht[4], char pcpf[], Lista extraFig) {
    char cpf[15], nome[30], sobrenome[30], sexo, nasc[11];
    int *tamanho;
    Info info = getValor(ht[2], pcpf), end;
    Ponto p;

    if (info == NULL) {
        fprintf(txt, "Nenhum morador com o cpf %s\n", pcpf);
        printf("Nenhum morador com esse cpf.\n");
        return;
    }

    end = getValor(ht[0], pcpf);
    p = getPontoEndereco(end);

    strcpy(cpf, getCpf(info));
    strcpy(nome, getNome(info));
    strcpy(sobrenome, getSobrenome(info));
    strcpy(nasc, getDataNascimento(info));
    sexo = getSexo(info);
    fprintf(txt, "Nome completo: %s %s; Cpf: %s; Sexo: %c; Data de nascimento: %s\n", nome, sobrenome, cpf, sexo, nasc);

    tamanho = (int*) malloc(sizeof(int));
    *tamanho = getTamanho(extraFig);
    listInsert(tamanho, extraFig);
    fprintf(svg, "\t<line id=\"%d\" x1=\"%lf\" y1=\"%lf\" x2=\"%lf\" y2=\"0\" style=\"stroke: black; stroke-width: 1\" />\n", *tamanho, getX(p), getY(p), getX(p));
    
    tamanho = (int*) malloc(sizeof(int));
    *tamanho = getTamanho(extraFig);
    listInsert(tamanho, extraFig);
    fprintf(svg, "\t<text id=\"%d\" x=\"%lf\" y=\"0\">Nome completo: %s %s; Cpf: %s; Sexo: %c; Data de nascimento: %s</text>\n", *tamanho, getX(p), nome, sobrenome, cpf, sexo, nasc);
}

void de(FILE *txt, QuadTree qt[11], char pcnpj[]) {
    char cnpj[20], nome[20], cpf[15], cep[20], codt[10], face;
    int num;
    Info info;
    QtNo aux;

    aux = getNodeByIdQt(qt[9], pcnpj);
    if (aux == NULL) {
        fprintf(txt, "Nenhum estabelecimento com o cnpj %s\n", pcnpj);
        printf("Estabelecimento nao encontrado.\n");
        return;
    }

    info = getInfoQt(qt[9], aux);
    strcpy(cnpj, pcnpj);
    strcpy(nome, getNomeEstabelecimento(info));
    strcpy(cpf, getCpfEstabelecimento(info));
    strcpy(cep, getCepEstabelecimento(info));
    strcpy(codt, getCodtEstabelecimento(info));
    face = getFaceEstabelecimento(info);
    num = getNumEstabelecimento(info);

    fprintf(txt, "Nome: %s; cpf: %s; cnpj: %s; cep: %s; face: %c; num: %d; codt: %s\n", nome, cpf, cnpj, cep, face, num, codt);
}

void mud(FILE *svg, FILE *txt, QuadTree qt[11], HashTable ht[4], char cpf[], char cep[], char face, int num, char compl[], Lista extraFig) {
    double x, y;
    int *tamanho;
    Info pes, end;
    Ponto p;
    QtNo aux;
    end = getValor(ht[0], cpf);
    pes = getValor(ht[2], cpf);
    p = getPontoEndereco(end);
    x = getX(p);
    y = getY(p);
    fprintf(txt, "Dados da pessoa:\nNome completo: %s %s; Cpf: %s; Sexo: %c; Data de nascimento: %s\n", getNome(pes), getSobrenome(pes), getCpf(pes), getSexo(pes), getDataNascimento(pes));
    fprintf(txt, "Endereço antigo:\nCep: %s; Face: %c; Num: %d; Complemento: %s\n", getCepEndereco(end), getFaceEndereco(end), getNumEndereco(end), getComplementoEndereco(end));
    fprintf(txt, "Endereço novo:\nCep: %s; Face: %c; Num: %d; Complemento: %s\n", cep, face, num, compl);

    aux = getNodeByIdQt(qt[10], cep);
    deletaItem(ht[0], cpf, 0);
    end = removeNoQt(qt[10], aux);
    desalocarEndereco(end);
    end = createEndereco(getValor(ht[3], cep), cpf, face, num, compl);
    adicionaItem(ht[0], cpf, end);
    p = getPontoEndereco(end);
    aux = insereQt(qt[10], p, end);

    tamanho = (int*) malloc(sizeof(int));
    *tamanho = getTamanho(extraFig);
    listInsert(tamanho, extraFig);
    fprintf(svg, "\t<line id=\"%d\" x1=\"%lf\" y1=\"%lf\" x2=\"%lf\" y2=\"%lf\" style=\"stroke:red; stroke-width: 3\" />\n", *tamanho, x, y, getX(p), getY(p));

    tamanho = (int*) malloc(sizeof(int));
    *tamanho = getTamanho(extraFig);
    listInsert(tamanho, extraFig);
    fprintf(svg, "\t<circle id=\"%d\" cx=\"%lf\" cy=\"%lf\" r=\"5\" fill=\"red\" stroke=\"white\" stroke-width=\"3\" />\n", *tamanho, x, y);

    tamanho = (int*) malloc(sizeof(int));
    *tamanho = getTamanho(extraFig);
    listInsert(tamanho, extraFig);
    fprintf(svg, "\t<circle id=\"%d\" cx=\"%lf\" cy=\"%lf\" r=\"5\" fill=\"blue\" stroke=\"white\" stroke-width=\"3\" />\n", *tamanho, getX(p), getY(p));
}

void dmprbt(QuadTree qt[11], char t, char saida[], char sfx[]){
    int i;
    switch(t) {
    case 'q':
        i = 0;
        break;
    case 'h':
        i = 1;
        break;
    case 's':
        i = 2;
        break;
    case 't':
        i = 3;
        break;
    default:
        printf("Valor inválido (%c)\n", t);
        return;
    }
    char* pathSvg = malloc((6 + strlen(sfx) + strlen(saida))*sizeof(char));
    sprintf(pathSvg,"%s-%s.svg",saida,sfx);
    FILE* svg = iniciarSvg(pathSvg);
    desenharQt(qt[i], svg);
    fecharSvg(svg);
    free(pathSvg);
}