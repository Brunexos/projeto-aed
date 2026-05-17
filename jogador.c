#include "jogador.h"
#include "questoes.h"
#include "tabuleiro.h"
#include "visual.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>

#ifdef _WIN32
    #include <windows.h>
#endif

#define RED     "\x1b[31m"
#define GREEN   "\x1b[32m"
#define YELLOW  "\x1b[33m"
#define BLUE    "\x1b[34m"
#define MAGENTA "\x1b[35m"
#define CYAN    "\x1b[36m"
#define WHITE   "\x1b[37m"
#define RESET   "\x1b[0m"

#define SETA_CIMA 72
#define SETA_BAIXO 80
#define ENTER 13

void pos_jog(int x, int y) {
    printf("\033[%d;%dH", y, x);
}

static const char* codigoCorJogador(CorJogador cor) {
    switch (cor) {
        case COR_VERDE:
            return GREEN;
        case COR_AZUL:
            return BLUE;
        case COR_CIANO:
            return CYAN;
        case COR_MAGENTA:
            return MAGENTA;
        case COR_BRANCO:
            return WHITE;
        default:
            return WHITE;
    }
}

static const char* nomeCorJogador(CorJogador cor) {
    switch (cor) {
        case COR_VERDE:
            return "Verde";
        case COR_AZUL:
            return "Azul";
        case COR_CIANO:
            return "Ciano";
        case COR_MAGENTA:
            return "Magenta";
        case COR_BRANCO:
            return "Branco";
        default:
            return "Branco";
    }
}

static void limparAreaDado(int x, int y) {
    for (int i = 0; i < 7; i++) {
        pos_jog(x, y + i);
        printf("                         ");
    }
}

static void desenharDado(int numero, int x, int y) {
    limparAreaDado(x, y);

    pos_jog(x, y);
    printf(CYAN "+-------+" RESET);

    if (numero == 1) {
        pos_jog(x, y + 1); printf(CYAN "|       |" RESET);
        pos_jog(x, y + 2); printf(CYAN "|   o   |" RESET);
        pos_jog(x, y + 3); printf(CYAN "|       |" RESET);
    }
    else if (numero == 2) {
        pos_jog(x, y + 1); printf(CYAN "| o     |" RESET);
        pos_jog(x, y + 2); printf(CYAN "|       |" RESET);
        pos_jog(x, y + 3); printf(CYAN "|     o |" RESET);
    }
    else if (numero == 3) {
        pos_jog(x, y + 1); printf(CYAN "| o     |" RESET);
        pos_jog(x, y + 2); printf(CYAN "|   o   |" RESET);
        pos_jog(x, y + 3); printf(CYAN "|     o |" RESET);
    }
    else if (numero == 4) {
        pos_jog(x, y + 1); printf(CYAN "| o   o |" RESET);
        pos_jog(x, y + 2); printf(CYAN "|       |" RESET);
        pos_jog(x, y + 3); printf(CYAN "| o   o |" RESET);
    }
    else if (numero == 5) {
        pos_jog(x, y + 1); printf(CYAN "| o   o |" RESET);
        pos_jog(x, y + 2); printf(CYAN "|   o   |" RESET);
        pos_jog(x, y + 3); printf(CYAN "| o   o |" RESET);
    }
    else if (numero == 6) {
        pos_jog(x, y + 1); printf(CYAN "| o   o |" RESET);
        pos_jog(x, y + 2); printf(CYAN "| o   o |" RESET);
        pos_jog(x, y + 3); printf(CYAN "| o   o |" RESET);
    }

    pos_jog(x, y + 4);
    printf(CYAN "+-------+" RESET);
}

static int animarRolagemDado(int x, int y) {
    int numero = 1;

    for (int i = 0; i < 18; i++) {
        numero = (rand() % 6) + 1;

        desenharDado(numero, x, y);

        pos_jog(x - 4, y + 6);
        printf(YELLOW "Rolando o dado..." RESET);

        Sleep(80 + i * 10);
    }

    numero = (rand() % 6) + 1;

    desenharDado(numero, x, y);

    pos_jog(x - 4, y + 6);
    printf(GREEN "Resultado final: %d" RESET, numero);

    Sleep(900);

    return numero;
}

static NivelPergunta escolherDificuldadePergunta() {
    int selecionado = 1;
    int tecla = 0;

    while (1) {
        system("cls");

        pos_jog(43, 5);
        printf(YELLOW "--- ESCOLHA A DIFICULDADE ---" RESET);

        pos_jog(43, 8);
        printf("%s 1. FACIL   " RESET, (selecionado == 1 ? GREEN " >" : "  "));

        pos_jog(43, 9);
        printf("%s 2. MEDIO   " RESET, (selecionado == 2 ? YELLOW " >" : "  "));

        pos_jog(43, 10);
        printf("%s 3. DIFICIL " RESET, (selecionado == 3 ? RED " >" : "  "));

        pos_jog(43, 13);
        printf(WHITE "Use as setas e pressione ENTER" RESET);

        tecla = _getch();

        if (tecla == 224 || tecla == 0) {
            tecla = _getch();

            if (tecla == SETA_CIMA) {
                selecionado--;

                if (selecionado < 1) {
                    selecionado = 3;
                }
            }
            else if (tecla == SETA_BAIXO) {
                selecionado++;

                if (selecionado > 3) {
                    selecionado = 1;
                }
            }
        }
        else if (tecla == ENTER) {
            if (selecionado == 1) {
                return FACIL;
            }
            else if (selecionado == 2) {
                return MEDIO;
            }
            else {
                return DIFICIL;
            }
        }
    }
}

void cadastrarJogadores(tp_fila *f, Casa *inicioTabuleiro) {
    int qtd = 2;
    int tecla;
    int selecionado = 2;

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

            if (tecla == SETA_CIMA) {
                selecionado = (selecionado <= 2) ? 4 : selecionado - 1;
            }
            else if (tecla == SETA_BAIXO) {
                selecionado = (selecionado >= 4) ? 2 : selecionado + 1;
            }
        }
        else if (tecla == ENTER) {
            qtd = selecionado;
            break;
        }
    }

    CorJogador coresDisponiveis[] = {
        COR_VERDE,
        COR_AZUL,
        COR_CIANO,
        COR_MAGENTA,
        COR_BRANCO
    };

    int numCores = 5;

    for (int i = 1; i <= qtd; i++) {
        tp_item novo;

        novo.id = i;
        novo.casaAtual = inicioTabuleiro;
        novo.preso = 0;

        system("cls");

        pos_jog(40, 5);
        printf(YELLOW "--- CADASTRO JOGADOR %d ---" RESET, i);

        pos_jog(40, 8);
        printf("Digite o nome: ");

        printf("\033[?25h");
        fgets(novo.nome, 50, stdin);
        novo.nome[strcspn(novo.nome, "\n")] = 0;
        printf("\033[?25l");

        int cor_idx = 0;

        while (1) {
            system("cls");

            pos_jog(40, 5);
            printf(YELLOW "--- ESCOLHA SUA COR, %s ---" RESET, novo.nome);

            for (int c = 0; c < numCores; c++) {
                pos_jog(45, 8 + c);

                if (cor_idx == c) {
                    printf(CYAN " > " RESET);
                } else {
                    printf("   ");
                }

                printf("%s%s" RESET, codigoCorJogador(coresDisponiveis[c]), nomeCorJogador(coresDisponiveis[c]));
            }

            tecla = _getch();

            if (tecla == 224 || tecla == 0) {
                tecla = _getch();

                if (tecla == SETA_CIMA) {
                    cor_idx = (cor_idx <= 0) ? numCores - 1 : cor_idx - 1;
                }
                else if (tecla == SETA_BAIXO) {
                    cor_idx = (cor_idx >= numCores - 1) ? 0 : cor_idx + 1;
                }
            }
            else if (tecla == ENTER) {
                novo.cor = coresDisponiveis[cor_idx];

                for (int k = cor_idx; k < numCores - 1; k++) {
                    coresDisponiveis[k] = coresDisponiveis[k + 1];
                }

                numCores--;
                break;
            }
        }

        if (insereFila(f, novo)) {
            system("cls");

            pos_jog(40, 10);
            printf(GREEN "[+] " RESET);
            printf("%s%s" RESET, codigoCorJogador(novo.cor), novo.nome);
            printf(GREEN " entrou com a cor %s!" RESET, nomeCorJogador(novo.cor));

            pos_jog(40, 12);
            printf("Pressione qualquer tecla para continuar...");

            _getch();
        } else {
            printf("Erro: A mesa esta cheia!\n");
            break;
        }
    }
}

int realizarJogada(tp_fila *f, Casa *inicioTabuleiro) {
    tp_item j;

    if (!removeFila(f, &j)) {
        return 0;
    }

    system("cls");
    desenharTabuleiro(inicioTabuleiro, f, &j);

    if (j.preso == 1) {
        pos_jog(40, 22);
        printf(RED "%s esta preso e perdeu esta rodada!" RESET, j.nome);

        j.preso = 0;
        insereFila(f, j);

        pos_jog(40, 24);
        printf("Pressione qualquer tecla para continuar...");
        _getch();

        return 0;
    }

    pos_jog(40, 22);
    printf(YELLOW ">>> VEZ DE: " RESET);
    printf("%s%s" RESET, codigoCorJogador(j.cor), j.nome);

    pos_jog(40, 24);
    printf("Casa atual: %d - %s", j.casaAtual->id, j.casaAtual->nome);

    pos_jog(40, 26);
    printf("Pressione ENTER para girar o dado...");

    while (_getch() != ENTER);

    int dado = animarRolagemDado(92, 22);

    j.casaAtual = moverCasas(j.casaAtual, dado);

    system("cls");
    desenharTabuleiro(inicioTabuleiro, f, &j);

    pos_jog(40, 22);
    printf(YELLOW ">>> VEZ DE: " RESET);
    printf("%s%s" RESET, codigoCorJogador(j.cor), j.nome);

    pos_jog(40, 24);
    printf(CYAN "Dado: %d" RESET, dado);

    pos_jog(40, 26);
    printf("O jogador avancou para a casa %d - %s", j.casaAtual->id, j.casaAtual->nome);

    pos_jog(40, 28);
    printf("---------------------------------------");

    if (j.casaAtual->tipo == PRISAO) {
        pos_jog(40, 30);
        printf(RED "Voce caiu na PRISAO! Vai perder a proxima rodada." RESET);

        j.preso = 1;

        pos_jog(40, 32);
        printf("Pressione qualquer tecla para continuar...");
        _getch();
    }
    else if (j.casaAtual->tipo == PERGUNTA) {
        int casasAcerto = 0;
        int casasErro = 0;

        pos_jog(40, 30);
        printf(YELLOW "Voce caiu em uma casa de pergunta!" RESET);

        pos_jog(40, 32);
        printf("Pressione qualquer tecla para escolher a dificuldade...");
        _getch();

        NivelPergunta nivelEscolhido = escolherDificuldadePergunta();

        if (nivelEscolhido == FACIL) {
            casasAcerto = 2;
            casasErro = 1;
        }
        else if (nivelEscolhido == MEDIO) {
            casasAcerto = 3;
            casasErro = 2;
        }
        else if (nivelEscolhido == DIFICIL) {
            casasAcerto = 4;
            casasErro = 3;
        }

        int acertou = responderPerguntaNivel(nivelEscolhido);

        if (acertou) {
            j.casaAtual = moverCasas(j.casaAtual, casasAcerto);

            system("cls");
            desenharTabuleiro(inicioTabuleiro, f, &j);

            pos_jog(35, 22);
            printf(GREEN "Voce acertou! Avancou %d casas." RESET, casasAcerto);
        } else {
            j.casaAtual = moverCasas(j.casaAtual, -casasErro);

            system("cls");
            desenharTabuleiro(inicioTabuleiro, f, &j);

            pos_jog(35, 22);
            printf(RED "Voce errou! Voltou %d casas." RESET, casasErro);
        }

        pos_jog(35, 24);
        printf("Nova casa: %d - %s", j.casaAtual->id, j.casaAtual->nome);

        pos_jog(35, 26);
        printf("Pressione qualquer tecla para continuar...");
        _getch();
    }

    if (j.casaAtual->id >= 30) {
        system("cls");
        desenharTabuleiro(inicioTabuleiro, f, &j);

        pos_jog(35, 22);
        printf(GREEN "==========================================" RESET);

        pos_jog(35, 23);
        printf(GREEN "   PARABENS! " RESET);
        printf("%s%s" RESET, codigoCorJogador(j.cor), j.nome);
        printf(GREEN " VENCEU O JOGO!   " RESET);

        pos_jog(35, 24);
        printf(GREEN "==========================================" RESET);

        return 1;
    }

    insereFila(f, j);
    return 0;
}