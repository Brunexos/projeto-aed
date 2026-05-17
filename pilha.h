#ifndef PILHA_H 
#define PILHA_H 

#include <stdio.h>

#define MAX_PILHA 100

typedef int tp_itemp;

typedef struct {
    int topo;
    tp_itemp item[MAX_PILHA];
} tp_pilha;

void inicializa_pilha(tp_pilha *p);
int pilha_vazia(tp_pilha *p);
int pilha_cheia(tp_pilha *p);
int push(tp_pilha *p, tp_itemp e);
int pop(tp_pilha *p, tp_itemp *e);
int top(tp_pilha *p, tp_itemp *e);
void imprime_pilha(tp_pilha p);
int altura_pilha(tp_pilha *p);

#endif