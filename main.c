#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "jogador.h"
#include "graficos.h"

#ifdef _WIN32
    #include <windows.h>
#endif

void configurarTerminal() {
#ifdef _WIN32
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD dwMode = 0;
    GetConsoleMode(hOut, &dwMode);
    dwMode |= 0x0004; 
    SetConsoleMode(hOut, dwMode);
#endif
}

int main(int argc, char *argv[]) {
    // Truque do "start" para garantir cores no Windows antigo
    if (argc == 1 || strcmp(argv[1], "jogo") != 0) {
        char comando[512];
        sprintf(comando, "start \"PROJETO_AED\" \"%s\" jogo", argv[0]);
        system(comando);
        return 0; 
    }

    configurarTerminal();
    srand(time(NULL));

    int opcao = 0;
    Jogador *jogador_da_vez = NULL;

    while (opcao != 3) {
        system("cls");
        printf("%s", RED); // Título em vermelho
        printf("==========================================\n");
        printf("       JOGO DO CLUBE DE PROGRAMACAO       \n");
        printf("==========================================\n" RESET);
        printf(" 1. Iniciar Novo Jogo\n");
        printf(" 2. Ver Instrucoes\n");
        printf(" 3. Sair\n");
        printf("\n Escolha: ");
        
        if (scanf("%d", &opcao) != 1) {
            while (getchar() != '\n');
            continue;
        }

        if (opcao == 1) {
            int qtd;
            do {
                printf("\nQuantos jogadores (2 a 4)? ");
                scanf("%d", &qtd);
            } while(qtd < 2 || qtd > 4);

            jogador_da_vez = criar_lista_jogadores(qtd);
            
            system("cls");
            printf("\n\x1b[32m--- JOGO INICIADO ---\x1b[0m\n");
            printf("O primeiro a jogar: %s%s%s [%c]\n", 
                   obterCor(jogador_da_vez->cor_escolhida), 
                   jogador_da_vez->nome, 
                   RESET, 
                   jogador_da_vez->icone);
            
            system("pause");
        }
    }

    return 0;
}