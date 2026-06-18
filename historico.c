#include "historico.h"
#include <stdio.h>
#include <string.h>

#define ARQUIVO_HISTORICO_RESPOSTAS "historico_respostas.csv"

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

static int arquivoPrecisaCabecalho(const char *nomeArquivo) {
    FILE *arquivo = fopen(nomeArquivo, "r");

    if (arquivo == NULL) {
        return 1;
    }

    int primeiro = fgetc(arquivo);
    fclose(arquivo);

    return primeiro == EOF;
}

static void copiarCampoCSV(char *destino, const char *origem, int tamanhoDestino) {
    int i = 0;

    if (tamanhoDestino <= 0) {
        return;
    }

    if (origem == NULL) {
        destino[0] = '\0';
        return;
    }

    while (origem[i] != '\0' && i < tamanhoDestino - 1) {
        if (origem[i] == ';' || origem[i] == '\n' || origem[i] == '\r') {
            destino[i] = ' ';
        } else {
            destino[i] = origem[i];
        }

        i++;
    }

    destino[i] = '\0';
}

static char letraAlternativa(int indice) {
    if (indice < 0 || indice > 3) {
        return '-';
    }

    return (char) ('A' + indice);
}

void inicializarHistoricoRespostasCSV() {
    if (!arquivoPrecisaCabecalho(ARQUIVO_HISTORICO_RESPOSTAS)) {
        return;
    }

    FILE *arquivo = fopen(ARQUIVO_HISTORICO_RESPOSTAS, "w");

    if (arquivo == NULL) {
        printf("Erro ao criar o arquivo %s.\n", ARQUIVO_HISTORICO_RESPOSTAS);
        return;
    }

    fprintf(
        arquivo,
        "turma;grupo;nome_jogador;id_pergunta;unidade;tema;subtema;dificuldade;resposta_jogador;resposta_correta;resultado\n"
    );

    fclose(arquivo);
}

void salvarRespostaHistoricoCSV(
    const char *turma,
    const char *grupo,
    const char *nomeJogador,
    ResultadoPergunta *resultado
) {
    if (resultado == NULL) {
        return;
    }

    inicializarHistoricoRespostasCSV();

    FILE *arquivo = fopen(ARQUIVO_HISTORICO_RESPOSTAS, "a");

    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo %s.\n", ARQUIVO_HISTORICO_RESPOSTAS);
        return;
    }

    Questao *questao = &resultado->questao;
    char idPergunta[10];

    char turmaCSV[100];
    char grupoCSV[100];
    char nomeCSV[100];
    char unidadeCSV[100];
    char temaCSV[120];
    char subtemaCSV[120];
    char dificuldadeCSV[40];
    snprintf(idPergunta, sizeof(idPergunta), "P%03d", questao->id);

    copiarCampoCSV(turmaCSV, turma, sizeof(turmaCSV));
    copiarCampoCSV(grupoCSV, grupo, sizeof(grupoCSV));
    copiarCampoCSV(nomeCSV, nomeJogador, sizeof(nomeCSV));
    copiarCampoCSV(unidadeCSV, questao->unidade, sizeof(unidadeCSV));
    copiarCampoCSV(temaCSV, questao->tema, sizeof(temaCSV));
    copiarCampoCSV(subtemaCSV, questao->subtema, sizeof(subtemaCSV));
    copiarCampoCSV(dificuldadeCSV, questao->dificuldade, sizeof(dificuldadeCSV));

    fprintf(
        arquivo,
        "%s;%s;%s;%s;%s;%s;%s;%s;%c;%c;%s\n",
        turmaCSV,
        grupoCSV,
        nomeCSV,
        idPergunta,
        unidadeCSV,
        temaCSV,
        subtemaCSV,
        dificuldadeCSV,
        letraAlternativa(resultado->respostaJogador),
        letraAlternativa(questao->respostaCorreta),
        resultado->acertou ? "Acertou" : "Errou"
    );

    fclose(arquivo);
}
