#ifndef QUESTOES_H
#define QUESTOES_H

#include "pilha.h"
#include "tabuleiro.h"

typedef struct {
    int id;
    char unidade[100];
    char tema[100];
    char subtema[100];
    char dificuldade[20];
    char enunciado[500];
    char alternativas[4][500];
    int respostaCorreta;
} Questao;

typedef struct {
    Questao questao;
    int respostaJogador;
    int acertou;
} ResultadoPergunta;

void QuestoesJogo();
int responderPerguntaNivel(NivelPergunta nivel);
int responderPerguntaNivelDetalhado(NivelPergunta nivel, ResultadoPergunta *resultado);
void salvarPerguntasCSV();
int totalPerguntasJogo();
const Questao* obterQuestaoPorIndice(int indice);
int sortearIndicePerguntaNivel(NivelPergunta nivel);

#endif
