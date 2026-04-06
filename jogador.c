#include "jogador.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Definições de cores restritas ao funcionamento interno deste módulo
#define RED     "\x1b[31m"
#define RESET   "\x1b[0m"

void cadastrarJogadores(tp_fila *f) {
    int qtd;
    
    printf("Quantos jogadores vao participar? ");
    scanf("%d", &qtd);
    system("cls");

    while(qtd < 2 || qtd > 4) {
        system("cls");
        printf(RED "ERRO: Numero de jogadores invalido!\n" RESET);
        printf("Escolha um numero de 2 a 4: ");
        scanf("%d", &qtd);
    }

    while (getchar() != '\n'); 

    for (int i = 1; i <= qtd; i++) {
        tp_item novo;
        novo.id = i;
        novo.posicao = 0;

        printf("Digite o nome do jogador %d: ", i);
        fgets(novo.nome, 50, stdin);
        
        // Remove o '\n' que o fgets captura
        novo.nome[strcspn(novo.nome, "\n")] = 0;
        // Atribui a primeira letra do nome ao campo peao
        novo.peao = novo.nome[0]; 
        // -------------------

        if (insereFila(f, novo)) {
            printf("-> [%s] entrou na mesa com o peao '%c'!\n\n", novo.nome, novo.peao);
        } else {
            printf("Erro: A mesa esta cheia!\n");
            break;
        }
    }
}

int realizarJogada(tp_fila *f) { 
    tp_item j;
    removeFila(f, &j); // Tira o jogador da vez

    printf("\n>>> VEZ DE: %s [%c] <<<\n", j.nome, j.peao);
    printf("Posicao atual: %d\n", j.posicao);

    int dado = (rand() % 6) + 1;
    printf("Girando o dado... caiu em: %d!\n", dado);

    j.posicao += dado; //acrescentar o dado
    printf("O peao '%c' avancou para a casa %d\n", j.peao, j.posicao);
    printf("---------------------------------------\n");

    if (j.posicao >= 30) {//condição de vitoria
        printf("\n==========================================\n");
        printf("   PARABENS! %s VENCEU O JOGO!   \n", j.nome);
        printf("==========================================\n");
        return 1; //fim de jogo
    }

    insereFila(f, j);//
    return 0;// proximo da fila
}