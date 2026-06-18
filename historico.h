#ifndef HISTORICO_H
#define HISTORICO_H

#include "questoes.h"

#define MAX_LOGS 6
#define TAM_LOG 150

#define TURMA_PADRAO "AED"
#define GRUPO_PADRAO "2026.1"

typedef struct {
    char logs[MAX_LOGS][TAM_LOG];
    int qtd;
} Historico;

void inicializarHistorico(Historico *h);
void adicionarLog(Historico *h, const char *mensagem);
void limparHistorico(Historico *h);
void inicializarHistoricoRespostasCSV();
void salvarRespostaHistoricoCSV(
    const char *turma,
    const char *grupo,
    const char *nomeJogador,
    ResultadoPergunta *resultado
);

#endif
