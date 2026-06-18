#ifndef RANKING_H
#define RANKING_H

#include "fila.h"

#define MAX_RANKING_GERAL 300

typedef struct {
    char nome[50];
    int totalAcertos;
    int totalErros;
    int totalPontos;
    int partidasJogadas;
} RankingGeralItem;

int obterProximoIdPartida();
void ordenarJogadoresPartida(tp_item jogadores[], int quantidade);
void exibirRankingPartida(tp_item jogadores[], int quantidade, const char *nomeVencedor);
void salvarRankingPartidaCSV(int idPartida, tp_item jogadores[], int quantidade);
void atualizarSalvarExibirRankingGeral(tp_item jogadores[], int quantidade);
void exibirRankingGeralSalvo();

#endif
