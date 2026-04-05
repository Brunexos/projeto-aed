#ifndef FILA_H
#define FILA_H

#include <stdio.h>

#define MAXF 300

typedef struct {
    int id;
    char nome[50];
    int posicao; // casa do peão
    char peao;   
} Jogador;

typedef Jogador tp_item;

typedef struct {
    tp_item item[MAXF];
    int ini, fim;
} tp_fila;

// Protótipos
void inicializaFila(tp_fila *f);
int filaVazia(tp_fila *f);
int filaCheia(tp_fila *f);
int insereFila(tp_fila *f, tp_item e);
int removeFila(tp_fila *f, tp_item *e);

#endif