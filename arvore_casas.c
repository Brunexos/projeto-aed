#include "arvore_casas.h"

#include <stdio.h>
#include <stdlib.h>

static int maior(int a, int b) {
    return (a > b) ? a : b;
}

static int alturaNo(NoCasa *no) {
    return (no == NULL) ? 0 : no->altura;
}

static NoCasa* criarNoCasa(int idCasa) {
    NoCasa *novo = (NoCasa*) malloc(sizeof(NoCasa));

    if (novo == NULL) {
        printf("Erro ao alocar memoria para a arvore AVL de casas.\n");
        exit(1);
    }

    novo->idCasa = idCasa;
    novo->quantidadeQuedas = 1;
    novo->altura = 1;
    novo->esquerda = NULL;
    novo->direita = NULL;

    return novo;
}

static int fatorBalanceamento(NoCasa *no) {
    if (no == NULL) {
        return 0;
    }

    return alturaNo(no->esquerda) - alturaNo(no->direita);
}

static NoCasa* rotacaoDireita(NoCasa *y) {
    NoCasa *x = y->esquerda;
    NoCasa *t2 = x->direita;

    x->direita = y;
    y->esquerda = t2;

    y->altura = maior(alturaNo(y->esquerda), alturaNo(y->direita)) + 1;
    x->altura = maior(alturaNo(x->esquerda), alturaNo(x->direita)) + 1;

    return x;
}

static NoCasa* rotacaoEsquerda(NoCasa *x) {
    NoCasa *y = x->direita;
    NoCasa *t2 = y->esquerda;

    y->esquerda = x;
    x->direita = t2;

    x->altura = maior(alturaNo(x->esquerda), alturaNo(x->direita)) + 1;
    y->altura = maior(alturaNo(y->esquerda), alturaNo(y->direita)) + 1;

    return y;
}

static NoCasa* inserirOuIncrementar(NoCasa *raiz, int idCasa) {
    if (raiz == NULL) {
        return criarNoCasa(idCasa);
    }

    if (idCasa < raiz->idCasa) {
        raiz->esquerda = inserirOuIncrementar(raiz->esquerda, idCasa);
    }
    else if (idCasa > raiz->idCasa) {
        raiz->direita = inserirOuIncrementar(raiz->direita, idCasa);
    }
    else {
        raiz->quantidadeQuedas++;
        return raiz;
    }

    raiz->altura = maior(alturaNo(raiz->esquerda), alturaNo(raiz->direita)) + 1;

    int balanceamento = fatorBalanceamento(raiz);

    if (balanceamento > 1 && idCasa < raiz->esquerda->idCasa) {
        return rotacaoDireita(raiz);
    }

    if (balanceamento < -1 && idCasa > raiz->direita->idCasa) {
        return rotacaoEsquerda(raiz);
    }

    if (balanceamento > 1 && idCasa > raiz->esquerda->idCasa) {
        raiz->esquerda = rotacaoEsquerda(raiz->esquerda);
        return rotacaoDireita(raiz);
    }

    if (balanceamento < -1 && idCasa < raiz->direita->idCasa) {
        raiz->direita = rotacaoDireita(raiz->direita);
        return rotacaoEsquerda(raiz);
    }

    return raiz;
}

void registrarQuedaCasa(NoCasa **raiz, int idCasa) {
    if (raiz == NULL) {
        return;
    }

    *raiz = inserirOuIncrementar(*raiz, idCasa);
}

void exibirQuedasCasas(NoCasa *raiz) {
    if (raiz == NULL) {
        return;
    }

    exibirQuedasCasas(raiz->esquerda);
    printf("Casa %d: %d queda(s)\n", raiz->idCasa, raiz->quantidadeQuedas);
    exibirQuedasCasas(raiz->direita);
}

static void salvarNoCSV(NoCasa *raiz, FILE *arquivo) {
    if (raiz == NULL || arquivo == NULL) {
        return;
    }

    salvarNoCSV(raiz->esquerda, arquivo);
    fprintf(arquivo, "%d;%d\n", raiz->idCasa, raiz->quantidadeQuedas);
    salvarNoCSV(raiz->direita, arquivo);
}

void salvarQuedasCasasCSV(NoCasa *raiz, const char *nomeArquivo) {
    FILE *arquivo = fopen(nomeArquivo, "w");

    if (arquivo == NULL) {
        printf("Erro ao salvar o arquivo %s.\n", nomeArquivo);
        return;
    }

    fprintf(arquivo, "id_casa;quantidade_quedas\n");
    salvarNoCSV(raiz, arquivo);

    fclose(arquivo);
}

void liberarArvoreCasas(NoCasa *raiz) {
    if (raiz == NULL) {
        return;
    }

    liberarArvoreCasas(raiz->esquerda);
    liberarArvoreCasas(raiz->direita);
    free(raiz);
}
