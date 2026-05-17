#ifndef TABULEIRO_H
#define TABULEIRO_H

#include <stdio.h>
#include <stdlib.h>

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
    struct Casa *prox;
    struct Casa *ant;
} Casa;

Casa* criarCasa(int id, const char *nome, TipoCasa tipo);
void inserirCasaFim(Casa **inicio, Casa **fim, Casa *nova);
void criarTabuleiroPadrao(Casa **inicio, Casa **fim);
Casa* moverCasas(Casa *atual, int passos);
void mostrarCasaAtual(Casa *casa);
void liberarTabuleiro(Casa *inicio);

#endif