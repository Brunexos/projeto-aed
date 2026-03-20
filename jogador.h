#ifndef JOGADOR_H
#define JOGADOR_H

#include <stdio.h>
#include <stdlib.h>

// Definição da estrutura do Jogador
typedef struct Jogador {
    char nome[50];
    char peao;
    int acertos;
    int erros;
    struct Jogador *prox; // Lista Circular
} Jogador;

// A função agora vive aqui dentro! 
// Usamos "static" para evitar erros de duplicidade se você incluir em vários arquivos.
static Jogador* criar_lista_jogadores(int n) {
    Jogador *primeiro = NULL;
    Jogador *ultimo = NULL;

    for (int i = 0; i < n; i++) {
        Jogador *novo = (Jogador*) malloc(sizeof(Jogador));
        if (novo == NULL) {
            printf("Erro de memoria!\n");
            exit(1);
        }

        printf("\n--- Cadastro Jogador %d ---\n", i + 1);
        printf("Nome: ");
        scanf("%s", novo->nome);
        
        printf("Escolha seu peao (ex: @, #, X): ");
        scanf(" %c", &novo->peao); // O espaço antes do %c é importante!

        novo->acertos = 0;
        novo->erros = 0;
        novo->prox = NULL;

        if (primeiro == NULL) {
            primeiro = novo;
            ultimo = novo;
        } else {
            ultimo->prox = novo;
            ultimo = novo;
        }
    }

    // Fecha o círculo da lista
    if (ultimo != NULL) {
        ultimo->prox = primeiro;
    }

    return primeiro;
}

#endif