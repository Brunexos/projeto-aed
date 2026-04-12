#include "jogador.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>

#ifdef _WIN32
    #include <windows.h>
#endif

// Cores
#define RED     "\x1b[31m"
#define GREEN   "\x1b[32m"
#define YELLOW  "\x1b[33m"
#define BLUE    "\x1b[34m"
#define CYAN    "\x1b[36m"
#define WHITE   "\x1b[37m"
#define RESET   "\x1b[0m"

// Teclas
#define SETA_CIMA 72
#define SETA_BAIXO 80
#define ENTER 13

// Função local para posicionar os textos deste módulo
void pos_jog(int x, int y) {
    printf("\033[%d;%dH", y, x);
}

void cadastrarJogadores(tp_fila *f) {
    int qtd = 2; // Padrão
    int tecla;
    int selecionado = 2; // Começa selecionando 2 jogadores

    // ---------------------------------------------------------
    // 1. TELA DE ESCOLHA DE QUANTIDADE DE JOGADORES (COM SETAS)
    // ---------------------------------------------------------
    while (1) {
        system("cls");
        pos_jog(45, 5);
        printf(YELLOW "--- QUANTIDADE DE JOGADORES ---" RESET);

        pos_jog(48, 8);
        printf("%s 2 Jogadores" RESET, (selecionado == 2 ? CYAN " >" : "  "));
        pos_jog(48, 9);
        printf("%s 3 Jogadores" RESET, (selecionado == 3 ? CYAN " >" : "  "));
        pos_jog(48, 10);
        printf("%s 4 Jogadores" RESET, (selecionado == 4 ? CYAN " >" : "  "));

        tecla = _getch();
        if (tecla == 224 || tecla == 0) {
            tecla = _getch();
            if (tecla == SETA_CIMA) selecionado = (selecionado <= 2) ? 4 : selecionado - 1;
            else if (tecla == SETA_BAIXO) selecionado = (selecionado >= 4) ? 2 : selecionado + 1;
        } else if (tecla == ENTER) {
            qtd = selecionado;
            break;
        }
    }

    // Array de peões disponíveis (pode adicionar mais se quiser)
    char peoes_disponiveis[] = {'@', '#', '&', '%', '$', '*'};
    int num_peoes = 6;

    // ---------------------------------------------------------
    // 2. CADASTRO DE NOME E ESCOLHA DE PEÃO PARA CADA JOGADOR
    // ---------------------------------------------------------
    for (int i = 1; i <= qtd; i++) {
        tp_item novo;
        novo.id = i;
        novo.posicao = 0;

        // Pegar o Nome
        system("cls");
        pos_jog(40, 5);
        printf(YELLOW "--- CADASTRO JOGADOR %d ---" RESET, i);
        pos_jog(40, 8);
        printf("Digite o nome: ");
        
        printf("\033[?25h"); // Mostra o cursor para o jogador digitar
        fgets(novo.nome, 50, stdin);
        novo.nome[strcspn(novo.nome, "\n")] = 0; // Remove o \n
        printf("\033[?25l"); // Esconde o cursor de novo

        // Escolher o Peão
        int peao_idx = 0;
        while (1) {
            system("cls");
            pos_jog(40, 5);
            printf(YELLOW "--- ESCOLHA SEU PEAO, %s ---" RESET, novo.nome);

            for (int p = 0; p < num_peoes; p++) {
                pos_jog(45, 8 + p);
                if (peao_idx == p)
                    printf(CYAN " > [ %c ]" RESET, peoes_disponiveis[p]);
                else
                    printf("   [ %c ]", peoes_disponiveis[p]);
            }

            tecla = _getch();
            if (tecla == 224 || tecla == 0) {
                tecla = _getch();
                if (tecla == SETA_CIMA) peao_idx = (peao_idx <= 0) ? num_peoes - 1 : peao_idx - 1;
                else if (tecla == SETA_BAIXO) peao_idx = (peao_idx >= num_peoes - 1) ? 0 : peao_idx + 1;
            } else if (tecla == ENTER) {
                novo.peao = peoes_disponiveis[peao_idx];

                // Remove o peão escolhido da lista para o próximo jogador não repetir
                for (int k = peao_idx; k < num_peoes - 1; k++) {
                    peoes_disponiveis[k] = peoes_disponiveis[k+1];
                }
                num_peoes--;
                break;
            }
        }

        // Insere na fila
        if (insereFila(f, novo)) {
            system("cls");
            pos_jog(40, 10);
            printf(GREEN "[+] %s entrou com o peao '%c'!" RESET, novo.nome, novo.peao);
            pos_jog(40, 12);
            printf("Pressione qualquer tecla para continuar...");
            _getch();
        } else {
            printf("Erro: A mesa esta cheia!\n");
            break;
        }
    }
}

int realizarJogada(tp_fila *f) { 
    tp_item j;
    removeFila(f, &j); // Tira o jogador da vez

    pos_jog(40, 5);
    printf(YELLOW ">>> VEZ DE: %s [%c] <<<" RESET, j.nome, j.peao);
    
    pos_jog(40, 7);
    printf("Posicao atual: %d", j.posicao);

    pos_jog(40, 9);
    printf("Pressione ENTER para girar o dado...");
    while(_getch() != ENTER); // Aguarda o jogador apertar ENTER para rolar o dado

    int dado = (rand() % 6) + 1;
    
    pos_jog(40, 11);
    printf(CYAN "Girando o dado... caiu em: %d!" RESET, dado);

    j.posicao += dado; // acrescentar o dado
    
    pos_jog(40, 13);
    printf("O peao [%c] avancou para a casa %d", j.peao, j.posicao);
    
    pos_jog(40, 15);
    printf("---------------------------------------");

    if (j.posicao >= 30) { // condição de vitoria
        system("cls");
        pos_jog(35, 10);
        printf(GREEN "==========================================" RESET);
        pos_jog(35, 11);
        printf(GREEN "   PARABENS! %s VENCEU O JOGO!   " RESET, j.nome);
        pos_jog(35, 12);
        printf(GREEN "==========================================" RESET);
        return 1; // fim de jogo
    }

    insereFila(f, j); // volta pro final da fila
    return 0; // proximo da fila
}