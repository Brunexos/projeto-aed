#ifndef FILA_H
#define FILA_H

#include <stdio.h>
#include "tabuleiro.h"

#define MAXF 300

typedef enum {
    COR_VERDE,
    COR_AZUL,
    COR_CIANO,
    COR_MAGENTA,
    COR_BRANCO
} CorJogador;

typedef struct {
    int id;
    char nome[50];
    Casa *casaAtual;
    CorJogador cor;
    int preso;
} Jogador;

typedef Jogador tp_item;

typedef struct {
    tp_item item[MAXF];
    int ini, fim;
} tp_fila;

void inicializaFila(tp_fila *f);
int filaVazia(tp_fila *f);
int filaCheia(tp_fila *f);
int insereFila(tp_fila *f, tp_item e);
int removeFila(tp_fila *f, tp_item *e);

#endif