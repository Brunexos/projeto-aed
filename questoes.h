#ifndef QUESTOES_H
#define QUESTOES_H

#include "pilha.h"
#include "tabuleiro.h"

typedef struct {
    char pergunta[500];
    char alternativas[4][500];
    int respostaCorreta;
} Questao;

void QuestoesJogo();
int responderPerguntaNivel(NivelPergunta nivel);

#endif