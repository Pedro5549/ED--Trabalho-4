#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "retangulo.h"
#include "circulo.h"
#include "texto.h"
#include "quadra.h"
#include "instrumentoUrbano.h"
#include "regiao.h"
#include "lista.h"
#include "sorts.h"
#include "ponto.h"
//#include "qry.h"
#include "svg.h"


char *obterNomeArquivo(char path[]){
    char *aux = strrchr(path,'/');
    if(aux == NULL){
        return strtok(path,".");
    }
	return strtok(&aux[1],".");
}

void geo(QuadTree quadtrees[11], char geoArq[], char saida[]){
    char tipo[4] ,corb[22] ,corp[22], cepid[20], texto[255];
    int i;
    double x, y, w, h, d;
    char cw[10] = "1";
    char rw[10] = "1";
    char eq[10] = "1";
    char eh[10] = "1";
    char es[10] = "1";
    char er[10] = "1";
	char corQP[22] = "blue";
	char corQB[22] = "blue";
	char corHP[22] = "blue";
	char corHB[22] = "blue";
	char corSP[22] = "blue";
	char corSB[22] = "blue";
	char corRP[22] = "blue";
	char corRB[22] = "blue";
    Lista list[9];
    for(i = 0; i < 9; i++){
        list[i] = createList();
    }
    FILE* geo = fopen(geoArq,"r");
    if(geo == NULL){
        printf("erro ao abrir o arquivo\n");
        exit(1);
    }
    while(fscanf(geo,"%s", tipo) != EOF){
        if (strcmp(tipo,"nx") == 0){
            fscanf(geo,"%lf", &x);
            if(fgetc(geo) != '\n'){
                fscanf(geo,"%lf %lf %lf %lf\n", &x, &y, &h, &w);
            }
        }
        else if(strcmp(tipo,"c") == 0){
            fscanf(geo,"%s %lf %lf %lf %s %s\n", cepid, &h, &x, &y, corb, corp);
            listInsert(list[4], criarCirculo(cepid,x,y,h,cw,corp,corb));
        }
        else if(strcmp(tipo,"r") == 0){
            fscanf(geo,"%s %lf %lf %lf %lf %s %s\n", cepid, &w, &h, &x, &y, corb, corp);
            listInsert(list[5], criarRetangulo(cepid,x,y,w,h,rw,corp,corb));
        }
        else if(strcmp(tipo,"t") == 0){
            fscanf(geo,"%s %lf %lf %s %s\n", cepid, &x, &y, corb, corp);
            fgets(texto, 255, geo);
            listInsert(list[6], criarTexto(cepid,x,y,corp,corb,texto));
        }
        else if(strcmp(tipo,"q") == 0){
            fscanf(geo,"%s %lf %lf %lf %lf\n", cepid, &x, &y, &w, &h);
            listInsert(list[0], criarQuadra(cepid,x,y,eq,w,h,corQP,corQB));
        }
        else if(strcmp(tipo,"h") == 0){
            fscanf(geo,"%s %lf %lf\n", cepid, &x, &y);
            listInsert(list[1], criarInstrumento(cepid,x,y,eh,corHP,corHB));
        }
        else if(strcmp(tipo,"s") == 0){
            fscanf(geo,"%s %lf %lf\n", cepid, &x, &y);
            listInsert(list[2], criarInstrumento(cepid,x,y,es,corSP,corSB));
        }
        else if(strcmp(tipo,"rb") == 0){
            fscanf(geo,"%s %lf %lf\n", cepid, &x, &y);
            listInsert(list[3], criarInstrumento(cepid,x,y,er,corRP,corRB));
        }
        else if(strcmp(tipo,"cq") == 0){
            fscanf(geo,"%s %s %s\n", eq, corQP, corQB);
        }
        else if(strcmp(tipo,"ch") == 0){
            fscanf(geo,"%s %s %s\n", eh, corHP, corHB);
        }
        else if(strcmp(tipo,"cs") == 0){
            fscanf(geo,"%s %s %s\n", es, corSP, corSB);
        }
        else if(strcmp(tipo,"cr") == 0){
            fscanf(geo,"%s %s %s\n", er, corRP, corRB);
        }
        else if(strcmp(tipo,"sw") == 0){
            fscanf(geo,"%s %s\n", cw, rw);
        }
        else if(strcmp(tipo, "ps") == 0) {
            fscanf(geo, "%lf %lf\n", &x, &y);
            listInsert(list[7], createPoint(x, y));
        }
        else if(strcmp(tipo, "dd") == 0) {
            fscanf(geo, "%lf %lf %lf %lf %lf\n", &x, &y, &w, &h, &d);
            listInsert(list[8], criarRegiao(x, y, w, h, d));
        }
    }
    void* (*getPonto[8])(void*) = {getPontoQuad, getPontoIU, getPontoIU, getPontoIU, getPontoCirc, getPontoRet, getPontoTxt, NULL};
    void (*swap[8])(void**, void**) = {swapQuad, swapIU, swapIU, swapIU, swapCirc, swapRet, swapTxt, swapPonto};
    for(i = 0; i < 8; i++){
        balancearQuadTree(quadtrees[i], list[i], getPonto[i], swap[i]);
    }
    for(No node = getFirst(list[8]); node != NULL; node = getNext(node)){
        desidadeQuadras(getInfo(node),quadtrees[0]);
    }
    FILE* svg = iniciarSvg(saida);
    desenharSvg(svg,quadtrees,NULL);
    fecharSvg(svg);
    fclose(geo);
    for(i = 0; i < 8; i++){
        removeList(list[i],0);
    }
    removeList(list[8],1);
}


void tratamento(char path[], char outPath[], char paramGeo[], char paramQry[], char paramEc[], char paramPm[]){
    char *geoArq = NULL;
    char *qryArq = NULL;
    char *ecArq = NULL;
    char* pmArq = NULL;
    char *nomeGeo = NULL;
    char *nomeQry = NULL;

    char *saida = NULL;
    char *saidaGeo = NULL;
    char *saidaQry = NULL;
    int i;
    if (path != NULL) {
        if(path[strlen(path) - 1] != '/'){
            geoArq = (char *)malloc((strlen(paramGeo)+strlen(path)+2)*sizeof(char));
    	    sprintf(geoArq,"%s/%s",path,paramGeo);
        }
		else{
            geoArq = (char *)malloc((strlen(paramGeo)+strlen(path)+1)*sizeof(char));
    	    sprintf(geoArq,"%s%s",path,paramGeo);
        }
        if (paramQry != NULL){
            qryArq = (char *)malloc((strlen(paramQry)+strlen(path)+2)*sizeof(char));
            sprintf(qryArq,"%s/%s",path,paramQry);
        }
        if (paramEc != NULL){
            ecArq = (char *)malloc((strlen(paramEc)+strlen(path)+2)*sizeof(char));
            sprintf(ecArq,"%s/%s",path,paramEc);
        }
        if (paramPm != NULL){
            pmArq = (char *)malloc((strlen(paramPm)+strlen(path)+2)*sizeof(char));
            sprintf(pmArq,"%s/%s",path,paramPm);
        }
	} else {
		geoArq = (char *)malloc((strlen(paramGeo)+1)*sizeof(char));
    	strcpy(geoArq, paramGeo);
        if(paramQry != NULL){
            qryArq = (char *)malloc((strlen(paramQry)+1)*sizeof(char));
            strcpy(qryArq, paramQry);
        }
        if(paramEc != NULL){
            qryArq = (char *)malloc((strlen(paramEc)+1)*sizeof(char));
            strcpy(ecArq, paramEc);
        }
        if(paramPm != NULL){
            pmArq = (char *)malloc((strlen(paramPm)+1)*sizeof(char));
            strcpy(pmArq, paramPm);
        }
	}
    nomeGeo = obterNomeArquivo(paramGeo);
    if (outPath[strlen(outPath) - 1] == '/'){
        saida = (char*)malloc((strlen(nomeGeo) + strlen(outPath) + 1)*sizeof(char));
        sprintf(saida,"%s%s",outPath,nomeGeo);
    }
    else{
        saida = (char*)malloc((strlen(nomeGeo) + strlen(outPath) + 2)*sizeof(char));
        sprintf(saida,"%s/%s",outPath,nomeGeo);void DesidadeQuadras(Regiao reg, QuadTree quadras);
    }
    saidaGeo = (char*)malloc((strlen(saida) + 5)*sizeof(char));
    sprintf(saidaGeo,"%s.svg",saida);
    char* (*getId[11])(void*) = {getCEP, getIdIU, getIdIU, getIdIU, getIdCirc, getIdRet, getIdTxt, NULL, NULL, NULL, NULL};
    QuadTree trees[11];
    for(i = 0; i < 11; i++){
        trees[i] = criaQt(getId[i]);
    }
    Lista figExtras = createList();
    geo(trees, geoArq,saidaGeo);
    if(paramEc != NULL){
        //chamar função para ec
        free(ecArq);
    }
    if(paramPm != NULL){
        //chamar função para ec
        free(pmArq);
    }
    if (paramQry != NULL){
        nomeQry = obterNomeArquivo(paramQry);
        saidaQry = (char*)malloc((strlen(outPath) + strlen(saida) + 2)*sizeof(char));
        sprintf(saidaQry,"%s-%s",saida,nomeQry);
        //qry
        free(saidaQry);
        free(qryArq); 
    }
    free(geoArq);
    free(saida);
    free(saidaGeo);
    for(i = 0; i < 11; i++){
        desalocaQt(trees[i]);
    }
    removeList(figExtras,1);
}