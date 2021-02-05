#ifndef __fila__
#define __fila__

typedef void* Fila;
typedef void* FilaInfo;

Fila createQueue();

int queueIsEmpty(Fila fila);

void enqueue(FilaInfo info, Fila fila);

FilaInfo dequeue(Fila fila);

#endif