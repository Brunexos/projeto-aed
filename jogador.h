#ifndef JOGADOR_H
#define JOGADOR_H

#include <stdio.h>
#include <string.h>
#include "fila.h"
#define RED     "\x1b[31m"
#define GREEN   "\x1b[32m"
#define YELLOW  "\x1b[33m"
#define RESET   "\x1b[0m"


void cadastrarJogadores(tp_fila *f) { //Bruno->cadastro usando FILA
    int qtd;
    printf("Quantos jogadores vao participar? ");
    scanf("%d", &qtd);
    system("cls");
    while(qtd<2 || qtd>4){//pro usuario nao escolher menor que 2 e maior que 4
       system("cls");
        printf(RED"ERRO ERRO ERRO ERRO ERRO ERRO\n" RESET);
        printf("escolha um numero de 2 a 4\n");
        scanf("%d", &qtd);
    }
    while (getchar() != '\n'); 

    for (int i = 1; i <= qtd; i++) {
        tp_item novo;
        novo.id = i;
        novo.posicao = 0;
        printf("Digite o nome do jogador %d: ", i);
        fgets(novo.nome, 50, stdin);
        novo.nome[strcspn(novo.nome, "\n")] = 0;

        if (insereFila(f, novo)) {
            printf("-> [%s] entrou na mesa!\n\n", novo.nome);
        } else {
            printf("Erro: A mesa esta cheia!\n");
            break;
        }
    }
}

#endif