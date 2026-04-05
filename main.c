#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "jogador.h"
#include "questoes.h"


#ifdef _WIN32
    #include <windows.h>
#endif

void configurarTerminal() { // NAO MEXE AQUI PELO AMOR DE DEUS
#ifdef _WIN32
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD dwMode = 0;
    GetConsoleMode(hOut, &dwMode);
    dwMode |= 0x0004;
    SetConsoleMode(hOut, dwMode);
#endif
}

int main(int argc, char *argv[]) {
    if (argc == 1 || strcmp(argv[1], "jogo") != 0) {
        char comando[512];
        sprintf(comando, "start \"MEU_JOGO\" \"%s\" jogo", argv[0]);
        system(comando);
        return 0; 
    }

    configurarTerminal();
    srand(time(NULL));

    tp_fila mesa;
    tp_item jogador_atual; 
    int opcao = 0;

    while (opcao != 3) {
        system("cls");//limpa o terminal
        printf("==========================================\n");
        printf("       JOGO DO CLUBE ARSENAI      \n");
        printf("==========================================\n");
        printf(GREEN " 1. Iniciar Novo Jogo (Cadastrar)\n" RESET);
        printf(YELLOW " 2. Ver Instrucoes\n" RESET);
        printf(RED" 3. Sair\n" RESET);
        printf(RED" 4. teste de questões\n" RESET);
        printf("\n Escolha: ");
        
        if (scanf("%d", &opcao) != 1) {
            while (getchar() != '\n');
            continue;
        }

        switch (opcao) {
            case 1:
                inicializaFila(&mesa);
                cadastrarJogadores(&mesa); 
                if (!filaVazia(&mesa)) {
                    removeFila(&mesa, &jogador_atual);
                    printf("\n\x1b[32mSucesso!\x1b[0m O primeiro a jogar: %s\n", jogador_atual.nome);
                    insereFila(&mesa, jogador_atual);
                }
                
                system("pause");
                break;

            case 2:
                printf(YELLOW "\n REGRAS: Avance 30 casas e vença o desafio!\n" RESET);
                system("pause");
                break;
            
            case 3:
                printf("\nSaindo...\n");
                break;


            case 4:
                QuestoesJogo();
                system("pause");
                break;
        }
    }

    return 0;
}