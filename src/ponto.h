#ifndef __ponto__
#define __ponto__

typedef void* Ponto;
typedef void* PontoInfo;

Ponto createPoint(double x, double y);

void setX(Ponto ponto, double x);

double getX(Ponto ponto);

void setY(Ponto ponto, double y);

double getY(Ponto ponto);

void desenharPonto(Ponto pont, FILE* svg);

void swapPonto(Ponto p1, Ponto p2);

void atribuirPonto(Ponto *p1, Ponto *p2);

double getPontoDist(Ponto pont, double x, double y);

#endif