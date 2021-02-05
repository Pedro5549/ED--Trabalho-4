#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "retangulo.h"
#include "circulo.h"
#include "quadra.h"
#include "lista.h"
#include "regiao.h"
#include "casos.h"
#include "ponto.h"

double maior(double n1, double n2){
    if(n1 > n2){
        return n1;
    }
    return n2;
}

double menor(double n1, double n2){
    if(n1 > n2){
        return n2;
    }
    return n1;
}

double distancia(double x1, double y1, double x2, double y2){
    return sqrt(pow(x1 - x2,2) + pow(y1 - y2,2));
}

// void retanguloIntCirculo(Circulo c, Retangulo r, int idc, int idr, FILE* txt, FILE* svg){
//     double deltaX, deltaY, x, y, w, h;
//     if(getXCirc(c) > getXRet(r)){
//         deltaX = pow(getXRet(r) + getWRet(r) - getXCirc(c),2);
//     }
//     else{
//         deltaX = pow(getXRet(r) - getXCirc(c),2);
//     }
//     if(getYCirc(c) > getYRet(r)){
//         deltaY = pow(getYRet(r) + getHRet(r) - getYCirc(c),2);
//     }
//     else{
//         deltaY = pow(getYRet(r) - getYCirc(c),2);
//     }
//     x = menor(getXRet(r),getXCirc(c) - getRCirc(c));
//     w = maior(getXRet(r) + getWRet(r), getXCirc(c) + getRCirc(c)) - x;
//     y = menor(getYRet(r),getYCirc(c) - getRCirc(c));
//     h = maior(getYRet(r) + getHRet(r), getYCirc(c) + getRCirc(c)) - y;
//     if(sqrt(deltaX + deltaY) <=  getRCirc(c)){
//         fprintf(txt,"%d: circulo %d: retangulo SIM",idc,idr);
//         fprintf(svg,"<rect x='%lf' y='%lf' width='%lf' height='%lf' fill='none' stroke='black'/>\n",x,y,w,h);
//     }else{
//         fprintf(txt,"%d: circulo %d: retangulo NAO",idc,idr);
//         fprintf(svg,"<rect x='%lf' y='%lf' width='%lf' height='%lf' fill='none' stroke='black' stroke-dasharray='4'/>\n",x,y,w,h);
//     }
// }

// void circuloInt(Circulo c1, int idc1, Circulo c2, int idc2, FILE* txt, FILE* svg){
//     double x,y,w,h;
//     x = menor(getXCirc(c1) - getRCirc(c1), getXCirc(c2) - getRCirc(c2));
//     w = maior(getXCirc(c1) + getRCirc(c1), getXCirc(c2) + getRCirc(c2)) - x;
//     y = menor(getYCirc(c1) - getRCirc(c1), getYCirc(c2) - getRCirc(c2));
//     h = maior(getYCirc(c1) + getRCirc(c1), getYCirc(c2) + getRCirc(c2)) - y;
//     if(distancia(getXCirc(c1), getXCirc(c1), getXCirc(c2), getYCirc(c2)) <= getRCirc(c2) + getRCirc(c1)){
//         fprintf(txt,"%d: circulo %d: circulo SIM",idc1,idc2);
//         fprintf(svg,"<rect x='%lf' y='%lf' width='%lf' height='%lf' fill='none' stroke='black'/>\n",x,y,w,h);
//     }
//     else{
//         fprintf(txt,"%d: circulo %d: circulo NAO",idc1,idc2);
//         fprintf(svg,"<rect x='%lf' y='%lf' width='%lf' height='%lf' fill='none' stroke='black' stroke-dasharray='4'/>\n",x,y,w,h);
//     }
// }

// void retanguloInt(Retangulo r1, int idr1, Retangulo r2, int idr2, FILE* txt, FILE* svg){
//     double x,w,y,h;
//     x = menor(getXRet(r1),getXRet(r2));
//     w = maior(getXRet(r1) + getWRet(r1),getXRet(r2) + getWRet(r2)) - x;
//     y = menor(getYRet(r1),getYRet(r2));
//     h = maior(getYRet(r1) + getHRet(r1),getYRet(r2) + getHRet(r2)) - y;
//     if (w <= getWRet(r1) + getWRet(r2) && h <= getHRet(r1) + getHRet(r2)){
//         fprintf(txt,"%d: retangulo %d: retangulo SIM",idr1,idr2);
//         fprintf(svg,"<rect x='%lf' y='%lf' width='%lf' height='%lf' fill='none' stroke='black'/>\n",x,y,w,h);
//     }
//     else{
//         fprintf(txt,"%d: retangulo %d: retangulo NAO",idr1,idr2);
//         fprintf(svg,"<rect x='%lf' y='%lf' width='%lf' height='%lf' fill='none' stroke='black' stroke-dasharray='4'/>\n",x,y,w,h);
//     }
// }

// int quadraInternoCirc(Quadra q, double x, double y, double r){
//     if(distancia(getXQuad(q),getYQuad(q),x,y) <= r && distancia(getXQuad(q) + getWQuad(q) ,getYQuad(q) + getHQuad(q),x,y) <= r){
//         if(distancia(getXQuad(q) + getWQuad(q), getYQuad(q),x,y) <= r && distancia(getXQuad(q), getYQuad(q) + getHQuad(q),x,y) <= r){
//             return 1;
//         }
//     }
//     return 0;
// }

// int quadraInternoRet(Quadra q, double x, double y, double w, double h){
//     return getXQuad(q) >= x && getYQuad(q) >= y && getXQuad(q) + getWQuad(q) <= x + w && getYQuad(q) + getHQuad(q) <= y + h;
// }

int pontoInternoCirc(Ponto ponto, double xCirc, double yCirc, double r){
    return distancia(getX(ponto), getY(ponto), xCirc, yCirc) <= r;
}

int pontoInternoRet(Ponto ponto, double xRet, double yRet, double w, double h){
    double dx = getX(ponto) - xRet;
    double dy = getY(ponto) - yRet;
    return dx >= 0 && dx <= w && dy >= 0 && dy <= h;
}

double obterArea(Lista l){
    double a = 0;
    Info i, j;
    No node;
    for(node = getFirst(l); getNext(node) != NULL; node = getNext(node)){
        i = getPontoCaso(getInfo(node));
        j = getPontoCaso(getInfo(getNext(node)));
        a += getX(i) * getY(j) - getY(i) * getX(j);
    }
    i = getPontoCaso(getInfo(node));
    j = getPontoCaso(getInfo(getFirst(l)));
    a += getX(i) * getY(j) - getY(i) * getX(j);
    return a/2;
}

Ponto centroide(Lista l, double area){
    double x = 0, y = 0;
    Info i, j;
    No node;
    for(node = getFirst(l); getNext(node) != NULL; node = getNext(node)){
        i = getPontoCaso(getInfo(node));
        j = getPontoCaso(getInfo(getNext(node)));
        x += (getX(i) + getX(j)) * getX(i) * getY(j) - getY(i) * getX(j);
        y += (getY(i) + getY(j)) * getX(i) * getY(j) - getY(i) * getX(j);
    }
    i = getPontoCaso(getInfo(node));
    j = getPontoCaso(getInfo(getFirst(l)));
    x += (getX(i) + getX(j)) * getX(i) * getY(j) - getY(i) * getX(j);
    y += (getY(i) + getY(j)) * getX(i) * getY(j) - getY(i) * getX(j);
    return createPoint(x/(6 * area), y/(6 * area));
}