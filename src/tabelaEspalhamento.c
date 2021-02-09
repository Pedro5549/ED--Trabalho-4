#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tabelaEspalhamento.h"
#include "lista.h"

typedef struct {
    Lista *tabela;
    int tamanho;
} HashTableStruct;

typedef struct {
    char *chave;
    Item valor;
} ItemStruct;

HashTable iniciaTabela(int tamanho) {
    HashTableStruct *h = (HashTableStruct*) malloc(sizeof(HashTableStruct));

    h->tamanho = tamanho;
    h->tabela = (Lista*) malloc(sizeof(Lista) * tamanho);

    for (int i = 0; i < tamanho; i++) {
        h->tabela[i] = createList();
        printf("Lista hash %d criada\n", i + 1);
    }

    return h;
}

int getChave(char *chave, int tamanhoHash) {
    int aux = 0, res;
    int tamanho = strlen(chave);

    for (int i = 0; i < tamanho; i++) {
        aux += chave[i];
    }

    res = aux % tamanhoHash;
    printf("Chave gerada: %d\n", res);
    return res;
}

void adicionaItem(HashTable t, char *chave, Item valor) {
    HashTableStruct *h = (HashTableStruct*) t;
    int hashKey = getChave(chave, h->tamanho);

    ItemStruct *item = (ItemStruct*) malloc(sizeof(ItemStruct));
    item->chave = chave;
    item->valor = valor;

    listInsert(h->tabela[hashKey], item);
}

void deletaItem(HashTable t, char *chave) {
    HashTableStruct *h = (HashTableStruct*) t;
    int hashKey = getChave(chave, h->tamanho);

    for (No aux = getFirst(h->tabela[hashKey]); aux != NULL; aux = getNext(aux)) {
        ItemStruct *i = (ItemStruct*) getInfo(aux);
        if (strcmp(i->chave, chave) == 0) {
            removeNode(h->tabela[hashKey], aux, 1);
            return;
        }
    }
}

Item getValor(HashTable t, char *chave) {
    HashTableStruct *h = (HashTableStruct*) t;
    int hashKey = getChave(chave, h->tamanho);

    for (No aux = getFirst(h->tabela[hashKey]); aux != NULL; aux = getNext(h->tabela[hashKey])) {
        ItemStruct *i = (ItemStruct*) getInfo(aux);
        if (strcmp(i->chave, chave) == 0) {
            return i->valor;
        }
    }

    return NULL;
}

void deletaTabela(HashTable t) {
    HashTableStruct *h = (HashTableStruct*) t;
    
    for (int i = 0; i < h->tamanho; i++) {
        removeList(h->tabela[i], 1);
    }

    if (h->tabela != NULL) {
        free(h->tabela);
    }

    free(h);

    printf("HashTable deletada.\n");
}

void imprimeTabela(HashTable t) {
    HashTableStruct *h = (HashTableStruct*) t;

    for (int i = 0; i < h->tamanho; i++) {
        No aux = getFirst(h->tabela[i]);
        while (aux != NULL) {
            ItemStruct *i = (ItemStruct*) getInfo(aux);
            printf("%s: %s\n", i->chave, i->valor);
            aux = getNext(aux);
        }
    }
}