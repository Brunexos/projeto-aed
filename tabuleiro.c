#include "tabuleiro.h"
#include <string.h>

#define TOTAL_CASAS 31

/*
    Esta lista guarda os IDs das casas:
    0 <-> 1 <-> 2 <-> ... <-> 30

    Ela é a aplicação direta da biblioteca listade.h no tabuleiro.
*/
static tp_listad *listaIdsTabuleiro = NULL;

/*
    Este vetor serve como mapa rápido:
    id da casa -> ponteiro Casa*
*/
static Casa *mapaCasas[TOTAL_CASAS];

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

    nova->noLista = NULL;

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

    if (listaIdsTabuleiro != NULL) {
        listaIdsTabuleiro = Destroi_listad(listaIdsTabuleiro);
    }

    listaIdsTabuleiro = inicializa_listad();

    if (listaIdsTabuleiro == NULL) {
        printf("Erro ao inicializar lista duplamente encadeada do tabuleiro.\n");
        exit(1);
    }

    for (int i = 0; i < TOTAL_CASAS; i++) {
        mapaCasas[i] = NULL;
    }

    for (int i = 0; i <= 30; i++) {
        TipoCasa tipo = NORMAL;
        char nome[100];

        sprintf(nome, "Casa %d", i);

        if (i == 6 || i == 18) {
            tipo = PRISAO;
            strcpy(nome, "Prisao");
        }
        else if (
            i == 4  ||
            i == 8  ||
            i == 10 ||
            i == 12 ||
            i == 16 ||
            i == 20 ||
            i == 22 ||
            i == 24 ||
            i == 28
        ) {
            tipo = PERGUNTA;
            strcpy(nome, "Pergunta");
        }

        /*
            Aqui a casa é registrada na lista duplamente encadeada.
            A lista guarda o ID da casa.
        */
        if (!insere_listad_no_fim(listaIdsTabuleiro, i)) {
            printf("Erro ao inserir casa na lista duplamente encadeada.\n");
            exit(1);
        }

        Casa *nova = criarCasa(i, nome, tipo);

        /*
            O nó recém-inserido é sempre o fim da lista.
            Então associamos a casa ao nó da lista.
        */
        nova->noLista = listaIdsTabuleiro->fim;

        mapaCasas[i] = nova;

        /*
            Estes ponteiros Casa->prox e Casa->ant mantêm compatibilidade
            com o desenho e com buscas visuais do tabuleiro.
        */
        inserirCasaFim(inicio, fim, nova);
    }
}

Casa* moverCasas(Casa *atual, int passos) {
    if (atual == NULL || atual->noLista == NULL) {
        return atual;
    }

    tp_no *noAtual = atual->noLista;

    if (passos > 0) {
        for (int i = 0; i < passos; i++) {
            if (noAtual->prox != NULL) {
                noAtual = noAtual->prox;
            }
        }
    }
    else if (passos < 0) {
        for (int i = 0; i < -passos; i++) {
            if (noAtual->ant != NULL) {
                noAtual = noAtual->ant;
            }
        }
    }

    int idDestino = noAtual->info;

    if (idDestino < 0 || idDestino >= TOTAL_CASAS) {
        return atual;
    }

    return mapaCasas[idDestino];
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

    if (listaIdsTabuleiro != NULL) {
        listaIdsTabuleiro = Destroi_listad(listaIdsTabuleiro);
    }

    for (int i = 0; i < TOTAL_CASAS; i++) {
        mapaCasas[i] = NULL;
    }
}