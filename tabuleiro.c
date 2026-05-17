#include "tabuleiro.h"
#include <string.h>

Casa* criarCasa(int id, const char *nome, TipoCasa tipo) {
    Casa *nova = (Casa*) malloc(sizeof(Casa));

    if (nova == NULL) {
        printf("Erro ao alocar memoria para casa.\n");
        exit(1);
    }

    nova->id = id;
    strcpy(nova->nome, nome);
    nova->tipo = tipo;
    nova->prox = NULL;
    nova->ant = NULL;

    return nova;
}

void inserirCasaFim(Casa **inicio, Casa **fim, Casa *nova) {
    if (*inicio == NULL) {
        *inicio = nova;
        *fim = nova;
    } else {
        (*fim)->prox = nova;
        nova->ant = *fim;
        *fim = nova;
    }
}

void criarTabuleiroPadrao(Casa **inicio, Casa **fim) {
    *inicio = NULL;
    *fim = NULL;

    for (int i = 0; i <= 30; i++) {
        TipoCasa tipo = NORMAL;
        char nome[100];

        sprintf(nome, "Casa %d", i);

        if (i == 6 || i == 18) {
            tipo = PRISAO;
            strcpy(nome, "Prisao");
        }
        else if (i == 4 || i == 8 || i == 10 || i == 12 || i == 16 || i == 20 || i == 22 || i == 24 || i == 28) {
            tipo = PERGUNTA;
            strcpy(nome, "Pergunta");
        }

        inserirCasaFim(inicio, fim, criarCasa(i, nome, tipo));
    }
}

Casa* moverCasas(Casa *atual, int passos) {
    if (atual == NULL) {
        return NULL;
    }

    if (passos > 0) {
        for (int i = 0; i < passos; i++) {
            if (atual->prox != NULL) {
                atual = atual->prox;
            }
        }
    }
    else if (passos < 0) {
        for (int i = 0; i < -passos; i++) {
            if (atual->ant != NULL) {
                atual = atual->ant;
            }
        }
    }

    return atual;
}

void mostrarCasaAtual(Casa *casa) {
    if (casa == NULL) {
        return;
    }

    printf("Casa atual: %d - %s\n", casa->id, casa->nome);
}

void liberarTabuleiro(Casa *inicio) {
    Casa *atual = inicio;

    while (atual != NULL) {
        Casa *prox = atual->prox;
        free(atual);
        atual = prox;
    }
}