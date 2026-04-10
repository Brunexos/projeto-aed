#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <conio.h>
#include "jogador.h"
#include "questoes.h"

#ifdef _WIN32
    #include <windows.h>
#endif

// Definições de cores ANSI
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

// Função para mover o cursor para X (coluna) e Y (linha)
void irPara(int x, int y) {
    printf("\033[%d;%dH", y, x);
}

void configurarTerminal() {
#ifdef _WIN32
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD dwMode = 0;
    GetConsoleMode(hOut, &dwMode);
    dwMode |= 0x0004; // Habilita processamento de sequências ANSI
    SetConsoleMode(hOut, dwMode);
#endif
    printf("\033[?25l"); // Esconde o cursor do terminal
}

// Banner ARSENAI com alinhamento interno para centralizar
void desenharBanner(int x, int y) {
    // Alinhamento dos espaços do próprio banner para compensar o tamanho
    irPara(x, y);     printf(CYAN  "      db       d8888b. .d8888. d88888b d8b   db   d888b  " RESET);
    irPara(x, y + 1); printf(CYAN  "     d88b     88  `8D 88'  YP 88'     888o  88 88' Y8b " RESET);
    irPara(x, y + 2); printf(BLUE  "    d8'`8b    88oobY' `8bo.   88ooooo 88V8o 88 88      " RESET);
    irPara(x, y + 3); printf(BLUE  "   d888888b   88`8b     `Y8b. 88~~~~~ 88 V8o88 88  ooo " RESET);
    irPara(x, y + 4); printf(WHITE "  d8'    `8b  88 `88. db   8D 88.     88  V888 88. ~8~ " RESET);
    irPara(x, y + 5); printf(WHITE " dP      YP  88   YD `8888Y' Y88888P VP   V8P  Y888P   " RESET);
}

int main(int argc, char *argv[]) {
    // Para resolver o "começo do zero" da centralização, vamos definir o tamanho do terminal
    // e o centro dele. Para telas grandes, um "centro" genérico de 60 colunas ajuda.
    
    if (argc == 1 || strcmp(argv[1], "jogo") != 0) {
        // No Windows, podemos forçar o tamanho da janela antes de iniciar
        // Isso ajuda a ter um centro consistente.
        // O comando 'mode con cols=120 lines=40' define o tamanho da janela.
        char comando[512];
        sprintf(comando, "mode con cols=120 lines=40 && start \"MEU_JOGO\" \"%s\" jogo", argv[0]);
        system(comando);
        return 0; 
    }

    configurarTerminal();
    srand(time(NULL));

    tp_fila mesa;
    int selecionado = 1;
    int tecla = 0;
    int rodando = 1;

    // Definições de posição do menu para centralizar em uma tela de 120 colunas
    // O banner tem 62 caracteres de largura. (120 - 62) / 2 = 29 colunas de recuo.
    int posX_centro = 30; // Ajustado para centralizar o banner de 62 caracteres
    int posY_centro = 3; 

    // O menu de navegação tem 42 caracteres de largura. (120 - 42) / 2 = 39 colunas de recuo.
    int posX_menu = 39; // Ajustado para centralizar o menu de 42 caracteres
    int posY_menu_offset = 8; // Offset vertical a partir do banner

    while (rodando) {
        system("cls");
        
        // Desenha o banner centralizado
        desenharBanner(posX_centro, posY_centro);

        // Desenha a moldura do menu centralizada
        irPara(posX_menu, posY_centro + posY_menu_offset);
        printf("+------------------------------------------+");
        irPara(posX_menu, posY_centro + posY_menu_offset + 1);
        printf("|       MENU DE NAVEGACAO - ARSENAI        |");
        irPara(posX_menu, posY_centro + posY_menu_offset + 2);
        printf("+------------------------------------------+");

        // Opções do Menu
        irPara(posX_menu, posY_centro + posY_menu_offset + 4);
        printf("| %s 1. Iniciar Novo Jogo" RESET "          |", (selecionado == 1 ? GREEN " >" : "  "));
        
        irPara(posX_menu, posY_centro + posY_menu_offset + 5);
        printf("| %s 2. Ver Instrucoes" RESET "             |", (selecionado == 2 ? YELLOW " >" : "  "));
        
        irPara(posX_menu, posY_centro + posY_menu_offset + 6);
        printf("| %s 3. Sair" RESET "                       |", (selecionado == 3 ? RED " >" : "  "));

        irPara(posX_menu, posY_centro + posY_menu_offset + 7);
        printf("| %s 4. Teste de Questoes" RESET "          |", (selecionado == 4 ? CYAN " >" : "  "));

        irPara(posX_menu, posY_centro + posY_menu_offset + 9);
        printf("+------------------------------------------+");
        
        irPara(posX_menu + 7, posY_centro + posY_menu_offset + 11);
        printf(WHITE "Use as setas para subir/descer" RESET);

        tecla = _getch();

        if (tecla == 0 || tecla == 224) {
            tecla = _getch();
            if (tecla == SETA_CIMA) {
                selecionado--;
                if (selecionado < 1) selecionado = 4;
            } else if (tecla == SETA_BAIXO) {
                selecionado++;
                if (selecionado > 4) selecionado = 1;
            }
        } 
        else if (tecla == ENTER) {
            irPara(1, posY_centro + posY_menu_offset + 14); 
            
            switch (selecionado) {
                case 1:
                    printf("\033[?25h"); 
                    inicializaFila(&mesa);
                    cadastrarJogadores(&mesa); 
                    printf("\033[?25l"); 
                    
                    if (!filaVazia(&mesa)) {
                        int fimDeJogo = 0;
                        while (fimDeJogo == 0) {
                            system("cls");
                            fimDeJogo = realizarJogada(&mesa);
                            if (fimDeJogo == 0) {
                                printf("\nPressione qualquer tecla para a proxima jogada...");
                                _getch();
                            }
                        }
                        printf("\nRetornando ao menu principal...\n");
                        system("pause");
                    }
                    break;

                case 2:
                    printf(YELLOW "\n REGRAS: Avance 30 casas e venca o desafio!\n" RESET);
                    system("pause");
                    break;

                case 3:
                    printf("\nSaindo...\n");
                    rodando = 0;
                    break;

                case 4:
                    QuestoesJogo();
                    system("pause");
                    break;
            }
        }
    }

    printf("\033[?25h"); 
    return 0;
}