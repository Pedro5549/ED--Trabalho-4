#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "leitura.h"
#include "retangulo.h"
#include "circulo.h"
#include "texto.h"
#include "quadra.h"
#include "instrumentoUrbano.h"
#include "regiao.h"
#include "lista.h"
#include "sorts.h"
#include "ponto.h"
#include "qry1.h"
#include "qry2.h"
#include "qry3.h"
#include "svg.h"
#include "casos.h"

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
                fscanf(geo,"%lf %lf %lf %lf", &x, &y, &h, &w);
            }
        }
        else if(strcmp(tipo,"c") == 0){
            fscanf(geo,"%s %lf %lf %lf %s %s", cepid, &h, &x, &y, corb, corp);
            listInsert(criarCirculo(cepid,x,y,h,cw,corp,corb), list[4]);
        }
        else if(strcmp(tipo,"r") == 0){
            fscanf(geo,"%s %lf %lf %lf %lf %s %s", cepid, &w, &h, &x, &y, corb, corp);
            listInsert(criarRetangulo(cepid,x,y,w,h,rw,corp,corb), list[5]);
        }
        else if(strcmp(tipo,"t") == 0){
            fscanf(geo,"%s %lf %lf %s %s", cepid, &x, &y, corb, corp);
            fgets(texto, 255, geo);
            listInsert(criarTexto(cepid,x,y,corp,corb,texto), list[6]);
        }
        else if(strcmp(tipo,"q") == 0){
            fscanf(geo,"%s %lf %lf %lf %lf", cepid, &x, &y, &w, &h);
            listInsert(criarQuadra(cepid,x,y,eq,w,h,corQP,corQB), list[0]);
        }
        else if(strcmp(tipo,"h") == 0){
            fscanf(geo,"%s %lf %lf", cepid, &x, &y);
            listInsert(criarInstrumento(cepid,x,y,eh,corHP,corHB), list[1]);
        }
        else if(strcmp(tipo,"s") == 0){
            fscanf(geo,"%s %lf %lf", cepid, &x, &y);
            listInsert(criarInstrumento(cepid,x,y,es,corSP,corSB), list[2]);
        }
        else if(strcmp(tipo,"rb") == 0){
            fscanf(geo,"%s %lf %lf", cepid, &x, &y);
            listInsert(criarInstrumento(cepid,x,y,er,corRP,corRB), list[3]);
        }
        else if(strcmp(tipo,"cq") == 0){
            fscanf(geo,"%s %s %s", eq, corQP, corQB);
        }
        else if(strcmp(tipo,"ch") == 0){
            fscanf(geo,"%s %s %s", eh, corHP, corHB);
        }
        else if(strcmp(tipo,"cs") == 0){
            fscanf(geo,"%s %s %s", es, corSP, corSB);
        }
        else if(strcmp(tipo,"cr") == 0){
            fscanf(geo,"%s %s %s", er, corRP, corRB);
        }
        else if(strcmp(tipo,"sw") == 0){
            fscanf(geo,"%s %s", cw, rw);
        }
        else if(strcmp(tipo, "ps") == 0) {
            fscanf(geo, "%lf %lf", &x, &y);
            listInsert(createPoint(x, y), list[7]);
        }
        else if(strcmp(tipo, "dd") == 0) {
            fscanf(geo, "%lf %lf %lf %lf %lf", &x, &y, &w, &h, &d);
            listInsert(criarRegiao(x, y, w, h, d), list[8]);
        }
    }
    void* (*getPonto[8])(void*) = {getPontoQuad, getPontoIU, getPontoIU, getPontoIU, getPontoCirc, getPontoRet, getPontoTxt, NULL};
    void (*swap[8])(void*, void*) = {swapQuad, swapIU, swapIU, swapIU, swapCirc, swapRet, swapTxt, swapPonto};
    for(i = 0; i < 8; i++){
        balancearQuadTree(quadtrees[i], list[i], getPonto[i], swap[i]);
    }
    for(No node = getFirst(list[8]); node != NULL; node = getNext(node)){
        densidadeQuadras(getInfo(node),quadtrees[0]);
    }
    FILE* svg = iniciarSvg(saida);
    desenharSvg(svg,quadtrees,NULL);
    fecharSvg(svg);
    fclose(geo);
    for(i = 0; i < 8; i++){
        removeList(list[i],NULL);
    }
    removeList(list[8],desalocarRegiao);
}

void qry(QuadTree qt[11], char path[], char nomeSaida[]){
    char* pathTxt = malloc((5 + strlen(nomeSaida))*sizeof(char));
    char* pathSvg = malloc((5 + strlen(nomeSaida))*sizeof(char));
    sprintf(pathTxt,"%s.txt",nomeSaida);
    sprintf(pathSvg,"%s.svg",nomeSaida);
    FILE* consulta = fopen(path,"r");
    FILE* saida = fopen(pathTxt,"w");
    FILE* svg = iniciarSvg(pathSvg);
    if(saida == NULL || consulta == NULL){
        printf("erro ao abrir o arquivo\n");
        exit(1);
    }
    Lista extraFig = createList();
    int j,k,i;
    double x,y,h,w;
    char face, tipo[5], cepid[20], aux[20], corb[22], corp[22];
    while(fscanf(consulta,"%s",tipo) != EOF){
        if(strcmp(tipo,"o?") == 0){
            fscanf(consulta,"%s %s",cepid,aux);
            fprintf(saida,"%s %s %s\n",tipo,cepid,aux);
            intersecao(svg,saida,qt,cepid,aux,extraFig);
        }
        else if(strcmp(tipo,"i?") == 0){
	        fscanf(consulta,"%s %lf %lf",cepid,&x,&y);
	        fprintf(saida,"%s %s %lf %lf\n",tipo,cepid,x,y);
	        pontoInterno(svg,saida,qt,cepid,x,y,extraFig);
        }
        else if(strcmp(tipo,"pnt") == 0){
	        fscanf(consulta,"%s %s %s",cepid,corb,corp);
	        fprintf(saida,"%s %s %s %s\n",tipo,cepid,corb,corp);
	        pnt(saida,qt,cepid,corb,corp);
        }
        else if(strcmp(tipo,"pnt*") == 0){
            fscanf(consulta,"%d %d %s %s",&j,&k,corb,corp);
            fprintf(saida,"%s %d %d %s %s\n",tipo,j,k,corb,corp);
            for(i = j; i <= k; i++){
                sprintf(cepid,"%d",i);
                pnt(saida,qt,cepid,corb,corp);
            }
        }
        else if(strcmp(tipo,"delf") == 0){
            fscanf(consulta,"%s",cepid);
            fprintf(saida,"%s %s\n",tipo,cepid);
            delf(saida,qt,cepid);
        }
        else if(strcmp(tipo,"delf*") == 0){
            fscanf(consulta,"%d %d",&j,&k);
            fprintf(saida,"%s %d %d\n",tipo,j,k);
            for(i = j; i <= k; i++){
                sprintf(cepid,"%d",i);
                delf(saida,qt,cepid);
            }
        }
        else if(strcmp(tipo,"dq") == 0){
            fscanf(consulta,"%s", cepid);
            if(strcmp(cepid,"#") == 0){
                i = 1;
                fscanf(consulta,"%s %lf",cepid,&h);
                fprintf(saida,"%s # %s %lf",tipo,cepid,h);
            }else{
                i = 0;
                fscanf(consulta,"%lf", &h);
                fprintf(saida,"%s %s %lf\n",tipo,cepid,h);
            }
            dq(svg, saida, qt, i, cepid, h);
        }
        else if(strcmp(tipo,"del") == 0){
            fscanf(consulta,"%s", cepid);
            fprintf(saida,"%s %s\n",tipo,cepid);
            del(svg, saida, qt, cepid, extraFig);
        }
        else if(strcmp(tipo,"cbq") == 0){
            fscanf(consulta,"%lf %lf %lf %s", &x, &y ,&h, corb);
            fprintf(saida,"%s %lf %lf %lf %s\n", tipo, x, y ,h, corb);
            cbq(saida, qt, x, y, h, corb);
        }
        else if(strcmp(tipo,"crd?") == 0){
            fscanf(consulta,"%s", cepid);
            fprintf(saida,"%s %s\n",tipo,cepid);
            crd(saida, qt, cepid);
        }
        else if(strcmp(tipo,"car") == 0){
            fscanf(consulta,"%lf %lf %lf %lf", &x, &y ,&w, &h);
            fprintf(saida,"%s %lf %lf %lf %lf\n", tipo, x, y, w, h);
            car(svg, saida, qt, x, y, w, h, extraFig);
        }
        else if(strcmp(tipo,"cv") == 0){
            fscanf(consulta,"%d %s %c %d", &i, cepid ,&face, &j);
            fprintf(saida,"%s %d %s %c %d\n",tipo, i, cepid ,face, j);
            cv(qt,i,cepid,face,j);
        }
        else if(strcmp(tipo,"soc") == 0){
            fscanf(consulta,"%d %s %c %d", &i, cepid ,&face, &j);
            fprintf(saida,"%s %d %s %c %d\n",tipo, i, cepid ,face, j);
            soc(svg,saida,qt,i,cepid,face,j,extraFig);
        }
        else if(strcmp(tipo,"ci") == 0){
            fscanf(consulta,"%lf %lf %lf", &x, &y, &h);
            fprintf(saida,"%s %lf %lf %lf\n",tipo, x, y, h);
            ci(svg,saida,qt,x,y,h,extraFig);
        }
    }
    desenharSvg(svg,qt,extraFig);
    fecharSvg(svg);
    fclose(saida);
    fclose(consulta);
    free(pathSvg);
    free(pathTxt);
    removeList(extraFig,free);
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
    char* (*getId[11])(void*) = {getCEP, getIdIU, getIdIU, getIdIU, getIdCirc, getIdRet, getIdTxt, NULL, getCEPCaso, NULL, NULL};
    QuadTree trees[11];
    for(i = 0; i < 11; i++){
        trees[i] = criaQt(getId[i]);
    }
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
        qry(trees, qryArq, saidaQry);
        free(saidaQry);
        free(qryArq); 
    }
    free(geoArq);
    free(saida);
    free(saidaGeo);
    for(i = 0; i < 11; i++){
        desalocaQt(trees[i]);
    }
}