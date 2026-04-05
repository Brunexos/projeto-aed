#ifndef QUESTOES_H
#define QUESTOES_H

#include "pilha.h"

// Definição das cores (se estiver usando no terminal)
#define RED "\033[31m"
#define RESET "\033[0m"

// Estrutura da Questão
typedef struct {
    char pergunta[500];
    char alternativas[4][500];
    int respostaCorreta;
} Questao;

// Protótipo da função principal que será chamada na main.c
void QuestoesJogo();

#endif