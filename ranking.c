#include "ranking.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ARQUIVO_RANKING_PARTIDAS "ranking_partidas.csv"
#define ARQUIVO_RANKING_GERAL "ranking_geral.csv"
#define ARQUIVO_HISTORICO_RESPOSTAS "historico_respostas.csv"
#define MAX_ANALISE_TEMAS 100

typedef struct {
    char tema[120];
    int acertos;
    int erros;
} AnaliseTema;

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

static int jogadorVemAntes(tp_item a, tp_item b) {
    if (a.pontuacao != b.pontuacao) {
        return a.pontuacao > b.pontuacao;
    }

    if (a.acertos != b.acertos) {
        return a.acertos > b.acertos;
    }

    if (a.erros != b.erros) {
        return a.erros < b.erros;
    }

    return a.posicaoAtual > b.posicaoAtual;
}

void ordenarJogadoresPartida(tp_item jogadores[], int quantidade) {
    for (int i = 0; i < quantidade - 1; i++) {
        for (int j = 0; j < quantidade - i - 1; j++) {
            if (!jogadorVemAntes(jogadores[j], jogadores[j + 1])) {
                tp_item temp = jogadores[j];
                jogadores[j] = jogadores[j + 1];
                jogadores[j + 1] = temp;
            }
        }
    }
}

int obterProximoIdPartida() {
    FILE *arquivo = fopen(ARQUIVO_RANKING_PARTIDAS, "r");
    char linha[300];
    int maiorId = 0;

    if (arquivo == NULL) {
        return 1;
    }

    while (fgets(linha, sizeof(linha), arquivo) != NULL) {
        int idAtual = 0;

        if (sscanf(linha, "%d;", &idAtual) == 1 && idAtual > maiorId) {
            maiorId = idAtual;
        }
    }

    fclose(arquivo);

    return maiorId + 1;
}

void exibirRankingPartida(tp_item jogadores[], int quantidade, const char *nomeVencedor) {
    printf("============== RANKING DA PARTIDA ==============\n");
    printf("Vencedor: %s\n\n", nomeVencedor);
    printf("%-8s %-25s %-10s %-10s %-10s\n", "Rank", "Nome", "Acertos", "Erros", "Pontos");

    for (int i = 0; i < quantidade; i++) {
        printf(
            "%-8d %-25s %-10d %-10d %-10d\n",
            i + 1,
            jogadores[i].nome,
            jogadores[i].acertos,
            jogadores[i].erros,
            jogadores[i].pontuacao
        );
    }
}

void salvarRankingPartidaCSV(int idPartida, tp_item jogadores[], int quantidade) {
    int precisaCabecalho = arquivoPrecisaCabecalho(ARQUIVO_RANKING_PARTIDAS);
    FILE *arquivo = fopen(ARQUIVO_RANKING_PARTIDAS, "a");

    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo %s.\n", ARQUIVO_RANKING_PARTIDAS);
        return;
    }

    if (precisaCabecalho) {
        fprintf(arquivo, "id_partida;nome_jogador;acertos;erros;pontuacao\n");
    }

    for (int i = 0; i < quantidade; i++) {
        char nomeCSV[100];

        copiarCampoCSV(nomeCSV, jogadores[i].nome, sizeof(nomeCSV));

        fprintf(
            arquivo,
            "%d;%s;%d;%d;%d\n",
            idPartida,
            nomeCSV,
            jogadores[i].acertos,
            jogadores[i].erros,
            jogadores[i].pontuacao
        );
    }

    fclose(arquivo);
}

static int rankingGeralVemAntes(RankingGeralItem a, RankingGeralItem b) {
    if (a.totalPontos != b.totalPontos) {
        return a.totalPontos > b.totalPontos;
    }

    if (a.totalAcertos != b.totalAcertos) {
        return a.totalAcertos > b.totalAcertos;
    }

    if (a.totalErros != b.totalErros) {
        return a.totalErros < b.totalErros;
    }

    return a.partidasJogadas < b.partidasJogadas;
}

static void ordenarRankingGeral(RankingGeralItem ranking[], int quantidade) {
    for (int i = 0; i < quantidade - 1; i++) {
        for (int j = 0; j < quantidade - i - 1; j++) {
            if (!rankingGeralVemAntes(ranking[j], ranking[j + 1])) {
                RankingGeralItem temp = ranking[j];
                ranking[j] = ranking[j + 1];
                ranking[j + 1] = temp;
            }
        }
    }
}

static int encontrarNoRankingGeral(RankingGeralItem ranking[], int quantidade, const char *nome) {
    for (int i = 0; i < quantidade; i++) {
        if (strcmp(ranking[i].nome, nome) == 0) {
            return i;
        }
    }

    return -1;
}

static int carregarRankingGeral(RankingGeralItem ranking[]) {
    FILE *arquivo = fopen(ARQUIVO_RANKING_GERAL, "r");
    char linha[400];
    int quantidade = 0;

    if (arquivo == NULL) {
        return 0;
    }

    while (fgets(linha, sizeof(linha), arquivo) != NULL && quantidade < MAX_RANKING_GERAL) {
        if (strncmp(linha, "nome_jogador", 12) == 0) {
            continue;
        }

        char *nome = strtok(linha, ";");
        char *acertos = strtok(NULL, ";");
        char *erros = strtok(NULL, ";");
        char *pontos = strtok(NULL, ";");
        char *partidas = strtok(NULL, ";\n\r");

        if (nome == NULL || acertos == NULL || erros == NULL || pontos == NULL || partidas == NULL) {
            continue;
        }

        strncpy(ranking[quantidade].nome, nome, sizeof(ranking[quantidade].nome) - 1);
        ranking[quantidade].nome[sizeof(ranking[quantidade].nome) - 1] = '\0';
        ranking[quantidade].totalAcertos = atoi(acertos);
        ranking[quantidade].totalErros = atoi(erros);
        ranking[quantidade].totalPontos = atoi(pontos);
        ranking[quantidade].partidasJogadas = atoi(partidas);
        quantidade++;
    }

    fclose(arquivo);

    return quantidade;
}

static void salvarRankingGeral(RankingGeralItem ranking[], int quantidade) {
    FILE *arquivo = fopen(ARQUIVO_RANKING_GERAL, "w");

    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo %s.\n", ARQUIVO_RANKING_GERAL);
        return;
    }

    fprintf(arquivo, "nome_jogador;total_acertos;total_erros;total_pontos;partidas_jogadas\n");

    for (int i = 0; i < quantidade; i++) {
        char nomeCSV[100];

        copiarCampoCSV(nomeCSV, ranking[i].nome, sizeof(nomeCSV));

        fprintf(
            arquivo,
            "%s;%d;%d;%d;%d\n",
            nomeCSV,
            ranking[i].totalAcertos,
            ranking[i].totalErros,
            ranking[i].totalPontos,
            ranking[i].partidasJogadas
        );
    }

    fclose(arquivo);
}

static void exibirRankingGeral(RankingGeralItem ranking[], int quantidade) {
    printf("\n============== RANKING GERAL ==============\n");
    printf("%-8s %-25s %-10s %-10s %-10s %-10s\n", "Rank", "Nome", "Acertos", "Erros", "Pontos", "Partidas");

    for (int i = 0; i < quantidade; i++) {
        printf(
            "%-8d %-25s %-10d %-10d %-10d %-10d\n",
            i + 1,
            ranking[i].nome,
            ranking[i].totalAcertos,
            ranking[i].totalErros,
            ranking[i].totalPontos,
            ranking[i].partidasJogadas
        );
    }
}

static int encontrarTemaAnalise(AnaliseTema analises[], int quantidade, const char *tema) {
    for (int i = 0; i < quantidade; i++) {
        if (strcmp(analises[i].tema, tema) == 0) {
            return i;
        }
    }

    return -1;
}

static int analiseTemaVemAntes(AnaliseTema a, AnaliseTema b) {
    int totalA = a.acertos + a.erros;
    int totalB = b.acertos + b.erros;

    if (totalA != totalB) {
        return totalA > totalB;
    }

    if (a.acertos != b.acertos) {
        return a.acertos > b.acertos;
    }

    return strcmp(a.tema, b.tema) < 0;
}

static void ordenarAnaliseTemas(AnaliseTema analises[], int quantidade) {
    for (int i = 0; i < quantidade - 1; i++) {
        for (int j = 0; j < quantidade - i - 1; j++) {
            if (!analiseTemaVemAntes(analises[j], analises[j + 1])) {
                AnaliseTema temp = analises[j];
                analises[j] = analises[j + 1];
                analises[j + 1] = temp;
            }
        }
    }
}

static int carregarAnaliseTemas(AnaliseTema analises[]) {
    FILE *arquivo = fopen(ARQUIVO_HISTORICO_RESPOSTAS, "r");
    char linha[1000];
    int quantidade = 0;

    if (arquivo == NULL) {
        return 0;
    }

    while (fgets(linha, sizeof(linha), arquivo) != NULL) {
        if (strncmp(linha, "turma;", 6) == 0) {
            continue;
        }

        char *turma = strtok(linha, ";");
        char *grupo = strtok(NULL, ";");
        char *nomeJogador = strtok(NULL, ";");
        char *idPergunta = strtok(NULL, ";");
        char *unidade = strtok(NULL, ";");
        char *tema = strtok(NULL, ";");
        char *subtema = strtok(NULL, ";");
        char *dificuldade = strtok(NULL, ";");
        char *respostaJogador = strtok(NULL, ";");
        char *respostaCorreta = strtok(NULL, ";");
        char *resultado = strtok(NULL, ";\n\r");

        (void) turma;
        (void) grupo;
        (void) nomeJogador;
        (void) idPergunta;
        (void) unidade;
        (void) subtema;
        (void) dificuldade;
        (void) respostaJogador;
        (void) respostaCorreta;

        if (tema == NULL || resultado == NULL) {
            continue;
        }

        int indice = encontrarTemaAnalise(analises, quantidade, tema);

        if (indice == -1 && quantidade < MAX_ANALISE_TEMAS) {
            indice = quantidade;
            strncpy(analises[indice].tema, tema, sizeof(analises[indice].tema) - 1);
            analises[indice].tema[sizeof(analises[indice].tema) - 1] = '\0';
            analises[indice].acertos = 0;
            analises[indice].erros = 0;
            quantidade++;
        }

        if (indice != -1) {
            if (strcmp(resultado, "Acertou") == 0) {
                analises[indice].acertos++;
            }
            else if (strcmp(resultado, "Errou") == 0) {
                analises[indice].erros++;
            }
        }
    }

    fclose(arquivo);

    ordenarAnaliseTemas(analises, quantidade);

    return quantidade;
}

static void formatarPercentual(double valor, char *destino, int tamanhoDestino) {
    snprintf(destino, tamanhoDestino, "%.1f%%", valor);

    for (int i = 0; destino[i] != '\0'; i++) {
        if (destino[i] == '.') {
            destino[i] = ',';
        }
    }
}

static void exibirAnaliseTemasHistorico() {
    AnaliseTema analises[MAX_ANALISE_TEMAS];
    int quantidade = carregarAnaliseTemas(analises);

    printf("\n============== ANALISE POR TEMA ==============\n");

    if (quantidade == 0) {
        printf("Nenhuma resposta registrada para analise por tema.\n");
        return;
    }

    printf("%-35s %-10s %-10s %-10s %-15s\n", "Tema", "Acertos", "Erros", "Total", "Aproveitamento");

    for (int i = 0; i < quantidade; i++) {
        int total = analises[i].acertos + analises[i].erros;
        double aproveitamento = 0.0;
        char percentual[20];

        if (total > 0) {
            aproveitamento = (analises[i].acertos * 100.0) / total;
        }

        formatarPercentual(aproveitamento, percentual, sizeof(percentual));

        printf(
            "%-35s %-10d %-10d %-10d %-15s\n",
            analises[i].tema,
            analises[i].acertos,
            analises[i].erros,
            total,
            percentual
        );
    }
}

void atualizarSalvarExibirRankingGeral(tp_item jogadores[], int quantidade) {
    RankingGeralItem ranking[MAX_RANKING_GERAL];
    int quantidadeRanking = carregarRankingGeral(ranking);

    for (int i = 0; i < quantidade; i++) {
        int indice = encontrarNoRankingGeral(ranking, quantidadeRanking, jogadores[i].nome);

        if (indice == -1 && quantidadeRanking < MAX_RANKING_GERAL) {
            indice = quantidadeRanking;
            strncpy(ranking[indice].nome, jogadores[i].nome, sizeof(ranking[indice].nome) - 1);
            ranking[indice].nome[sizeof(ranking[indice].nome) - 1] = '\0';
            ranking[indice].totalAcertos = 0;
            ranking[indice].totalErros = 0;
            ranking[indice].totalPontos = 0;
            ranking[indice].partidasJogadas = 0;
            quantidadeRanking++;
        }

        if (indice != -1) {
            ranking[indice].totalAcertos += jogadores[i].acertos;
            ranking[indice].totalErros += jogadores[i].erros;
            ranking[indice].totalPontos += jogadores[i].pontuacao;
            ranking[indice].partidasJogadas += 1;
        }
    }

    ordenarRankingGeral(ranking, quantidadeRanking);
    salvarRankingGeral(ranking, quantidadeRanking);
    exibirRankingGeral(ranking, quantidadeRanking);
    exibirAnaliseTemasHistorico();
}

void exibirRankingGeralSalvo() {
    RankingGeralItem ranking[MAX_RANKING_GERAL];
    int quantidadeRanking = carregarRankingGeral(ranking);

    if (quantidadeRanking == 0) {
        printf("============== RANKING GERAL ==============\n\n");
        printf("Nenhuma partida foi registrada ainda.\n");
        exibirAnaliseTemasHistorico();
        return;
    }

    ordenarRankingGeral(ranking, quantidadeRanking);
    exibirRankingGeral(ranking, quantidadeRanking);
    exibirAnaliseTemasHistorico();
}
