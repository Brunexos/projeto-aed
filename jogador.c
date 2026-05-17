#include "jogador.h"
#include "questoes.h"
#include "tabuleiro.h"
#include "visual.h"
#include "historico.h"

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
#define ESC 27
#define LARGURA_TERMINAL 160

void pos_jog(int x, int y) {
    printf("\033[%d;%dH", y, x);
}

static int centroXJog(int larguraTexto) {
    return (LARGURA_TERMINAL - larguraTexto) / 2;
}

static void textoCentroJog(int y, const char *texto) {
    pos_jog(centroXJog(strlen(texto)), y);
    printf("%s", texto);
}

static void textoCentroJogCor(int y, const char *cor, const char *texto) {
    pos_jog(centroXJog(strlen(texto)), y);
    printf("%s%s%s", cor, texto, RESET);
}

static void limparTela() {
    system("cls");
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

static void mostrarRegrasTela() {
    limparTela();

    textoCentroJogCor(4, YELLOW, "================ REGRAS DO JOGO ================");

    textoCentroJog(7, "- O jogador joga o dado e anda pelo tabuleiro.");
    textoCentroJog(9, "- Casa NORMAL: nada acontece.");
    textoCentroJog(11, "- Casa PRISAO: perde a proxima rodada.");
    textoCentroJog(13, "- Casa PERGUNTA: o jogador escolhe a dificuldade.");
    textoCentroJog(15, "- FACIL: acertou anda 2, errou volta 1.");
    textoCentroJog(17, "- MEDIO: acertou anda 3, errou volta 2.");
    textoCentroJog(19, "- DIFICIL: acertou anda 4, errou volta 3.");
    textoCentroJog(21, "- Vence quem chegar na casa 30.");

    textoCentroJogCor(25, CYAN, "Pressione qualquer tecla para voltar...");

    _getch();
}

static void mostrarHistoricoTela(Historico *historico) {
    limparTela();

    textoCentroJogCor(4, CYAN, "============== HISTORICO DA PARTIDA ==============");

    if (historico == NULL || historico->qtd == 0) {
        textoCentroJog(8, "Nenhum evento registrado ainda.");
    } else {
        for (int i = 0; i < historico->qtd; i++) {
            char linha[200];
            sprintf(linha, "- %s", historico->logs[i]);
            textoCentroJog(8 + i * 2, linha);
        }
    }

    textoCentroJogCor(25, CYAN, "Pressione qualquer tecla para voltar...");

    _getch();
}

static int menuAcaoRodada(
    Casa *inicioTabuleiro,
    tp_fila *fila,
    tp_item *jogador,
    Historico *historico
) {
    int selecionado = 1;
    int tecla = 0;

    while (1) {
        limparTela();

        desenharTelaJogo(
            inicioTabuleiro,
            fila,
            jogador,
            historico,
            "Escolha uma acao para esta rodada",
            0
        );

        int xMenu = centroXJog(50);

        pos_jog(xMenu, 34);
        printf(CYAN "+---------------- MENU DA RODADA ----------------+" RESET);

        pos_jog(xMenu, 36);
        printf("| %s 1. Rolar dado" RESET "                              |",
            selecionado == 1 ? GREEN " >" : "  "
        );

        pos_jog(xMenu, 37);
        printf("| %s 2. Ver regras" RESET "                              |",
            selecionado == 2 ? YELLOW " >" : "  "
        );

        pos_jog(xMenu, 38);
        printf("| %s 3. Ver historico" RESET "                           |",
            selecionado == 3 ? CYAN " >" : "  "
        );

        pos_jog(xMenu, 39);
        printf(CYAN "+------------------------------------------------+" RESET);

        textoCentroJogCor(41, WHITE, "Use as setas e pressione ENTER");

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
                return 1;
            }
            else if (selecionado == 2) {
                mostrarRegrasTela();
            }
            else if (selecionado == 3) {
                mostrarHistoricoTela(historico);
            }
        }
    }
}

static NivelPergunta escolherDificuldadePergunta() {
    int selecionado = 1;
    int tecla = 0;

    while (1) {
        limparTela();

        textoCentroJogCor(5, YELLOW, "--- ESCOLHA A DIFICULDADE ---");

        int xOpcoes = centroXJog(20);

        pos_jog(xOpcoes, 8);
        printf("%s 1. FACIL   " RESET, (selecionado == 1 ? GREEN " >" : "  "));

        pos_jog(xOpcoes, 9);
        printf("%s 2. MEDIO   " RESET, (selecionado == 2 ? YELLOW " >" : "  "));

        pos_jog(xOpcoes, 10);
        printf("%s 3. DIFICIL " RESET, (selecionado == 3 ? RED " >" : "  "));

        textoCentroJogCor(13, WHITE, "Use as setas e pressione ENTER");

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

static const char* nomeNivel(NivelPergunta nivel) {
    if (nivel == FACIL) {
        return "FACIL";
    }

    if (nivel == MEDIO) {
        return "MEDIO";
    }

    return "DIFICIL";
}

static Casa* animarMovimento(
    Casa *inicioTabuleiro,
    tp_fila *fila,
    tp_item *jogador,
    Historico *historico,
    int passos,
    const char *statusBase
) {
    int direcao = passos >= 0 ? 1 : -1;
    int total = passos >= 0 ? passos : -passos;

    char status[100];

    for (int i = 1; i <= total; i++) {
        jogador->casaAtual = moverCasas(jogador->casaAtual, direcao);

        sprintf(status, "%s Passo %d de %d", statusBase, i, total);

        limparTela();

        desenharTelaJogo(
            inicioTabuleiro,
            fila,
            jogador,
            historico,
            status,
            0
        );

        Sleep(320);
    }

    return jogador->casaAtual;
}

static void telaVitoria(Casa *inicioTabuleiro, tp_fila *fila, tp_item *vencedor, Historico *historico) {
    limparTela();

    desenharTelaJogo(
        inicioTabuleiro,
        fila,
        vencedor,
        historico,
        "Fim de jogo",
        0
    );

    textoCentroJogCor(34, GREEN, "============================================================");
    textoCentroJogCor(35, GREEN, "                         VITORIA!                           ");
    textoCentroJogCor(36, GREEN, "============================================================");

    char linha[150];

    sprintf(linha, "Vencedor: %s - Casa %d", vencedor->nome, vencedor->casaAtual->id);
    textoCentroJog(38, linha);

    textoCentroJogCor(40, CYAN, "Posicoes finais:");

    sprintf(linha, "1. %s - Casa %d", vencedor->nome, vencedor->casaAtual->id);
    textoCentroJog(42, linha);

    int linhaY = 43;
    int posicao = 2;
    int idx = fila->ini;

    while (idx != fila->fim) {
        idx = (idx == MAXF - 1) ? 0 : idx + 1;

        tp_item jogador = fila->item[idx];

        if (jogador.casaAtual != NULL) {
            sprintf(linha, "%d. %s - Casa %d", posicao, jogador.nome, jogador.casaAtual->id);
        } else {
            sprintf(linha, "%d. %s - Casa --", posicao, jogador.nome);
        }

        textoCentroJog(linhaY, linha);

        linhaY++;
        posicao++;
    }

    textoCentroJogCor(linhaY + 2, YELLOW, "Pressione qualquer tecla para voltar ao menu...");

    _getch();
}

void cadastrarJogadores(tp_fila *f, Casa *inicioTabuleiro) {
    int qtd = 2;
    int tecla;
    int selecionado = 2;

    while (1) {
        limparTela();

        textoCentroJogCor(5, YELLOW, "--- QUANTIDADE DE JOGADORES ---");

        int xOpcoes = centroXJog(20);

        pos_jog(xOpcoes, 8);
        printf("%s 2 Jogadores" RESET, (selecionado == 2 ? CYAN " >" : "  "));

        pos_jog(xOpcoes, 9);
        printf("%s 3 Jogadores" RESET, (selecionado == 3 ? CYAN " >" : "  "));

        pos_jog(xOpcoes, 10);
        printf("%s 4 Jogadores" RESET, (selecionado == 4 ? CYAN " >" : "  "));

        textoCentroJogCor(13, WHITE, "Use as setas e pressione ENTER");

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

        limparTela();

        char tituloCadastro[80];

        sprintf(tituloCadastro, "--- CADASTRO JOGADOR %d ---", i);
        textoCentroJogCor(5, YELLOW, tituloCadastro);

        pos_jog(centroXJog(30), 8);
        printf("Digite o nome: ");

        printf("\033[?25h");
        fgets(novo.nome, 50, stdin);
        novo.nome[strcspn(novo.nome, "\n")] = 0;
        printf("\033[?25l");

        int cor_idx = 0;

        while (1) {
            limparTela();

            char tituloCor[100];

            sprintf(tituloCor, "--- ESCOLHA SUA COR, %s ---", novo.nome);
            textoCentroJogCor(5, YELLOW, tituloCor);

            int xCores = centroXJog(20);

            for (int c = 0; c < numCores; c++) {
                pos_jog(xCores, 8 + c);

                if (cor_idx == c) {
                    printf(CYAN " > " RESET);
                } else {
                    printf("   ");
                }

                printf("%s%s" RESET, codigoCorJogador(coresDisponiveis[c]), nomeCorJogador(coresDisponiveis[c]));
            }

            textoCentroJogCor(15, WHITE, "Use as setas e pressione ENTER");

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
            limparTela();

            pos_jog(centroXJog(50), 10);
            printf(GREEN "[+] " RESET);
            printf("%s%s" RESET, codigoCorJogador(novo.cor), novo.nome);
            printf(GREEN " entrou com a cor %s!" RESET, nomeCorJogador(novo.cor));

            textoCentroJog(12, "Pressione qualquer tecla para continuar...");

            _getch();
        } else {
            textoCentroJogCor(10, RED, "Erro: A mesa esta cheia!");
            _getch();
            break;
        }
    }
}

int realizarJogada(tp_fila *f, Casa *inicioTabuleiro, Historico *historico) {
    tp_item j;
    char log[150];

    if (!removeFila(f, &j)) {
        return 0;
    }

    limparTela();

    desenharTelaJogo(
        inicioTabuleiro,
        f,
        &j,
        historico,
        "Inicio da rodada",
        0
    );

    if (j.preso == 1) {
        sprintf(log, "%s perdeu a rodada porque estava preso", j.nome);
        adicionarLog(historico, log);

        limparTela();

        desenharTelaJogo(
            inicioTabuleiro,
            f,
            &j,
            historico,
            "Jogador preso: perdeu a rodada",
            0
        );

        j.preso = 0;
        insereFila(f, j);

        textoCentroJogCor(40, RED, "O jogador esta preso e perdeu esta rodada!");
        textoCentroJog(42, "Pressione qualquer tecla para continuar...");

        _getch();

        return 0;
    }

    menuAcaoRodada(inicioTabuleiro, f, &j, historico);

    limparTela();

    desenharTelaJogo(
        inicioTabuleiro,
        f,
        &j,
        historico,
        "Rolando o dado",
        0
    );

    int dado = animarRolagemDado(92, 34);

    sprintf(log, "%s tirou %d no dado", j.nome, dado);
    adicionarLog(historico, log);

    j.casaAtual = animarMovimento(
        inicioTabuleiro,
        f,
        &j,
        historico,
        dado,
        "Avancando no tabuleiro."
    );

    sprintf(log, "%s foi para a casa %d", j.nome, j.casaAtual->id);
    adicionarLog(historico, log);

    limparTela();

    desenharTelaJogo(
        inicioTabuleiro,
        f,
        &j,
        historico,
        "Movimento finalizado",
        dado
    );

    if (j.casaAtual->tipo == PRISAO) {
        sprintf(log, "%s caiu na prisao", j.nome);
        adicionarLog(historico, log);

        j.preso = 1;

        limparTela();

        desenharTelaJogo(
            inicioTabuleiro,
            f,
            &j,
            historico,
            "Caiu na prisao: perde a proxima rodada",
            dado
        );

        textoCentroJogCor(40, RED, "Voce caiu na PRISAO! Vai perder a proxima rodada.");
        textoCentroJog(42, "Pressione qualquer tecla para continuar...");

        _getch();
    }
    else if (j.casaAtual->tipo == PERGUNTA) {
        int casasAcerto = 0;
        int casasErro = 0;

        sprintf(log, "%s caiu em uma casa de pergunta", j.nome);
        adicionarLog(historico, log);

        limparTela();

        desenharTelaJogo(
            inicioTabuleiro,
            f,
            &j,
            historico,
            "Casa de pergunta: escolha a dificuldade",
            dado
        );

        textoCentroJogCor(40, YELLOW, "Voce caiu em uma casa de pergunta!");
        textoCentroJog(42, "Pressione qualquer tecla para escolher a dificuldade...");

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

        sprintf(log, "%s escolheu pergunta %s", j.nome, nomeNivel(nivelEscolhido));
        adicionarLog(historico, log);

        int acertou = responderPerguntaNivel(nivelEscolhido);

        if (acertou) {
            sprintf(log, "%s acertou e avancou %d casas", j.nome, casasAcerto);
            adicionarLog(historico, log);

            j.casaAtual = animarMovimento(
                inicioTabuleiro,
                f,
                &j,
                historico,
                casasAcerto,
                "Resposta correta."
            );

            limparTela();

            desenharTelaJogo(
                inicioTabuleiro,
                f,
                &j,
                historico,
                "Acertou a pergunta",
                dado
            );

            char msg[100];
            sprintf(msg, "Voce acertou! Avancou %d casas.", casasAcerto);
            textoCentroJogCor(40, GREEN, msg);
        } else {
            sprintf(log, "%s errou e voltou %d casas", j.nome, casasErro);
            adicionarLog(historico, log);

            j.casaAtual = animarMovimento(
                inicioTabuleiro,
                f,
                &j,
                historico,
                -casasErro,
                "Resposta errada."
            );

            limparTela();

            desenharTelaJogo(
                inicioTabuleiro,
                f,
                &j,
                historico,
                "Errou a pergunta",
                dado
            );

            char msg[100];
            sprintf(msg, "Voce errou! Voltou %d casas.", casasErro);
            textoCentroJogCor(40, RED, msg);
        }

        char novaCasa[100];
        sprintf(novaCasa, "Nova casa: %d - %s", j.casaAtual->id, j.casaAtual->nome);
        textoCentroJog(42, novaCasa);

        textoCentroJog(44, "Pressione qualquer tecla para continuar...");

        _getch();
    }

    if (j.casaAtual->id >= 30) {
        sprintf(log, "%s venceu o jogo", j.nome);
        adicionarLog(historico, log);

        telaVitoria(inicioTabuleiro, f, &j, historico);

        return 1;
    }

    insereFila(f, j);

    return 0;
}