#ifndef LISTADE_H
#define LISTADE_H 

#include <stdio.h>
#include <stdlib.h>

/*
    Biblioteca de lista duplamente encadeada.

    Usei tp_itemd para não conflitar com:
    - tp_item  da fila
    - tp_itemp da pilha
*/

typedef int tp_itemd;

typedef struct tp_no_aux {
    struct tp_no_aux *ant;
    tp_itemd info;
    struct tp_no_aux *prox;
} tp_no;

typedef struct {
    tp_no *ini;
    tp_no *fim;
} tp_listad;

tp_listad *inicializa_listad();
int listad_vazia(tp_listad *lista);
tp_no *aloca_listad();
int insere_listad_no_fim(tp_listad *lista, tp_itemd e);
void imprime_listad(tp_listad *lista, int ordem);
int remove_listad(tp_listad *lista, tp_itemd e);
tp_no *busca_listade(tp_listad *lista, tp_itemd e);
tp_listad *Destroi_listad(tp_listad *lista);
void inverte_listad(tp_listad *lista);

#endif