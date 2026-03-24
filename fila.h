#ifndef FILA_H
#define FILA_H

#include <stdio.h>
#include <string.h>

#define MAXF 300

// 1. Defina a estrutura primeiro
typedef struct {
    int id;
    char nome[50];
    int posicao;
    char peao; // Adicionado para bater com seu main()
} Jogador;

// 2. Agora defina o item da fila como sendo um Jogador
typedef Jogador tp_item;

typedef struct {
    tp_item item[MAXF];
    int ini, fim;
} tp_fila;

void inicializaFila(tp_fila *f) {
    f->ini = f->fim = MAXF - 1;
}

int filaVazia(tp_fila *f) {
    return (f->ini == f->fim);
}

int proximo(int pos) {
    return (pos == MAXF - 1) ? 0 : pos + 1;
}

int filaCheia(tp_fila *f) {
    return (proximo(f->fim) == f->ini);
}

int insereFila(tp_fila *f, tp_item e) {
    if (filaCheia(f)) return 0;
    f->fim = proximo(f->fim);
    f->item[f->fim] = e;
    return 1; // Faltava o return de sucesso
}

int removeFila(tp_fila *f, tp_item *e) {
    if (filaVazia(f)) return 0;
    f->ini = proximo(f->ini);
    *e = f->item[f->ini];
    return 1; // Faltava o return de sucesso
}
#endif