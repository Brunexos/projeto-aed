#include "pilha.h"

void inicializa_pilha(tp_pilha *p) {
    p->topo = -1;
}

int pilha_vazia(tp_pilha *p) {
    return (p->topo == -1);
}

int pilha_cheia(tp_pilha *p) {
    return (p->topo == MAX - 1);
}

int push(tp_pilha *p, tp_itemp e) {
    if (pilha_cheia(p)) return 0;
    p->topo++;
    p->item[p->topo] = e;
    return 1;
}

int pop(tp_pilha *p, tp_itemp *e) {
    if (pilha_vazia(p)) return 0;
    *e = p->item[p->topo];
    p->topo--;
    return 1;
}

int top(tp_pilha *p, tp_itemp *e) {
    if (pilha_vazia(p)) return 0;
    *e = p->item[p->topo];
    return 1;
}

void imprime_pilha(tp_pilha p) {
    tp_itemp e;
    printf("\n");
    while (!pilha_vazia(&p)) {
        pop(&p, &e);
        printf("%d\n", e);
    }
}

int altura_pilha(tp_pilha *p) {
    return p->topo + 1;
}