#ifndef JOGADOR_H
#define JOGADOR_H

#include "fila.h"
#include "tabuleiro.h"
#include "historico.h"

#define RED     "\x1b[31m"
#define GREEN   "\x1b[32m"
#define YELLOW  "\x1b[33m"
#define BLUE    "\x1b[34m"
#define RESET   "\x1b[0m"

void cadastrarJogadores(tp_fila *f, Casa *inicioTabuleiro);
int realizarJogada(tp_fila *f, Casa *inicioTabuleiro, Historico *historico);

#endif