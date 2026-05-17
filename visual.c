#include "visual.h"
#include <stdio.h>
#include <string.h>

#define RED     "\x1b[31m"
#define GREEN   "\x1b[32m"
#define YELLOW  "\x1b[33m"
#define BLUE    "\x1b[34m"
#define MAGENTA "\x1b[35m"
#define CYAN    "\x1b[36m"
#define WHITE   "\x1b[37m"
#define RESET   "\x1b[0m"

static const char* codigoCorJogador(CorJogador cor) {
    switch (cor) {
        case COR_VERDE:
            return GREEN;
        case COR_AZUL:
            return BLUE;
        case COR_CIANO:
            return CYAN;
        case COR_MAGENTA:
            return MAGENTA;
        case COR_BRANCO:
            return WHITE;
        default:
            return WHITE;
    }
}

static Casa* buscarCasaPorId(Casa *inicio, int id) {
    Casa *atual = inicio;

    while (atual != NULL) {
        if (atual->id == id) {
            return atual;
        }

        atual = atual->prox;
    }

    return NULL;
}

static void copiarNomeCurto(char *destino, const char *origem, int limite) {
    int i;

    for (i = 0; i < limite && origem[i] != '\0'; i++) {
        destino[i] = origem[i];
    }

    destino[i] = '\0';
}

static int buscarJogadorNaCasa(tp_fila *fila, tp_item *jogadorDaVez, int idCasa, tp_item *encontrado, int *quantidade) {
    int achou = 0;
    *quantidade = 0;

    if (jogadorDaVez != NULL && jogadorDaVez->casaAtual != NULL) {
        if (jogadorDaVez->casaAtual->id == idCasa) {
            if (!achou) {
                *encontrado = *jogadorDaVez;
                achou = 1;
            }

            (*quantidade)++;
        }
    }

    if (fila == NULL) {
        return achou;
    }

    int idx = fila->ini;

    while (idx != fila->fim) {
        idx = (idx == MAXF - 1) ? 0 : idx + 1;

        tp_item jogador = fila->item[idx];

        if (jogador.casaAtual != NULL && jogador.casaAtual->id == idCasa) {
            if (!achou) {
                *encontrado = jogador;
                achou = 1;
            }

            (*quantidade)++;
        }
    }

    return achou;
}

static void imprimirCasa(Casa *casa, tp_fila *fila, tp_item *jogadorDaVez) {
    if (casa == NULL) {
        printf("                ");
        return;
    }

    char marcador;
    const char *corCasa;

    if (casa->tipo == PRISAO) {
        marcador = 'P';
        corCasa = RED;
    }
    else if (casa->tipo == PERGUNTA) {
        marcador = '?';
        corCasa = YELLOW;
    }
    else {
        marcador = '.';
        corCasa = WHITE;
    }

    tp_item jogadorEncontrado;
    int quantidade = 0;
    int temJogador = buscarJogadorNaCasa(fila, jogadorDaVez, casa->id, &jogadorEncontrado, &quantidade);

    printf("%s[%02d%c " RESET, corCasa, casa->id, marcador);

    if (temJogador) {
        char nomeCurto[10];

        copiarNomeCurto(nomeCurto, jogadorEncontrado.nome, 8);

        printf("%s%-8s" RESET, codigoCorJogador(jogadorEncontrado.cor), nomeCurto);

        if (quantidade > 1) {
            printf("%s+%d" RESET, codigoCorJogador(jogadorEncontrado.cor), quantidade - 1);
        } else {
            printf("  ");
        }
    } else {
        printf("          ");
    }

    printf("%s]" RESET, corCasa);
}

void desenharTabuleiro(Casa *inicio, tp_fila *fila, tp_item *jogadorDaVez) {
    printf("\n");

    printf(CYAN "=============================== TABULEIRO ===============================\n" RESET);

    printf(WHITE "Legenda: " RESET);
    printf(WHITE "[00.]" RESET " Normal  ");
    printf(YELLOW "[04?]" RESET " Pergunta  ");
    printf(RED "[06P]" RESET " Prisao  ");
    printf(CYAN "Nome colorido = jogador\n\n" RESET);

    int linhas[6][6] = {
        {0, 1, 2, 3, 4, 5},
        {11, 10, 9, 8, 7, 6},
        {12, 13, 14, 15, 16, 17},
        {23, 22, 21, 20, 19, 18},
        {24, 25, 26, 27, 28, 29},
        {30, -1, -1, -1, -1, -1}
    };

    for (int l = 0; l < 6; l++) {
        for (int c = 0; c < 6; c++) {
            int id = linhas[l][c];

            if (id == -1) {
                printf("                ");
            } else {
                Casa *casa = buscarCasaPorId(inicio, id);
                imprimirCasa(casa, fila, jogadorDaVez);
            }

            if (c < 5) {
                printf("--");
            }
        }

        printf("\n");

        if (l < 5) {
            printf("      |                                                                                             \n");
            printf("      |                                                                                             \n");
        }
    }

    printf("\n");
}