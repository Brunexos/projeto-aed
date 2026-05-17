#ifndef HISTORICO_H
#define HISTORICO_H

#define MAX_LOGS 6
#define TAM_LOG 150

typedef struct {
    char logs[MAX_LOGS][TAM_LOG];
    int qtd;
} Historico;

void inicializarHistorico(Historico *h);
void adicionarLog(Historico *h, const char *mensagem);
void limparHistorico(Historico *h);

#endif