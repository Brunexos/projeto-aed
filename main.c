#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "jogador.h"

#ifdef _WIN32
    #include <windows.h>
#endif

void configurarTerminal() {
#ifdef _WIN32
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD dwMode = 0;
    GetConsoleMode(hOut, &dwMode);
    dwMode |= 0x0004; // Habilita cores ANSI
    SetConsoleMode(hOut, dwMode);
#endif
}

int main(int argc, char *argv[]) {
    // Lógica para abrir em janela externa (CMD)
    if (argc == 1 || strcmp(argv[1], "jogo") != 0) {
        char comando[512];
        sprintf(comando, "start \"MEU_JOGO\" \"%s\" jogo", argv[0]);
        system(comando);
        return 0; 
    }

    configurarTerminal();
    srand(time(NULL));

    int opcao = 0;
    Jogador *jogador_da_vez = NULL;

    while (opcao != 3) {
        system("cls");
        printf("==========================================\n");
        printf("       JOGO DO CLUBE DE PROGRAMACAO       \n");
        printf("==========================================\n");
        printf(" 1. Iniciar Novo Jogo\n");
        printf(" 2. Ver Instrucoes\n");
        printf(" 3. Sair\n");
        printf("\n Escolha: ");
        
        if (scanf("%d", &opcao) != 1) {
            while (getchar() != '\n'); // Limpa o lixo do teclado
            continue;
        }

        switch (opcao) {
            case 1: {
                int qtd;
                do {
                    printf("\nQuantos jogadores (2 a 4)? ");
                    if (scanf("%d", &qtd) != 1) {
                        while (getchar() != '\n');
                        qtd = 0;
                    }
                } while(qtd < 2 || qtd > 4);

                // Chama a função que está no jogador.h
                jogador_da_vez = criar_lista_jogadores(qtd);
                
                printf("\n\x1b[32mSucesso!\x1b[0m O primeiro a jogar: %s [%c]\n", 
                        jogador_da_vez->nome, jogador_da_vez->peao);
                system("pause");
                break;
            }
            case 2:
                printf("\n REGRAS: Avance 30 casas e vença o desafio!\n");
                system("pause");
                break;
        }
    }

    return 0;
}