#include "historico.h"
#include <string.h>

void inicializarHistorico(Historico *h) {
    h->qtd = 0;

    for (int i = 0; i < MAX_LOGS; i++) {
        h->logs[i][0] = '\0';
    }
}

void limparHistorico(Historico *h) {
    inicializarHistorico(h);
}

void adicionarLog(Historico *h, const char *mensagem) {
    if (h == NULL || mensagem == NULL) {
        return;
    }

    if (h->qtd < MAX_LOGS) {
        strncpy(h->logs[h->qtd], mensagem, TAM_LOG - 1);
        h->logs[h->qtd][TAM_LOG - 1] = '\0';
        h->qtd++;
    } else {
        for (int i = 0; i < MAX_LOGS - 1; i++) {
            strcpy(h->logs[i], h->logs[i + 1]);
        }

        strncpy(h->logs[MAX_LOGS - 1], mensagem, TAM_LOG - 1);
        h->logs[MAX_LOGS - 1][TAM_LOG - 1] = '\0';
    }
}