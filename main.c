// Comando para compilar e rodar:
// gcc main.c fila.c jogador.c pilha.c questoes.c tabuleiro.c visual.c historico.c listade.c arvore_casas.c ranking.c -o jogo.exe; if ($?) { .\jogo.exe }

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <conio.h>

#include "jogador.h"
#include "questoes.h"
#include "tabuleiro.h"
#include "visual.h"
#include "historico.h"
#include "ranking.h"

#ifdef _WIN32
    #include <windows.h>
#endif

#define RED     "\x1b[31m"
#define GREEN   "\x1b[32m"
#define YELLOW  "\x1b[33m" 
#define BLUE    "\x1b[34m"
#define CYAN    "\x1b[36m"
#define WHITE   "\x1b[37m"
#define RESET   "\x1b[0m"

#define SETA_CIMA 72
#define SETA_BAIXO 80
#define ENTER 13

#define LARGURA_TERMINAL 160
#define ALTURA_TERMINAL 55

void irPara(int x, int y) {
    printf("\033[%d;%dH", y, x);
}

int centroX(int larguraTexto) {
    return (LARGURA_TERMINAL - larguraTexto) / 2;
}

void imprimirCentralizado(int y, const char *texto) {
    int x = centroX(strlen(texto));
    irPara(x, y);
    printf("%s", texto);
}

void imprimirCentralizadoCor(int y, const char *cor, const char *texto) {
    int x = centroX(strlen(texto));
    irPara(x, y);
    printf("%s%s%s", cor, texto, RESET);
}

void configurarTerminal() {
#ifdef _WIN32
    system("mode con cols=160 lines=55");

    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD dwMode = 0;

    GetConsoleMode(hOut, &dwMode);

    dwMode |= 0x0004;

    SetConsoleMode(hOut, dwMode);
#endif

    printf("\033[?25l");
}

void desenharBanner(int x, int y) {
    irPara(x, y);
    printf(CYAN  "      d88b  .d88b.   d888b   .d88b.      d8888b.  .d88b.      .d8888. db    db .d8888.  .d8b.  " RESET);

    irPara(x, y + 1);
    printf(CYAN  "     `8P' .8P  Y8. 88' Y8b .8P  Y8.     88  `8D .8P  Y8.     88'  YP 88    88 88'  YP d8' `8b " RESET);

    irPara(x, y + 2);
    printf(BLUE  "      88  88    88 88      88    88     88   88 88    88     `8bo.   88    88 `8bo.   88ooo88 " RESET);

    irPara(x, y + 3);
    printf(BLUE  "      88  88    88 88  ooo 88    88     88   88 88    88       `Y8b. 88    88   `Y8b. 88~~~88 " RESET);

    irPara(x, y + 4);
    printf(WHITE "  db. 88  `8b  d8' 88. ~8~ `8b  d8'     88  .8D `8b  d8'     db   8D 88b  d88 db   8D 88   88 " RESET);

    irPara(x, y + 5);
    printf(WHITE "  Y8888P   `Y88P'   Y888P   `Y88P'      Y8888D'  `Y88P'      `8888Y' ~Y8888P' `8888Y' YP   YP " RESET);
}

void telaApresentacao() {
    system("cls");

    imprimirCentralizadoCor(7, CYAN, "APRESENTADO POR:");
    imprimirCentralizadoCor(9, WHITE, "=== ARSENAI ===");

    Sleep(2500);

    system("cls");

    imprimirCentralizadoCor(6, YELLOW, "DESENVOLVIDO POR:");

    imprimirCentralizadoCor(8, WHITE, "- Bruno Barbosa");
    imprimirCentralizadoCor(9, WHITE, "- Rafael Prado");
    imprimirCentralizadoCor(10, WHITE, "- Felipe Cardim");
    imprimirCentralizadoCor(11, WHITE, "- Nicolas Jezler");
    imprimirCentralizadoCor(12, WHITE, "- Samuel Dias");

    imprimirCentralizado(25, "Carregando o arsenal...");

    int larguraBarra = 31;
    int xBarra = centroX(larguraBarra);

    irPara(xBarra, 27);
    printf("[");

    irPara(xBarra + 30, 27);
    printf("]");

    irPara(xBarra + 1, 27);

    for (int i = 0; i < 29; i++) {
        printf(GREEN "#" RESET);
        Sleep(100);
    }

    Sleep(800);
}

void telaRegrasPrincipal() {
    system("cls");

    imprimirCentralizadoCor(6, YELLOW, "================ REGRAS DO JOGO ================");

    imprimirCentralizado(9,  "- O jogador joga o dado e anda pelo tabuleiro.");
    imprimirCentralizado(11, "- Casa NORMAL: nada acontece.");
    imprimirCentralizado(13, "- Casa PRISAO: perde a proxima rodada.");
    imprimirCentralizado(15, "- Casa de PERGUNTA: o jogador escolhe a dificuldade.");
    imprimirCentralizado(17, "- FACIL: acertou anda 2, errou volta 1.");
    imprimirCentralizado(19, "- MEDIO: acertou anda 3, errou volta 2.");
    imprimirCentralizado(21, "- DIFICIL: acertou anda 4, errou volta 3.");
    imprimirCentralizado(23, "- Vence quem chegar na casa 30.");
    imprimirCentralizado(25, "- No tabuleiro, o nome do jogador aparece na cor escolhida.");

    imprimirCentralizadoCor(30, CYAN, "Pressione qualquer tecla para voltar...");

    _getch();
}

void desenharMenuPrincipal(int selecionado) {
    int larguraBanner = 101;
    int xBanner = centroX(larguraBanner);
    int yBanner = 4;

    int larguraMenu = 44;
    int xMenu = centroX(larguraMenu);
    int yMenu = 17;

    desenharBanner(xBanner, yBanner);

    irPara(xMenu, yMenu);
    printf("+------------------------------------------+");

    irPara(xMenu, yMenu + 1);
    printf("|       MENU DE NAVEGACAO - ARSENAI        |");

    irPara(xMenu, yMenu + 2);
    printf("+------------------------------------------+");

    irPara(xMenu, yMenu + 4);
    printf("| %s 1. Iniciar Novo Jogo" RESET "          |", selecionado == 1 ? GREEN " >" : "  ");

    irPara(xMenu, yMenu + 5);
    printf("| %s 2. Ver Instrucoes" RESET "             |", selecionado == 2 ? YELLOW " >" : "  ");

    irPara(xMenu, yMenu + 6);
    printf("| %s 3. Sair" RESET "                       |", selecionado == 3 ? RED " >" : "  ");

    irPara(xMenu, yMenu + 7);
    printf("| %s 4. Ver Ranking Geral" RESET "          |", selecionado == 4 ? CYAN " >" : "  ");

    irPara(xMenu, yMenu + 9);
    printf("+------------------------------------------+");

    imprimirCentralizadoCor(yMenu + 12, WHITE, "Use as setas para subir/descer e ENTER para selecionar");
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

    telaApresentacao();

    tp_fila mesa;
    Historico historicoPartida;

    Casa *inicioTabuleiro = NULL;
    Casa *fimTabuleiro = NULL;

    criarTabuleiroPadrao(&inicioTabuleiro, &fimTabuleiro);
    salvarPerguntasCSV();

    int selecionado = 1;
    int tecla = 0;
    int rodando = 1;

    while (rodando) {
        system("cls");

        desenharMenuPrincipal(selecionado);

        tecla = _getch();

        if (tecla == 0 || tecla == 224) {
            tecla = _getch();

            if (tecla == SETA_CIMA) {
                selecionado--;

                if (selecionado < 1) {
                    selecionado = 4;
                }
            }
            else if (tecla == SETA_BAIXO) {
                selecionado++;

                if (selecionado > 4) {
                    selecionado = 1;
                }
            }
        }
        else if (tecla == ENTER) {
            switch (selecionado) {
                case 1:
                    printf("\033[?25h");

                    inicializaFila(&mesa);
                    inicializarHistorico(&historicoPartida);
                    inicializarHistoricoRespostasCSV();

                    NoCasa *arvoreQuedas = NULL;

                    adicionarLog(&historicoPartida, "Partida iniciada");

                    cadastrarJogadores(&mesa, inicioTabuleiro);

                    adicionarLog(&historicoPartida, "Jogadores cadastrados");

                    printf("\033[?25l");

                    if (!filaVazia(&mesa)) {
                        int fimDeJogo = 0;

                        while (fimDeJogo == 0) {
                            system("cls");

                            fimDeJogo = realizarJogada(&mesa, inicioTabuleiro, &historicoPartida, &arvoreQuedas);

                            if (fimDeJogo == 0) {
                                imprimirCentralizadoCor(52, WHITE, "Pressione qualquer tecla para a proxima jogada...");
                                _getch();
                            }
                        }
                    }

                    liberarArvoreCasas(arvoreQuedas);

                    break;

                case 2:
                    telaRegrasPrincipal();
                    break;

                case 3:
                    system("cls");
                    imprimirCentralizadoCor(20, RED, "Saindo...");
                    Sleep(700);
                    rodando = 0;
                    break;

                case 4:
                    system("cls");
                    printf("\033[?25h");
                    exibirRankingGeralSalvo();
                    imprimirCentralizadoCor(30, CYAN, "Pressione qualquer tecla para voltar...");
                    _getch();
                    printf("\033[?25l");
                    break;
            }
        }
    }

    liberarTabuleiro(inicioTabuleiro);

    printf("\033[?25h");

    return 0;
}
