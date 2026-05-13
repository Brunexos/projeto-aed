//comando pra dar start gcc *.c -o jogo.exe ; .\jogo.exe

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
#define SETA_CIMA 72 //definição de teclas
#define SETA_BAIXO 80 //definição de teclas
#define ENTER 13 //definição de teclas (quando apertar enter, vai selecionar a aba)

// Função para mover o cursor para X (coluna) e Y (linha) // pra levar os  prints pro meio da tela...
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

// Banner JOGO DO SUSA com alinhamento interno para centralizar
void desenharBanner(int x, int y) {
    // Alinhamento dos espaços do próprio banner para compensar o tamanho
    irPara(x, y);     printf(CYAN  "      d88b  .d88b.   d888b   .d88b.      d8888b.  .d88b.      .d8888. db    db .d8888.  .d8b.  " RESET);
    irPara(x, y + 1); printf(CYAN  "     `8P' .8P  Y8. 88' Y8b .8P  Y8.     88  `8D .8P  Y8.     88'  YP 88    88 88'  YP d8' `8b " RESET);
    irPara(x, y + 2); printf(BLUE  "      88  88    88 88      88    88     88   88 88    88     `8bo.   88    88 `8bo.   88ooo88 " RESET);
    irPara(x, y + 3); printf(BLUE  "      88  88    88 88  ooo 88    88     88   88 88    88       `Y8b. 88    88   `Y8b. 88~~~88 " RESET);
    irPara(x, y + 4); printf(WHITE "  db. 88  `8b  d8' 88. ~8~ `8b  d8'     88  .8D `8b  d8'     db   8D 88b  d88 db   8D 88   88 " RESET);
    irPara(x, y + 5); printf(WHITE "  Y8888P   `Y88P'   Y888P   `Y88P'      Y8888D'  `Y88P'      `8888Y' ~Y8888P' `8888Y' YP   YP " RESET);
}

// Nova função de apresentação (Tela de Carregamento)
void telaApresentacao() {
    // Tela 1: Nome da Equipe
    system("cls");
    irPara(50, 5); 
    printf(CYAN "APRESENTADO POR:" RESET);
    irPara(50, 7);
    printf(WHITE "=== ARSENAI ===" RESET);
    Sleep(2500); // Pausa por 2.5 segundos

    // Tela 2: Créditos e Carregamento
    system("cls");
    irPara(51, 5);
    printf(YELLOW "DESENVOLVIDO POR:" RESET);
    
    // Nomes dos criadores (substitua pelos reais)
    irPara(53, 7); printf(WHITE "- Bruno Barbosa" RESET);
    irPara(53, 8); printf(WHITE "- Rafael Prado" RESET);
    irPara(53, 9); printf(WHITE "- Felipe Cardim" RESET);
    irPara(53, 10); printf(WHITE "- Nicolas Jezler" RESET);
    irPara(53, 11); printf(WHITE "- Samuel" RESET);

    // Brincadeira da barra de carregamento "falsa"
    irPara(50, 23);
    printf("Carregando o arsenal...");
    
    irPara(45, 25);
    printf("[");
    irPara(75, 25);
    printf("]");
    
    irPara(46, 25);
    for (int i = 0; i < 29; i++) {
        printf(GREEN "#" RESET); 
        Sleep(100); 
    }
    
    Sleep(800); 
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

    // Chamada da tela de apresentação que criamos
    telaApresentacao();

    tp_fila mesa;
    int selecionado = 1;
    int tecla = 0;
    int rodando = 1;

    // Definições de posição do menu para centralizar em uma tela de 120 colunas
    // O banner tem 62 caracteres de largura. (120 - 62) / 2 = 29 colunas de recuo.
    int posX_centro = 13; // Ajustado para centralizar o banner de 62 caracteres
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
            if (tecla == SETA_CIMA) { //se apertar a tecla cima, o selecionado vai diminuir, o que ocasiona a escolha por setas
                selecionado--;
                if (selecionado < 1) selecionado = 4;
            } else if (tecla == SETA_BAIXO) { //se apertar a tecla baixo, o selecionado vai aumentar, o que ocasiona a escolha por setas
                selecionado++;
                if (selecionado > 4) selecionado = 1;
            }
        } 
        else if (tecla == ENTER) { // ja esta escolhido a opção no menu
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