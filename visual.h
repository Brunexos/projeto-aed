#ifndef VISUAL_H
#define VISUAL_H

#include "fila.h"
#include "tabuleiro.h"
#include "historico.h"

void desenharTabuleiro(Casa *inicio, tp_fila *fila, tp_item *jogadorDaVez);

void desenharTelaJogo(
    Casa *inicio,
    tp_fila *fila,
    tp_item *jogadorDaVez,
    Historico *historico,
    const char *status,
    int dado
);

#endif