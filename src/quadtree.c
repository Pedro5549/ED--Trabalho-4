#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "quadtree.h"
#include "lista.h"
#include "ponto.h"
#include "fila.h"
#include "verificacao.h"
#define nw 0
#define ne 1
#define sw 2
#define se 3

typedef struct node{
    struct node *children[4];
    struct node *parent;
    Ponto ponto;
    QtInfo info;
}NodeStruct;

typedef struct quadtree{
    NodeStruct* root;
    char* (*fun)(QtInfo);
}QuadtreeStruct;



void desalocaNos(NodeStruct* node){
    if(node == NULL){
        return;
    }
    for(int i = 0; i < 4; i++){
        desalocaNos(node->children[i]);
    }
    if(node->ponto != node->info){
        free(node->ponto);
    }
    free(node->info);
    free(node);
}

Ponto getPontoQt(QuadTree qt, QtNo pNo){
    NodeStruct* node = (NodeStruct*) pNo;
    qt = qt;
    return node->ponto;
}

QtInfo getInfoById(QuadTree qt, QtNo no, char* chave){
    NodeStruct* node = (NodeStruct*) no;
    QuadtreeStruct* quadtree = (QuadtreeStruct*) qt;
    if(strcmp(quadtree->fun(getInfoQt(qt, node)), chave) == 0){
        return getInfoQt(qt, node);
    }
    QtInfo aux;
    for(int i = 0; i < 4; i++){
        if(node->children[i] != NULL){
            aux = getInfoById(qt,node->children[i],chave);
            if(aux != NULL){
                return aux;
            }
        }
    }
    return NULL;
}


void dentroRetanguloQt(QuadTree qt, NodeStruct* node, Lista l, double x1, double y1, double x2, double y2, void* (*fun)(void*, void*)){
    if(node == NULL){
        return;
    }
    if(pontoInternoRet(getX(node->ponto), getY(node->ponto), x1, y1, x2 - x1, y2 - y1)){
        if(fun != NULL){
            listInsert(fun(qt, node),l);
        }
        else{
            listInsert(node,l);
        }
        
    }
    for(int i = 0; i < 4; i++){
        dentroRetanguloQt(qt, node->children[i], l, x1, y1, x2, y2, fun);
    }
}

void dentroCirculoQt(QuadTree qt, NodeStruct* node, Lista l, double x, double y, double r, void* (*fun)(void*, void*)){
    if(node == NULL){
        return;
    }
    if(pontoInternoCirc(getX(node->ponto), getY(node->ponto), x, y, r)){
        if(fun != NULL){
            listInsert(fun(qt, node),l);
        }
        else{
            listInsert(node,l);
        }
        
    }
    for(int i = 0; i < 4; i++){
        dentroCirculoQt(qt, node->children[i], l, x, y, r, fun);
    }
}

char* getChaveQt(QuadTree qt, QtNo pNo){
    QuadtreeStruct* quadtree = (QuadtreeStruct*) qt;
    NodeStruct* node = (NodeStruct*) pNo;
    return quadtree->fun(getInfoQt(quadtree, node));
}

void percorreProfundidade(QuadTree qt, NodeStruct* node, funcVisita f,ExtraInfo ei){
    if(node == NULL){
        return;
    }
    f(getInfoQt(qt, node),ei);
    for(int i = 0; i < 4; i++){
        percorreProfundidade(qt, node->children[i],f,ei);
    }
}

QuadTree criaQt(funcGetChave f){
    QuadtreeStruct* qt = (QuadtreeStruct*)malloc(sizeof(QuadtreeStruct));
    qt->root = NULL;
    qt->fun = f;
    return qt;
}

Lista chavesDentroRetanguloQt(QuadTree qt, double x1, double y1, double x2, double y2){
    QuadtreeStruct* quadtree = (QuadtreeStruct*) qt;
    NodeStruct* node = quadtree->root;
    Lista l = createList();
    dentroRetanguloQt(qt, node, l, x1, y1, x2, y2, (void*(*)(void*, void*))getChaveQt);
    return l;

}

Lista chavesDentroCirculoQt(QuadTree qt,double x, double y, double r){
    QuadtreeStruct* quadtree = (QuadtreeStruct*) qt;
    NodeStruct* node = quadtree->root;
    Lista l = createList();
    dentroCirculoQt(qt, node, l, x, y, r, (void*(*)(void*, void*))getChaveQt);
    return l;
}

Lista pontosDentroRetanguloQt(QuadTree qt, double x1, double y1, double x2, double y2){
    QuadtreeStruct* quadtree = (QuadtreeStruct*) qt;
    NodeStruct* node = quadtree->root;
    Lista l = createList();
    dentroRetanguloQt(qt, node, l, x1, y1, x2, y2, getPontoQt);
    return l;

}

Lista pontosDentroCirculoQt(QuadTree qt,double x, double y, double r){
    QuadtreeStruct* quadtree = (QuadtreeStruct*) qt;
    NodeStruct* node = quadtree->root;
    Lista l = createList();
    dentroCirculoQt(qt, node, l, x, y, r, getPontoQt);
    return l;
}

Lista nosDentroCirculoQt(QuadTree qt,double x, double y, double r){
    QuadtreeStruct* quadtree = (QuadtreeStruct*) qt;
    NodeStruct* node = quadtree->root;
    Lista l = createList();
    dentroCirculoQt(qt, node, l, x, y, r, NULL);
    return l;
}

Lista nosDentroRetanguloQt(QuadTree qt, double x1, double y1, double x2, double y2){
    QuadtreeStruct* quadtree = (QuadtreeStruct*) qt;
    NodeStruct* node = quadtree->root;
    Lista l = createList();
    dentroRetanguloQt(qt, node, l, x1, y1, x2, y2, NULL);
    return l;

}

void percorreProfundidadeQt(QuadTree qt,funcVisita f,ExtraInfo ei){
    QuadtreeStruct* quadtree = (QuadtreeStruct*) qt;
    NodeStruct* node = quadtree->root;
    percorreProfundidade(qt, node, f, ei);
}

void percorreLarguraQt(QuadTree qt,funcVisita f,ExtraInfo ei){
    QuadtreeStruct* quadtree = (QuadtreeStruct*) qt;
    NodeStruct* aux;
    if(quadtree->root == NULL){
        return;
    }
    Fila fila = createQueue();
    enqueue(quadtree->root, fila);
    do{
        aux = dequeue(fila);
        for(int i = 0; i < 4; i++){
            if(aux->children[i] != NULL){
                enqueue(aux->children[i], fila);
            }
        }
        f(getInfoQt(quadtree, aux),ei);
    }while(!queueIsEmpty(fila));
    free(fila);
}

QtNo insereQt(QuadTree qt,Ponto p, QtInfo pInfo){
    QuadtreeStruct* quadtree = (QuadtreeStruct*) qt;
    NodeStruct* node = (NodeStruct*)malloc(sizeof(NodeStruct));
    NodeStruct* aux = quadtree->root; 
    node->ponto = p;
    node->info = pInfo;
    node->parent = NULL;
    for(int i = 0; i < 4; i++){
        node->children[i] = NULL;
    }
    if(aux == NULL){
        quadtree->root = node;
        return node;
    }
    Ponto pAux;
    do{
        pAux = aux->ponto;
        if(getX(p) >= getX(pAux)){
            if(getY(p) >= getY(pAux)){
                if(aux->children[ne] == NULL){
                    aux->children[ne] = node;
                    node->parent = aux;
                }
                else{
                    aux = aux->children[ne];
                }
            }
            else{
                if(aux->children[nw] == NULL){
                    aux->children[nw] = node;
                    node->parent = aux;
                }
                else{
                    aux = aux->children[nw];
                }
            }
        }
        else{
            if(getY(p) >= getY(pAux)){
                if(aux->children[se] == NULL){
                    aux->children[se] = node;
                    node->parent = aux;
                }
                else{
                    aux = aux->children[se];
                }
            }
            else{
                if(aux->children[sw] == NULL){
                    aux->children[sw] = node;
                    node->parent = aux;
                }
                else{
                    aux = aux->children[sw];
                }
            }
        }
    }while(node->parent == NULL);
    return node;
}

QtInfo removeNoQt(QuadTree qt,QtNo pNo){
    QuadtreeStruct* quadtree = (QuadtreeStruct*) qt;
    NodeStruct* node = (NodeStruct*) pNo;
    NodeStruct* aux;
    int i;
    QtInfo info;
    Fila fila = createQueue();
    if(node->parent == NULL){
        for(i = 0; i < 4; i++){
            if(node->children[i] != NULL){
                enqueue(node->children[i], fila);
            }
        }
        quadtree->root = NULL;
    }
    else{
        for(i = 0; i < 4; i++){
            if(node->children[i] != NULL){
                if(node->parent->children[i] == NULL){
                    node->parent->children[i] = node->children[i];
                }
                else{
                    enqueue(node->children[i], fila);
                }
            }
        }
        for(i = 0; i < 4; i++){
            if(node->parent->children[i] == node){
                node->parent->children[i] = NULL;
                break;
            }
        }
    }
    while(!queueIsEmpty(fila)){
        aux = dequeue(fila);
        for(i = 0; i < 4; i++){
            if(aux->children[i] != NULL){
                enqueue(aux->children[i], fila);
            }
        }
        insereQt(quadtree, aux->ponto, aux->info);
        free(aux);
    }
    info = getInfoQt(quadtree, node);
    free(node);
    free(fila);
    return info;
}

QtNo getNoQt(QuadTree qt, double x, double y){
    QuadtreeStruct* quadtree = (QuadtreeStruct*) qt;
    NodeStruct* aux = quadtree->root;
    Ponto p;
    while (aux != NULL){
        p = aux->ponto;
        if(x == getX(p) && y == getY(p)){
            return aux;
        }
        if(x > getX(p)){
            if(y > getY(p)){
                aux = aux->children[ne];
            }
            else{
                aux = aux->children[nw];
            }
        }
        else{
            if(y > getY(p)){
                aux = aux->children[se];
            }
            else{
                aux = aux->children[sw];
            }
        }
    }
    return NULL;
}

QtInfo getInfoQt(QuadTree qt, QtNo pNo){
    NodeStruct* node = (NodeStruct*) pNo;
    qt = qt;
    return node->info;
}

QtInfo getInfoByIdQt(QuadTree qt, char* chave){
    QuadtreeStruct* quadtree = (QuadtreeStruct*) qt;
    return getInfoById(qt, quadtree->root, chave);
}

void desalocaQt(QuadTree qt){
    QuadtreeStruct* quadtree = (QuadtreeStruct*) qt;
    desalocaNos(quadtree->root);
    free(qt);
}
