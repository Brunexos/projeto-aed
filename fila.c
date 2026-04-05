#include "fila.h"

// Função auxiliar interna
static int proximo(int pos) {
    return (pos == MAXF - 1) ? 0 : pos + 1;
}

void inicializaFila(tp_fila *f) {
    f->ini = f->fim = MAXF - 1;
}

int filaVazia(tp_fila *f) {
    return (f->ini == f->fim);
}

int filaCheia(tp_fila *f) {
    return (proximo(f->fim) == f->ini);
}

int insereFila(tp_fila *f, tp_item e) {
    if (filaCheia(f)) return 0;
    f->fim = proximo(f->fim);
    f->item[f->fim] = e;
    return 1;
}

int removeFila(tp_fila *f, tp_item *e) {
    if (filaVazia(f)) return 0;
    f->ini = proximo(f->ini);
    *e = f->item[f->ini];
    return 1;
}