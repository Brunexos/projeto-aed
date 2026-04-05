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

    // Validação do número de jogadores
    while(qtd < 2 || qtd > 4) {
        system("cls");
        printf(RED "ERRO: Numero de jogadores invalido!\n" RESET);
        printf("Escolha um numero de 2 a 4: ");
        scanf("%d", &qtd);
    }

    // Limpa o buffer do teclado para o fgets não ler o '\n' do scanf
    while (getchar() != '\n'); 

    for (int i = 1; i <= qtd; i++) {
        tp_item novo;
        novo.id = i;
        novo.posicao = 0;

        printf("Digite o nome do jogador %d: ", i);
        fgets(novo.nome, 50, stdin);
        
        // Remove o '\n' que o fgets captura
        novo.nome[strcspn(novo.nome, "\n")] = 0;

        if (insereFila(f, novo)) {
            printf("-> [%s] entrou na mesa!\n\n", novo.nome);
        } else {
            printf("Erro: A mesa esta cheia!\n");
            break;
        }
    }
}