#ifndef __pilha__
#define __pilha__

typedef void* Pilha;
typedef void* PilhaInfo;

Pilha createStack();

int stackIsEmpty(Pilha pilha);

void pushStack(Pilha pilha, PilhaInfo info);

PilhaInfo popStack(Pilha pilha);

#endif