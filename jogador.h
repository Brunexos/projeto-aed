#ifndef JOGADOR_H
#define JOGADOR_H

#include <stdio.h>
#include <stdlib.h>
#include "graficos.h"

typedef struct Jogador {
    char nome[50];
    char icone;      // O caractere dentro do peão (ex: @, X)
    int cor_escolhida; // 1 a 4
    int acertos;
    int erros;
    struct Jogador *prox;
} Jogador;

static Jogador* criar_lista_jogadores(int n) {
    Jogador *primeiro = NULL, *ultimo = NULL;

    for (int i = 0; i < n; i++) {
        Jogador *novo = (Jogador*) malloc(sizeof(Jogador));
        if (!novo) exit(1);

        system("cls");
        printf("--- Cadastro Jogador %d ---\n", i + 1);
        printf("Nome: ");
        scanf(" %[^\n]", novo->nome);

        mostrarOpcoesPeoes();
        
        int esc;
        do {
            printf("Escolha o numero da cor (1-4): ");
            scanf("%d", &esc);
        } while(esc < 1 || esc > 4);
        
        novo->cor_escolhida = esc;

        printf("Digite um caractere para voce ser representado: (ex: %d, X, @): ", i+1);
        scanf(" %c", &novo->icone);

        novo->acertos = 0;
        novo->erros = 0;

        if (primeiro == NULL) {
            primeiro = novo;
            ultimo = novo;
        } else {
            ultimo->prox = novo;
            ultimo = novo;
        }
    }
    ultimo->prox = primeiro; // Fecha a lista circular
    return primeiro;
}

#endif