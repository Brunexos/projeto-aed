#ifndef TABULEIRO_H
#define TABULEIRO_H

#include <stdio.h>
#include <stdlib.h>
#include "listade.h"

typedef enum {
    NORMAL,
    PRISAO,
    PERGUNTA
} TipoCasa;

typedef enum {
    SEM_NIVEL,
    FACIL,
    MEDIO,
    DIFICIL
} NivelPergunta;

typedef struct Casa {
    int id;
    char nome[100];
    TipoCasa tipo;

    /*
        Estes ponteiros mantêm a leitura visual do tabuleiro.
        Eles também ajudam em buscas e impressão.
    */
    struct Casa *prox;
    struct Casa *ant;

    /*
        Este ponteiro conecta a Casa ao nó real da biblioteca listade.h.
        Assim, o movimento do jogador usa a lista duplamente encadeada.
    */
    tp_no *noLista;
} Casa;

Casa* criarCasa(int id, const char *nome, TipoCasa tipo);
void inserirCasaFim(Casa **inicio, Casa **fim, Casa *nova);
void criarTabuleiroPadrao(Casa **inicio, Casa **fim);
Casa* moverCasas(Casa *atual, int passos);
void mostrarCasaAtual(Casa *casa);
void liberarTabuleiro(Casa *inicio);

#endif