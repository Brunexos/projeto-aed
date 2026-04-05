#ifndef JOGADOR_H
#define JOGADOR_H

#include "fila.h"

// Adicione estas linhas aqui para que a main possa enxergar
#define RED     "\x1b[31m"
#define GREEN   "\x1b[32m"
#define YELLOW  "\x1b[33m"
#define BLUE    "\x1b[34m"
#define RESET   "\x1b[0m"

void cadastrarJogadores(tp_fila *f);

#endif