#include "raylib.h"
// Compilar:
// gcc main_raylib.c fila.c pilha.c questoes.c tabuleiro.c historico.c listade.c arvore_casas.c ranking.c -o jogo_gui.exe -lraylib -lopengl32 -lgdi32 -lwinmm -lm

#include "arvore_casas.h"
#include "fila.h"
#include "historico.h"
#include "questoes.h"
#include "ranking.h"
#include "tabuleiro.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>

#define MAX_GUI_JOGADORES 4
#define CASA_FINAL 30
#define MAX_RANKING_GUI 12
#define MAX_TEMAS_GUI 12
#define TAB_X 38
#define TAB_Y 230
#define TAB_TAM 78
#define TAB_GAP 10

typedef enum {
    TELA_MENU,
    TELA_GRUPO,
    TELA_CADASTRO,
    TELA_JOGO,
    TELA_ESCOLHER_DIFICULDADE,
    TELA_PERGUNTA,
    TELA_RANKING,
    TELA_FIM
} TelaGUI;

typedef enum {
    MOVIMENTO_NENHUM,
    MOVIMENTO_DADO,
    MOVIMENTO_RESPOSTA
} TipoMovimentoGUI;

typedef struct {
    Rectangle area;
    const char *texto;
} BotaoGUI;

typedef struct {
    char nome[50];
    Casa *casaAtual;
    int posicaoAtual;
    int pontuacao;
    int acertos;
    int erros;
    int preso;
    Color cor;
} JogadorGUI;

typedef struct {
    char nome[50];
    int acertos;
    int erros;
    int pontos;
    int partidas;
} LinhaRankingGUI;

typedef struct {
    char tema[120];
    int acertos;
    int erros;
} TemaResumoGUI;

typedef struct {
    int dadoRolando;
    float dadoTempo;
    float dadoDuracao;
    float trocaDadoTempo;
    int dadoResultado;
    int dadoVisivel;

    int movimentoAtivo;
    int jogador;
    int direcao;
    int passosRestantes;
    int origemId;
    int destinoId;
    float movimentoTempo;
    float movimentoDuracao;
    TipoMovimentoGUI tipoMovimento;
} AnimacaoGUI;

static const Color COR_FUNDO = {22, 30, 38, 255};
static const Color COR_PAINEL = {32, 44, 55, 255};
static const Color COR_PAINEL_CLARO = {43, 59, 72, 255};
static const Color COR_TEXTO = {238, 243, 247, 255};
static const Color COR_TEXTO_FRACO = {170, 184, 195, 255};
static const Color COR_DESTAQUE = {63, 195, 182, 255};
static const Color COR_ALERTA = {238, 196, 94, 255};
static const Color COR_ERRO = {229, 94, 94, 255};

static Font fonteUI;
static int fonteUICarregada = 0;

static Font fonteAtiva() {
    return fonteUICarregada ? fonteUI : GetFontDefault();
}

static void TextoUI(const char *texto, int x, int y, int tamanho, Color cor) {
    DrawTextEx(fonteAtiva(), texto, (Vector2){(float)x, (float)y}, (float)tamanho, 1.0f, cor);
}

static int MedirTextoUI(const char *texto, int tamanho) {
    Vector2 medida = MeasureTextEx(fonteAtiva(), texto, (float)tamanho, 1.0f);
    return (int)medida.x;
}

#define DrawText(texto, x, y, tamanho, cor) TextoUI(texto, x, y, tamanho, cor)
#define MeasureText(texto, tamanho) MedirTextoUI(texto, tamanho)

typedef struct {
    char itens[6][160];
    int quantidade;
} HistoricoAcoesGUI;

static int botaoClicado(BotaoGUI botao) {
    Vector2 mouse = GetMousePosition();
    return CheckCollisionPointRec(mouse, botao.area) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON);
}

static void adicionarAcaoHistorico(HistoricoAcoesGUI *historico, const char *mensagem) {
    if (historico == NULL || mensagem == NULL) {
        return;
    }

    if (historico->quantidade < 6) {
        strncpy(historico->itens[historico->quantidade], mensagem, sizeof(historico->itens[historico->quantidade]) - 1);
        historico->itens[historico->quantidade][sizeof(historico->itens[historico->quantidade]) - 1] = '\0';
        historico->quantidade++;
    } else {
        for (int i = 0; i < 5; i++) {
            strcpy(historico->itens[i], historico->itens[i + 1]);
        }

        strncpy(historico->itens[5], mensagem, sizeof(historico->itens[5]) - 1);
        historico->itens[5][sizeof(historico->itens[5]) - 1] = '\0';
    }
}

static void desenharHistoricoAcoes(HistoricoAcoesGUI *historico, int x, int y) {
    DrawText("Historico de acoes", x, y, 24, COR_TEXTO);

    if (historico == NULL || historico->quantidade == 0) {
        DrawText("Aguardando a primeira acao.", x, y + 38, 17, COR_TEXTO_FRACO);
        return;
    }

    for (int i = 0; i < historico->quantidade; i++) {
        int posY = y + 38 + i * 28;
        Color cor = (i == historico->quantidade - 1) ? COR_TEXTO : COR_TEXTO_FRACO;
        DrawText(historico->itens[i], x, posY, 17, cor);
    }
}

static void desenharBotao(BotaoGUI botao, Color cor) {
    Vector2 mouse = GetMousePosition();
    int hover = CheckCollisionPointRec(mouse, botao.area);
    Color preenchimento = hover ? ColorBrightness(cor, 0.18f) : cor;

    DrawRectangleRounded(botao.area, 0.16f, 10, preenchimento);
    DrawRectangleRoundedLines(botao.area, 0.16f, 10, Fade(WHITE, 0.18f));

    int fonte = 22;
    int larguraTexto = MeasureText(botao.texto, fonte);
    DrawText(
        botao.texto,
        (int)(botao.area.x + (botao.area.width - larguraTexto) / 2),
        (int)(botao.area.y + (botao.area.height - fonte) / 2),
        fonte,
        COR_TEXTO
    );
}

static void desenharTitulo(const char *titulo, const char *subtitulo) {
    DrawText("JOGO DO SUSA", 40, 30, 42, COR_TEXTO);
    DrawText("ARSENAI", 43, 75, 20, COR_DESTAQUE);

    DrawText(titulo, 40, 125, 32, COR_TEXTO);

    if (subtitulo != NULL) {
        DrawText(subtitulo, 42, 165, 20, COR_TEXTO_FRACO);
    }
}

static void desenharTextoCentralizado(const char *texto, int y, int tamanho, Color cor) {
    int larguraTexto = MeasureText(texto, tamanho);
    DrawText(texto, (GetScreenWidth() - larguraTexto) / 2, y, tamanho, cor);
}

static Color corCasa(TipoCasa tipo) {
    if (tipo == PRISAO) {
        return COR_ERRO;
    }

    if (tipo == PERGUNTA) {
        return COR_ALERTA;
    }

    return (Color){92, 114, 132, 255};
}

static const char* nomeTipoCasa(TipoCasa tipo) {
    if (tipo == PRISAO) {
        return "Prisao";
    }

    if (tipo == PERGUNTA) {
        return "Pergunta";
    }

    return "Normal";
}

static void sincronizarPosicao(JogadorGUI *jogador) {
    if (jogador != NULL && jogador->casaAtual != NULL) {
        jogador->posicaoAtual = jogador->casaAtual->id;
    }
}

static int pontosPorNivelGUI(NivelPergunta nivel) {
    if (nivel == FACIL) {
        return 10;
    }

    if (nivel == MEDIO) {
        return 20;
    }

    return 30;
}

static int casasAcertoNivel(NivelPergunta nivel) {
    if (nivel == FACIL) {
        return 2;
    }

    if (nivel == MEDIO) {
        return 3;
    }

    return 4;
}

static int casasErroNivel(NivelPergunta nivel) {
    if (nivel == FACIL) {
        return 1;
    }

    if (nivel == MEDIO) {
        return 2;
    }

    return 3;
}

static const char* nomeNivelGUI(NivelPergunta nivel) {
    if (nivel == FACIL) {
        return "Facil";
    }

    if (nivel == MEDIO) {
        return "Medio";
    }

    return "Dificil";
}

static int encontrarPosicaoCasaGrid(int idCasa, int *linhaOut, int *colunaOut) {
    int ordem[6][6] = {
        {0, 1, 2, 3, 4, 5},
        {11, 10, 9, 8, 7, 6},
        {12, 13, 14, 15, 16, 17},
        {23, 22, 21, 20, 19, 18},
        {24, 25, 26, 27, 28, 29},
        {30, -1, -1, -1, -1, -1}
    };

    for (int linha = 0; linha < 6; linha++) {
        for (int coluna = 0; coluna < 6; coluna++) {
            if (ordem[linha][coluna] == idCasa) {
                *linhaOut = linha;
                *colunaOut = coluna;
                return 1;
            }
        }
    }

    return 0;
}

static Vector2 centroCasaTabuleiro(int idCasa) {
    int linha = 0;
    int coluna = 0;

    if (!encontrarPosicaoCasaGrid(idCasa, &linha, &coluna)) {
        return (Vector2){0, 0};
    }

    return (Vector2){
        TAB_X + coluna * (TAB_TAM + TAB_GAP) + TAB_TAM / 2.0f,
        TAB_Y + linha * (TAB_TAM + TAB_GAP) + TAB_TAM - 16.0f
    };
}

static Vector2 interpolar(Vector2 a, Vector2 b, float t) {
    return (Vector2){
        a.x + (b.x - a.x) * t,
        a.y + (b.y - a.y) * t
    };
}

static void desenharPeao(Vector2 posicao, Color cor, int ativo) {
    DrawCircle((int)posicao.x, (int)posicao.y, ativo ? 12 : 9, cor);
    DrawCircle((int)posicao.x - 3, (int)posicao.y - 4, ativo ? 4 : 3, Fade(WHITE, 0.45f));

    if (ativo) {
        DrawCircleLines((int)posicao.x, (int)posicao.y, 16, WHITE);
    }
}

static Vector2 girarPonto(Vector2 centro, float x, float y, float graus) {
    float rad = graus * DEG2RAD;
    float c = cosf(rad);
    float s = sinf(rad);

    return (Vector2){
        centro.x + x * c - y * s,
        centro.y + x * s + y * c
    };
}

static void desenharPontoDado(Vector2 centro, float x, float y, float angulo, float raio) {
    Vector2 p = girarPonto(centro, x, y, angulo);
    DrawCircleV(p, raio, COR_FUNDO);
}

static void desenharDadoGrafico(int x, int y, int tamanho, int valor, float angulo) {
    Vector2 centro = {(float)x + tamanho / 2.0f, (float)y + tamanho / 2.0f};
    float raioQuadrado = tamanho * 0.72f;
    float desloc = tamanho * 0.22f;
    float raioPonto = tamanho * 0.055f;

    DrawPoly(centro, 4, raioQuadrado, angulo + 45.0f, RAYWHITE);
    DrawPolyLinesEx(centro, 4, raioQuadrado, angulo + 45.0f, 3.0f, COR_ALERTA);

    if (valor == 1 || valor == 3 || valor == 5) {
        desenharPontoDado(centro, 0, 0, angulo, raioPonto);
    }

    if (valor >= 2) {
        desenharPontoDado(centro, -desloc, -desloc, angulo, raioPonto);
        desenharPontoDado(centro, desloc, desloc, angulo, raioPonto);
    }

    if (valor >= 4) {
        desenharPontoDado(centro, desloc, -desloc, angulo, raioPonto);
        desenharPontoDado(centro, -desloc, desloc, angulo, raioPonto);
    }

    if (valor == 6) {
        desenharPontoDado(centro, -desloc, 0, angulo, raioPonto);
        desenharPontoDado(centro, desloc, 0, angulo, raioPonto);
    }
}

static void iniciarMovimentoAnimado(
    AnimacaoGUI *animacao,
    JogadorGUI jogadores[],
    int indiceJogador,
    int passos,
    TipoMovimentoGUI tipo
) {
    int direcao = passos >= 0 ? 1 : -1;
    int total = passos >= 0 ? passos : -passos;
    Casa *proximaCasa = NULL;

    if (animacao == NULL || total <= 0) {
        return;
    }

    proximaCasa = moverCasas(jogadores[indiceJogador].casaAtual, direcao);

    animacao->movimentoAtivo = 1;
    animacao->jogador = indiceJogador;
    animacao->direcao = direcao;
    animacao->passosRestantes = total;
    animacao->origemId = jogadores[indiceJogador].posicaoAtual;
    animacao->destinoId = proximaCasa != NULL ? proximaCasa->id : jogadores[indiceJogador].posicaoAtual;
    animacao->movimentoTempo = 0.0f;
    animacao->movimentoDuracao = 0.28f;
    animacao->tipoMovimento = tipo;
}

static void iniciarRolagemDado(AnimacaoGUI *animacao) {
    if (animacao == NULL) {
        return;
    }

    animacao->dadoRolando = 1;
    animacao->dadoTempo = 0.0f;
    animacao->dadoDuracao = 1.0f;
    animacao->trocaDadoTempo = 0.0f;
    animacao->dadoResultado = (rand() % 6) + 1;
    animacao->dadoVisivel = (rand() % 6) + 1;
}

static TipoMovimentoGUI atualizarAnimacoesGUI(
    AnimacaoGUI *animacao,
    JogadorGUI jogadores[],
    NoCasa **arvoreQuedas,
    float dt,
    int *dadoAtual
) {
    if (animacao == NULL) {
        return MOVIMENTO_NENHUM;
    }

    if (animacao->dadoRolando) {
        animacao->dadoTempo += dt;
        animacao->trocaDadoTempo += dt;

        if (animacao->trocaDadoTempo >= 0.08f) {
            animacao->trocaDadoTempo = 0.0f;
            animacao->dadoVisivel = (rand() % 6) + 1;
        }

        if (animacao->dadoTempo >= animacao->dadoDuracao) {
            animacao->dadoRolando = 0;
            animacao->dadoVisivel = animacao->dadoResultado;
            *dadoAtual = animacao->dadoResultado;
            iniciarMovimentoAnimado(animacao, jogadores, animacao->jogador, animacao->dadoResultado, MOVIMENTO_DADO);
        }
    }

    if (animacao->movimentoAtivo) {
        JogadorGUI *jogador = &jogadores[animacao->jogador];
        animacao->movimentoTempo += dt;

        if (animacao->movimentoTempo >= animacao->movimentoDuracao) {
            Casa *proximaCasa = moverCasas(jogador->casaAtual, animacao->direcao);

            if (proximaCasa != NULL) {
                jogador->casaAtual = proximaCasa;
                sincronizarPosicao(jogador);
            }

            animacao->passosRestantes--;

            if (animacao->passosRestantes <= 0 || jogador->posicaoAtual >= CASA_FINAL) {
                TipoMovimentoGUI tipoFinalizado = animacao->tipoMovimento;
                animacao->movimentoAtivo = 0;

                if (arvoreQuedas != NULL && jogador->casaAtual != NULL) {
                    registrarQuedaCasa(arvoreQuedas, jogador->casaAtual->id);
                }

                return tipoFinalizado;
            }

            proximaCasa = moverCasas(jogador->casaAtual, animacao->direcao);

            if (proximaCasa == NULL || proximaCasa->id == jogador->posicaoAtual) {
                TipoMovimentoGUI tipoFinalizado = animacao->tipoMovimento;
                animacao->movimentoAtivo = 0;

                if (arvoreQuedas != NULL && jogador->casaAtual != NULL) {
                    registrarQuedaCasa(arvoreQuedas, jogador->casaAtual->id);
                }

                return tipoFinalizado;
            }

            animacao->origemId = jogador->posicaoAtual;
            animacao->destinoId = proximaCasa->id;
            animacao->movimentoTempo = 0.0f;
        }
    }

    return MOVIMENTO_NENHUM;
}

static tp_item converterJogadorGUI(JogadorGUI jogador, int id) {
    tp_item item;

    item.id = id;
    strncpy(item.nome, jogador.nome, sizeof(item.nome) - 1);
    item.nome[sizeof(item.nome) - 1] = '\0';
    item.casaAtual = jogador.casaAtual;
    item.posicaoAtual = jogador.posicaoAtual;
    item.pontuacao = jogador.pontuacao;
    item.acertos = jogador.acertos;
    item.erros = jogador.erros;
    item.preso = jogador.preso;
    item.cor = COR_BRANCO;

    return item;
}

static int carregarRankingGeralGUI(LinhaRankingGUI linhas[], int limite) {
    FILE *arquivo = fopen("ranking_geral.csv", "r");
    char linha[400];
    int quantidade = 0;

    if (arquivo == NULL) {
        return 0;
    }

    while (fgets(linha, sizeof(linha), arquivo) != NULL && quantidade < limite) {
        if (strncmp(linha, "nome_jogador", 12) == 0) {
            continue;
        }

        char *nome = strtok(linha, ";");
        char *acertos = strtok(NULL, ";");
        char *erros = strtok(NULL, ";");
        char *pontos = strtok(NULL, ";");
        char *partidas = strtok(NULL, ";\n\r");

        if (nome == NULL || acertos == NULL || erros == NULL || pontos == NULL || partidas == NULL) {
            continue;
        }

        strncpy(linhas[quantidade].nome, nome, sizeof(linhas[quantidade].nome) - 1);
        linhas[quantidade].nome[sizeof(linhas[quantidade].nome) - 1] = '\0';
        linhas[quantidade].acertos = atoi(acertos);
        linhas[quantidade].erros = atoi(erros);
        linhas[quantidade].pontos = atoi(pontos);
        linhas[quantidade].partidas = atoi(partidas);
        quantidade++;
    }

    fclose(arquivo);

    return quantidade;
}

static int encontrarTemaResumoGUI(TemaResumoGUI temas[], int quantidade, const char *tema) {
    for (int i = 0; i < quantidade; i++) {
        if (strcmp(temas[i].tema, tema) == 0) {
            return i;
        }
    }

    return -1;
}

static int carregarAnaliseTemaGUI(TemaResumoGUI temas[], int limite) {
    FILE *arquivo = fopen("historico_respostas.csv", "r");
    char linha[1000];
    int quantidade = 0;

    if (arquivo == NULL) {
        return 0;
    }

    while (fgets(linha, sizeof(linha), arquivo) != NULL) {
        if (strncmp(linha, "turma;", 6) == 0) {
            continue;
        }

        strtok(linha, ";");
        strtok(NULL, ";");
        strtok(NULL, ";");
        strtok(NULL, ";");
        strtok(NULL, ";");
        char *tema = strtok(NULL, ";");
        strtok(NULL, ";");
        strtok(NULL, ";");
        strtok(NULL, ";");
        strtok(NULL, ";");
        char *resultado = strtok(NULL, ";\n\r");

        if (tema == NULL || resultado == NULL) {
            continue;
        }

        int indice = encontrarTemaResumoGUI(temas, quantidade, tema);

        if (indice == -1 && quantidade < limite) {
            indice = quantidade;
            strncpy(temas[indice].tema, tema, sizeof(temas[indice].tema) - 1);
            temas[indice].tema[sizeof(temas[indice].tema) - 1] = '\0';
            temas[indice].acertos = 0;
            temas[indice].erros = 0;
            quantidade++;
        }

        if (indice != -1) {
            if (strcmp(resultado, "Acertou") == 0) {
                temas[indice].acertos++;
            }
            else if (strcmp(resultado, "Errou") == 0) {
                temas[indice].erros++;
            }
        }
    }

    fclose(arquivo);

    return quantidade;
}

static void desenharTabelaRankingGeralGUI(int x, int y) {
    LinhaRankingGUI linhas[MAX_RANKING_GUI];
    int quantidade = carregarRankingGeralGUI(linhas, MAX_RANKING_GUI);

    DrawText("Ranking geral", x, y, 26, COR_TEXTO);

    if (quantidade == 0) {
        DrawText("Nenhuma partida registrada ainda.", x, y + 42, 20, COR_TEXTO_FRACO);
        return;
    }

    DrawText("Nome", x, y + 44, 18, COR_ALERTA);
    DrawText("Acertos", x + 230, y + 44, 18, COR_ALERTA);
    DrawText("Erros", x + 330, y + 44, 18, COR_ALERTA);
    DrawText("Pontos", x + 415, y + 44, 18, COR_ALERTA);
    DrawText("Partidas", x + 510, y + 44, 18, COR_ALERTA);

    for (int i = 0; i < quantidade; i++) {
        int linhaY = y + 78 + i * 28;
        char buffer[40];

        DrawText(linhas[i].nome, x, linhaY, 18, COR_TEXTO);
        snprintf(buffer, sizeof(buffer), "%d", linhas[i].acertos);
        DrawText(buffer, x + 230, linhaY, 18, COR_TEXTO_FRACO);
        snprintf(buffer, sizeof(buffer), "%d", linhas[i].erros);
        DrawText(buffer, x + 330, linhaY, 18, COR_TEXTO_FRACO);
        snprintf(buffer, sizeof(buffer), "%d", linhas[i].pontos);
        DrawText(buffer, x + 415, linhaY, 18, COR_TEXTO_FRACO);
        snprintf(buffer, sizeof(buffer), "%d", linhas[i].partidas);
        DrawText(buffer, x + 510, linhaY, 18, COR_TEXTO_FRACO);
    }
}

static void desenharAnaliseTemaGUI(int x, int y) {
    TemaResumoGUI temas[MAX_TEMAS_GUI];
    int quantidade = carregarAnaliseTemaGUI(temas, MAX_TEMAS_GUI);

    DrawText("Analise por tema", x, y, 26, COR_TEXTO);

    if (quantidade == 0) {
        DrawText("Nenhuma resposta registrada ainda.", x, y + 42, 20, COR_TEXTO_FRACO);
        return;
    }

    DrawText("Tema", x, y + 44, 18, COR_ALERTA);
    DrawText("Acertos", x + 310, y + 44, 18, COR_ALERTA);
    DrawText("Erros", x + 410, y + 44, 18, COR_ALERTA);
    DrawText("Total", x + 500, y + 44, 18, COR_ALERTA);
    DrawText("Aproveit.", x + 580, y + 44, 18, COR_ALERTA);

    for (int i = 0; i < quantidade; i++) {
        int linhaY = y + 78 + i * 28;
        int total = temas[i].acertos + temas[i].erros;
        double aproveitamento = total > 0 ? (temas[i].acertos * 100.0) / total : 0.0;
        char buffer[50];

        DrawText(temas[i].tema, x, linhaY, 18, COR_TEXTO);
        snprintf(buffer, sizeof(buffer), "%d", temas[i].acertos);
        DrawText(buffer, x + 310, linhaY, 18, COR_TEXTO_FRACO);
        snprintf(buffer, sizeof(buffer), "%d", temas[i].erros);
        DrawText(buffer, x + 410, linhaY, 18, COR_TEXTO_FRACO);
        snprintf(buffer, sizeof(buffer), "%d", total);
        DrawText(buffer, x + 500, linhaY, 18, COR_TEXTO_FRACO);
        snprintf(buffer, sizeof(buffer), "%.1f%%", aproveitamento);
        DrawText(buffer, x + 580, linhaY, 18, COR_TEXTO_FRACO);
    }
}

static void desenharTabuleiroGUI(
    Casa *inicio,
    JogadorGUI jogadores[],
    int qtdJogadores,
    int jogadorAtual,
    AnimacaoGUI *animacao
) {
    int ordem[6][6] = {
        {0, 1, 2, 3, 4, 5},
        {11, 10, 9, 8, 7, 6},
        {12, 13, 14, 15, 16, 17},
        {23, 22, 21, 20, 19, 18},
        {24, 25, 26, 27, 28, 29},
        {30, -1, -1, -1, -1, -1}
    };

    int jogadorPulando = (animacao != NULL && animacao->movimentoAtivo) ? animacao->jogador : -1;

    for (int linha = 0; linha < 6; linha++) {
        for (int coluna = 0; coluna < 6; coluna++) {
            int idCasa = ordem[linha][coluna];

            if (idCasa < 0) {
                continue;
            }

            Casa *casa = inicio;
            while (casa != NULL && casa->id != idCasa) {
                casa = casa->prox;
            }

            if (casa == NULL) {
                continue;
            }

            Rectangle r = {
                (float)(TAB_X + coluna * (TAB_TAM + TAB_GAP)),
                (float)(TAB_Y + linha * (TAB_TAM + TAB_GAP)),
                (float)TAB_TAM,
                (float)TAB_TAM
            };

            DrawRectangleRounded(r, 0.14f, 8, corCasa(casa->tipo));
            DrawRectangleRoundedLines(r, 0.14f, 8, Fade(WHITE, 0.35f));

            char idTexto[8];
            snprintf(idTexto, sizeof(idTexto), "%02d", casa->id);
            DrawText(idTexto, (int)r.x + 8, (int)r.y + 8, 22, COR_TEXTO);
            DrawText(nomeTipoCasa(casa->tipo), (int)r.x + 8, (int)r.y + 36, 12, Fade(COR_TEXTO, 0.84f));

            int ocupantes = 0;
            for (int j = 0; j < qtdJogadores; j++) {
                if (j == jogadorPulando) {
                    continue;
                }

                if (jogadores[j].casaAtual != NULL && jogadores[j].casaAtual->id == casa->id) {
                    int px = (int)r.x + 17 + ocupantes * 18;
                    int py = (int)r.y + 62;
                    desenharPeao((Vector2){(float)px, (float)py}, jogadores[j].cor, j == jogadorAtual);

                    ocupantes++;
                }
            }
        }
    }

    if (jogadorPulando >= 0) {
        float progresso = animacao->movimentoTempo / animacao->movimentoDuracao;
        float salto = 0.0f;
        Vector2 origem = centroCasaTabuleiro(animacao->origemId);
        Vector2 destino = centroCasaTabuleiro(animacao->destinoId);
        Vector2 posicao;

        if (progresso < 0.0f) {
            progresso = 0.0f;
        }
        else if (progresso > 1.0f) {
            progresso = 1.0f;
        }

        posicao = interpolar(origem, destino, progresso);
        salto = sinf(progresso * PI) * 26.0f;
        posicao.y -= salto;

        desenharPeao(posicao, jogadores[jogadorPulando].cor, 1);
    }
}

static void desenharPainelJogadores(JogadorGUI jogadores[], int qtdJogadores, int jogadorAtual, const char *grupo) {
    Rectangle painel = {650, 220, 500, 390};
    DrawRectangleRounded(painel, 0.04f, 8, COR_PAINEL);
    DrawText("Jogadores", 675, 245, 28, COR_TEXTO);

    char grupoTexto[80];
    snprintf(grupoTexto, sizeof(grupoTexto), "Grupo do historico: %s", grupo);
    DrawText(grupoTexto, 675, 280, 18, COR_TEXTO_FRACO);

    for (int i = 0; i < qtdJogadores; i++) {
        int y = 325 + i * 62;
        Color linha = (i == jogadorAtual) ? COR_PAINEL_CLARO : Fade(COR_PAINEL_CLARO, 0.42f);
        DrawRectangleRounded((Rectangle){675, (float)y, 440, 48}, 0.08f, 8, linha);
        DrawCircle(696, y + 24, 10, jogadores[i].cor);
        DrawText(jogadores[i].nome, 715, y + 8, 20, COR_TEXTO);

        char dados[160];
        snprintf(
            dados,
            sizeof(dados),
            "Casa %02d | Acertos %d | Erros %d | Pontos %d",
            jogadores[i].posicaoAtual,
            jogadores[i].acertos,
            jogadores[i].erros,
            jogadores[i].pontuacao
        );
        DrawText(dados, 715, y + 30, 14, COR_TEXTO_FRACO);
    }
}

static void desenharRankingGUI(JogadorGUI jogadores[], int qtdJogadores) {
    DrawText("Ranking da partida", 650, 625, 26, COR_TEXTO);

    tp_item ranking[MAX_GUI_JOGADORES];
    for (int i = 0; i < qtdJogadores; i++) {
        ranking[i] = converterJogadorGUI(jogadores[i], i + 1);
    }

    ordenarJogadoresPartida(ranking, qtdJogadores);

    for (int i = 0; i < qtdJogadores; i++) {
        char linha[180];
        snprintf(
            linha,
            sizeof(linha),
            "%d. %s  A:%d  E:%d  P:%d",
            i + 1,
            ranking[i].nome,
            ranking[i].acertos,
            ranking[i].erros,
            ranking[i].pontuacao
        );
        DrawText(linha, 655, 665 + i * 24, 18, COR_TEXTO_FRACO);
    }
}

static void finalizarPartidaGUI(
    JogadorGUI jogadores[],
    int qtdJogadores,
    int indiceVencedor,
    NoCasa *arvoreQuedas
) {
    tp_item ranking[MAX_GUI_JOGADORES];
    int idPartida = obterProximoIdPartida();

    for (int i = 0; i < qtdJogadores; i++) {
        ranking[i] = converterJogadorGUI(jogadores[i], i + 1);
    }

    ordenarJogadoresPartida(ranking, qtdJogadores);
    salvarRankingPartidaCSV(idPartida, ranking, qtdJogadores);
    atualizarSalvarExibirRankingGeral(ranking, qtdJogadores);
    salvarQuedasCasasCSV(arvoreQuedas, "quedas_casas.csv");

    (void) indiceVencedor;
}

static void desenharTelaMenu(TelaGUI *telaAtual) {
    int centroX = GetScreenWidth() / 2;
    Rectangle painel = {centroX - 245.0f, 125, 490, 430};

    DrawRectangleRounded(painel, 0.06f, 12, COR_PAINEL);
    DrawRectangleRoundedLines(painel, 0.06f, 12, Fade(COR_DESTAQUE, 0.45f));

    desenharTextoCentralizado("JOGO DO SUSA", 165, 48, COR_TEXTO);
    desenharTextoCentralizado("ARSENAI", 218, 22, COR_DESTAQUE);
    desenharTextoCentralizado("Versao grafica com Raylib", 258, 24, COR_TEXTO_FRACO);

    BotaoGUI iniciar = {{centroX - 150.0f, 315, 300, 60}, "Iniciar jogo"};
    BotaoGUI ranking = {{centroX - 150.0f, 390, 300, 60}, "Ver ranking"};
    BotaoGUI sair = {{centroX - 150.0f, 465, 300, 60}, "Sair"};

    desenharBotao(iniciar, COR_DESTAQUE);
    desenharBotao(ranking, COR_PAINEL_CLARO);
    desenharBotao(sair, COR_ERRO);

    desenharTextoCentralizado("Use o mouse para jogar, rolar o dado e responder perguntas.", 590, 20, COR_TEXTO_FRACO);

    if (botaoClicado(iniciar)) {
        *telaAtual = TELA_GRUPO;
    }

    if (botaoClicado(ranking)) {
        *telaAtual = TELA_RANKING;
    }

    if (botaoClicado(sair)) {
        CloseWindow();
    }
}

int main(void) {
    const int largura = 1200;
    const int altura = 800;

    InitWindow(largura, altura, "Jogo do SUSA - ARSENAI");
    fonteUI = LoadFontEx("C:\\Windows\\Fonts\\segoeuib.ttf", 64, NULL, 0);
    if (fonteUI.texture.id != 0) {
        SetTextureFilter(fonteUI.texture, TEXTURE_FILTER_BILINEAR);
        fonteUICarregada = 1;
    }
    SetTargetFPS(60);

    srand((unsigned int)time(NULL));

    Casa *inicioTabuleiro = NULL;
    Casa *fimTabuleiro = NULL;
    criarTabuleiroPadrao(&inicioTabuleiro, &fimTabuleiro);
    salvarPerguntasCSV();
    inicializarHistoricoRespostasCSV();

    TelaGUI telaAtual = TELA_MENU;
    JogadorGUI jogadores[MAX_GUI_JOGADORES];
    int qtdJogadores = 2;
    int jogadorCadastro = 0;
    int jogadorAtual = 0;
    int indiceVencedor = -1;
    int dadoAtual = 0;
    int jogoFinalizado = 0;
    char grupoHistorico[30] = "2026.1";
    char nomeDigitado[50] = "";
    char mensagem[180] = "Clique em Rolar dado para jogar.";
    NoCasa *arvoreQuedas = NULL;
    AnimacaoGUI animacao = {0};
    HistoricoAcoesGUI historicoAcoes = {0};
    float atrasoAcao = 0.0f;
    TelaGUI telaDepoisAtraso = TELA_JOGO;
    int trocarJogadorDepoisAtraso = 0;

    NivelPergunta nivelEscolhido = FACIL;
    Questao questaoAtual;
    int temQuestaoAtual = 0;

    Color cores[MAX_GUI_JOGADORES] = {
        {71, 190, 125, 255},
        {80, 150, 240, 255},
        {214, 110, 220, 255},
        {236, 199, 91, 255}
    };

    for (int i = 0; i < MAX_GUI_JOGADORES; i++) {
        snprintf(jogadores[i].nome, sizeof(jogadores[i].nome), "Jogador %d", i + 1);
        jogadores[i].casaAtual = inicioTabuleiro;
        jogadores[i].posicaoAtual = 0;
        jogadores[i].pontuacao = 0;
        jogadores[i].acertos = 0;
        jogadores[i].erros = 0;
        jogadores[i].preso = 0;
        jogadores[i].cor = cores[i];
    }

    while (!WindowShouldClose()) {
        float dt = GetFrameTime();
        TipoMovimentoGUI movimentoFinalizado = MOVIMENTO_NENHUM;

        if (atrasoAcao > 0.0f) {
            atrasoAcao -= dt;

            if (atrasoAcao <= 0.0f) {
                atrasoAcao = 0.0f;

                if (trocarJogadorDepoisAtraso) {
                    jogadorAtual = (jogadorAtual + 1) % qtdJogadores;
                    char acao[160];
                    snprintf(acao, sizeof(acao), "Vez de %s.", jogadores[jogadorAtual].nome);
                    adicionarAcaoHistorico(&historicoAcoes, acao);
                    trocarJogadorDepoisAtraso = 0;
                }

                telaAtual = telaDepoisAtraso;
            }
        }

        if (atrasoAcao <= 0.0f) {
            movimentoFinalizado = atualizarAnimacoesGUI(
                &animacao,
                jogadores,
                &arvoreQuedas,
                dt,
                &dadoAtual
            );
        }

        if (movimentoFinalizado != MOVIMENTO_NENHUM) {
            JogadorGUI *j = &jogadores[jogadorAtual];
            char acao[160];

            if (j->posicaoAtual >= CASA_FINAL) {
                indiceVencedor = jogadorAtual;
                jogoFinalizado = 1;
                snprintf(acao, sizeof(acao), "%s chegou na casa final e venceu.", j->nome);
                adicionarAcaoHistorico(&historicoAcoes, acao);
                finalizarPartidaGUI(jogadores, qtdJogadores, indiceVencedor, arvoreQuedas);
                telaAtual = TELA_FIM;
            }
            else if (movimentoFinalizado == MOVIMENTO_DADO) {
                snprintf(acao, sizeof(acao), "%s tirou %d e foi para a casa %d.", j->nome, dadoAtual, j->posicaoAtual);
                adicionarAcaoHistorico(&historicoAcoes, acao);

                if (j->casaAtual->tipo == PRISAO) {
                    j->preso = 1;
                    snprintf(mensagem, sizeof(mensagem), "%s caiu na prisao e perde a proxima rodada.", j->nome);
                    adicionarAcaoHistorico(&historicoAcoes, mensagem);
                    atrasoAcao = 1.15f;
                    trocarJogadorDepoisAtraso = 1;
                    telaDepoisAtraso = TELA_JOGO;
                }
                else if (j->casaAtual->tipo == PERGUNTA) {
                    snprintf(mensagem, sizeof(mensagem), "%s caiu em pergunta. Escolha a dificuldade.", j->nome);
                    adicionarAcaoHistorico(&historicoAcoes, mensagem);
                    atrasoAcao = 0.85f;
                    trocarJogadorDepoisAtraso = 0;
                    telaDepoisAtraso = TELA_ESCOLHER_DIFICULDADE;
                }
                else {
                    snprintf(mensagem, sizeof(mensagem), "%s foi para a casa %d.", j->nome, j->posicaoAtual);
                    atrasoAcao = 0.9f;
                    trocarJogadorDepoisAtraso = 1;
                    telaDepoisAtraso = TELA_JOGO;
                }
            }
            else if (movimentoFinalizado == MOVIMENTO_RESPOSTA) {
                snprintf(acao, sizeof(acao), "%s terminou na casa %d.", j->nome, j->posicaoAtual);
                adicionarAcaoHistorico(&historicoAcoes, acao);
                atrasoAcao = 1.0f;
                trocarJogadorDepoisAtraso = 1;
                telaDepoisAtraso = TELA_JOGO;
            }
        }

        BeginDrawing();
        ClearBackground(COR_FUNDO);

        if (telaAtual == TELA_MENU) {
            desenharTelaMenu(&telaAtual);
        }
        else if (telaAtual == TELA_GRUPO) {
            int centroX = GetScreenWidth() / 2;
            DrawRectangleRounded((Rectangle){centroX - 260.0f, 145, 520, 360}, 0.06f, 12, COR_PAINEL);
            DrawRectangleRoundedLines((Rectangle){centroX - 260.0f, 145, 520, 360}, 0.06f, 12, Fade(COR_DESTAQUE, 0.45f));
            desenharTextoCentralizado("Escolha o grupo", 185, 34, COR_TEXTO);
            desenharTextoCentralizado("Esse valor sera salvo no historico_respostas.csv.", 230, 18, COR_TEXTO_FRACO);

            BotaoGUI op1 = {{centroX - 155.0f, 285, 310, 58}, "2026.1"};
            BotaoGUI op2 = {{centroX - 155.0f, 360, 310, 58}, "professor"};
            BotaoGUI op3 = {{centroX - 155.0f, 435, 310, 58}, "outro_semestre"};

            desenharBotao(op1, COR_DESTAQUE);
            desenharBotao(op2, COR_PAINEL_CLARO);
            desenharBotao(op3, COR_PAINEL_CLARO);

            if (botaoClicado(op1) || botaoClicado(op2) || botaoClicado(op3)) {
                if (botaoClicado(op1)) {
                    strcpy(grupoHistorico, "2026.1");
                }
                else if (botaoClicado(op2)) {
                    strcpy(grupoHistorico, "professor");
                }
                else {
                    strcpy(grupoHistorico, "outro_semestre");
                }

                qtdJogadores = 2;
                jogadorCadastro = 0;
                nomeDigitado[0] = '\0';
                telaAtual = TELA_CADASTRO;
            }
        }
        else if (telaAtual == TELA_CADASTRO) {
            int centroX = GetScreenWidth() / 2;
            DrawRectangleRounded((Rectangle){centroX - 330.0f, 110, 660, 500}, 0.05f, 12, COR_PAINEL);
            DrawRectangleRoundedLines((Rectangle){centroX - 330.0f, 110, 660, 500}, 0.05f, 12, Fade(COR_DESTAQUE, 0.45f));
            desenharTextoCentralizado("Cadastro dos jogadores", 145, 34, COR_TEXTO);
            desenharTextoCentralizado("Digite o nome e clique em confirmar.", 188, 20, COR_TEXTO_FRACO);

            DrawText("Quantidade de jogadores", centroX - 140, 235, 22, COR_TEXTO);
            BotaoGUI dois = {{centroX - 135.0f, 272, 80, 45}, "2"};
            BotaoGUI tres = {{centroX - 40.0f, 272, 80, 45}, "3"};
            BotaoGUI quatro = {{centroX + 55.0f, 272, 80, 45}, "4"};

            desenharBotao(dois, qtdJogadores == 2 ? COR_DESTAQUE : COR_PAINEL_CLARO);
            desenharBotao(tres, qtdJogadores == 3 ? COR_DESTAQUE : COR_PAINEL_CLARO);
            desenharBotao(quatro, qtdJogadores == 4 ? COR_DESTAQUE : COR_PAINEL_CLARO);

            if (botaoClicado(dois)) qtdJogadores = 2;
            if (botaoClicado(tres)) qtdJogadores = 3;
            if (botaoClicado(quatro)) qtdJogadores = 4;

            char tituloNome[80];
            snprintf(tituloNome, sizeof(tituloNome), "Nome do jogador %d", jogadorCadastro + 1);
            DrawText(tituloNome, centroX - 210, 350, 22, COR_TEXTO);
            DrawRectangleRounded((Rectangle){centroX - 210.0f, 385, 420, 54}, 0.08f, 8, COR_PAINEL_CLARO);
            DrawRectangleRoundedLines((Rectangle){centroX - 210.0f, 385, 420, 54}, 0.08f, 8, COR_DESTAQUE);
            DrawText(nomeDigitado, centroX - 192, 401, 22, COR_TEXTO);

            int tecla = GetCharPressed();
            while (tecla > 0) {
                int len = (int)strlen(nomeDigitado);
                if (len < 48 && tecla >= 32 && tecla <= 125) {
                    nomeDigitado[len] = (char)tecla;
                    nomeDigitado[len + 1] = '\0';
                }
                tecla = GetCharPressed();
            }

            if (IsKeyPressed(KEY_BACKSPACE)) {
                int len = (int)strlen(nomeDigitado);
                if (len > 0) {
                    nomeDigitado[len - 1] = '\0';
                }
            }

            BotaoGUI confirmar = {{centroX - 110.0f, 470, 220, 54}, "Confirmar"};
            desenharBotao(confirmar, COR_DESTAQUE);

            if (botaoClicado(confirmar) || IsKeyPressed(KEY_ENTER)) {
                if (strlen(nomeDigitado) == 0) {
                    snprintf(nomeDigitado, sizeof(nomeDigitado), "Jogador %d", jogadorCadastro + 1);
                }

                strncpy(jogadores[jogadorCadastro].nome, nomeDigitado, sizeof(jogadores[jogadorCadastro].nome) - 1);
                jogadores[jogadorCadastro].nome[sizeof(jogadores[jogadorCadastro].nome) - 1] = '\0';
                jogadores[jogadorCadastro].casaAtual = inicioTabuleiro;
                sincronizarPosicao(&jogadores[jogadorCadastro]);
                jogadores[jogadorCadastro].pontuacao = 0;
                jogadores[jogadorCadastro].acertos = 0;
                jogadores[jogadorCadastro].erros = 0;
                jogadores[jogadorCadastro].preso = 0;

                jogadorCadastro++;
                nomeDigitado[0] = '\0';

                if (jogadorCadastro >= qtdJogadores) {
                    jogadorAtual = 0;
                    jogoFinalizado = 0;
                    indiceVencedor = -1;
                    dadoAtual = 0;
                    strcpy(mensagem, "Clique em Rolar dado para jogar.");

                    if (arvoreQuedas != NULL) {
                        liberarArvoreCasas(arvoreQuedas);
                    }
                    arvoreQuedas = NULL;
                    memset(&animacao, 0, sizeof(animacao));
                    atrasoAcao = 0.0f;
                    trocarJogadorDepoisAtraso = 0;
                    telaDepoisAtraso = TELA_JOGO;
                    historicoAcoes.quantidade = 0;
                    adicionarAcaoHistorico(&historicoAcoes, "Partida iniciada.");
                    {
                        char acao[160];
                        snprintf(acao, sizeof(acao), "Vez de %s.", jogadores[jogadorAtual].nome);
                        adicionarAcaoHistorico(&historicoAcoes, acao);
                    }

                    telaAtual = TELA_JOGO;
                }
            }

            DrawText("Jogadores cadastrados", centroX - 285, 545, 20, COR_TEXTO);
            for (int i = 0; i < jogadorCadastro; i++) {
                DrawCircle(centroX - 270 + i * 150, 585, 9, jogadores[i].cor);
                DrawText(jogadores[i].nome, centroX - 255 + i * 150, 574, 19, COR_TEXTO_FRACO);
            }
        }
        else if (telaAtual == TELA_JOGO) {
            desenharTitulo("Tabuleiro", mensagem);
            desenharTabuleiroGUI(inicioTabuleiro, jogadores, qtdJogadores, jogadorAtual, &animacao);
            desenharPainelJogadores(jogadores, qtdJogadores, jogadorAtual, grupoHistorico);
            desenharHistoricoAcoes(&historicoAcoes, 650, 625);

            char dadoTexto[40];
            int dadoDesenhado = animacao.dadoRolando ? animacao.dadoVisivel : (dadoAtual > 0 ? dadoAtual : 1);
            float anguloDado = animacao.dadoRolando ? animacao.dadoTempo * 720.0f : 0.0f;
            snprintf(dadoTexto, sizeof(dadoTexto), "Dado: %d", dadoAtual);
            desenharDadoGrafico(650, 138, 72, dadoDesenhado, anguloDado);
            DrawText(dadoTexto, 735, 162, 26, COR_ALERTA);

            BotaoGUI rolar = {{820, 150, 180, 54}, "Rolar dado"};
            int podeRolar = !animacao.dadoRolando && !animacao.movimentoAtivo;
            desenharBotao(rolar, podeRolar ? COR_DESTAQUE : Fade(COR_PAINEL_CLARO, 0.75f));

            if (atrasoAcao > 0.0f) {
                DrawText("Aguarde...", 1018, 164, 20, COR_TEXTO_FRACO);
            }

            if (podeRolar && atrasoAcao <= 0.0f && botaoClicado(rolar)) {
                JogadorGUI *j = &jogadores[jogadorAtual];
                char acao[160];

                if (j->preso) {
                    j->preso = 0;
                    snprintf(mensagem, sizeof(mensagem), "%s perdeu a rodada porque estava preso.", j->nome);
                    adicionarAcaoHistorico(&historicoAcoes, mensagem);
                    atrasoAcao = 0.85f;
                    trocarJogadorDepoisAtraso = 1;
                    telaDepoisAtraso = TELA_JOGO;
                }
                else {
                    animacao.jogador = jogadorAtual;
                    iniciarRolagemDado(&animacao);
                    snprintf(mensagem, sizeof(mensagem), "%s esta rolando o dado...", j->nome);
                    snprintf(acao, sizeof(acao), "%s clicou para rolar o dado.", j->nome);
                    adicionarAcaoHistorico(&historicoAcoes, acao);
                }
            }
        }
        else if (telaAtual == TELA_ESCOLHER_DIFICULDADE) {
            desenharTitulo("Escolha a dificuldade", jogadores[jogadorAtual].nome);

            BotaoGUI facil = {{40, 240, 260, 58}, "Facil"};
            BotaoGUI medio = {{40, 315, 260, 58}, "Medio"};
            BotaoGUI dificil = {{40, 390, 260, 58}, "Dificil"};

            desenharBotao(facil, COR_DESTAQUE);
            desenharBotao(medio, COR_ALERTA);
            desenharBotao(dificil, COR_ERRO);

            if (botaoClicado(facil) || botaoClicado(medio) || botaoClicado(dificil)) {
                if (botaoClicado(facil)) {
                    nivelEscolhido = FACIL;
                }
                else if (botaoClicado(medio)) {
                    nivelEscolhido = MEDIO;
                }
                else {
                    nivelEscolhido = DIFICIL;
                }

                int indicePergunta = sortearIndicePerguntaNivel(nivelEscolhido);
                const Questao *questao = obterQuestaoPorIndice(indicePergunta);

                if (questao != NULL) {
                    questaoAtual = *questao;
                    temQuestaoAtual = 1;
                    telaAtual = TELA_PERGUNTA;
                }
            }
        }
        else if (telaAtual == TELA_PERGUNTA) {
            desenharTitulo("Pergunta", nomeNivelGUI(nivelEscolhido));

            if (temQuestaoAtual) {
                DrawText(questaoAtual.enunciado, 40, 230, 22, COR_TEXTO);

                for (int i = 0; i < 4; i++) {
                    Rectangle area = {60, (float)(300 + i * 78), 850, 58};
                    char textoAlternativa[560];
                    snprintf(textoAlternativa, sizeof(textoAlternativa), "%c) %s", 'A' + i, questaoAtual.alternativas[i]);
                    BotaoGUI botao = {area, textoAlternativa};
                    desenharBotao(botao, COR_PAINEL_CLARO);

                    if (botaoClicado(botao)) {
                        JogadorGUI *j = &jogadores[jogadorAtual];
                        ResultadoPergunta resultado;
                        char acao[160];
                        resultado.questao = questaoAtual;
                        resultado.respostaJogador = i;
                        resultado.acertou = (i == questaoAtual.respostaCorreta);

                        salvarRespostaHistoricoCSV(TURMA_PADRAO, grupoHistorico, j->nome, &resultado);

                        if (resultado.acertou) {
                            j->acertos++;
                            j->pontuacao += pontosPorNivelGUI(nivelEscolhido);
                            snprintf(mensagem, sizeof(mensagem), "%s acertou e avancou casas.", j->nome);
                            snprintf(acao, sizeof(acao), "%s acertou uma pergunta %s.", j->nome, nomeNivelGUI(nivelEscolhido));
                            adicionarAcaoHistorico(&historicoAcoes, acao);
                            iniciarMovimentoAnimado(
                                &animacao,
                                jogadores,
                                jogadorAtual,
                                casasAcertoNivel(nivelEscolhido),
                                MOVIMENTO_RESPOSTA
                            );
                        }
                        else {
                            j->erros++;
                            j->pontuacao -= pontosPorNivelGUI(nivelEscolhido) / 2;
                            if (j->pontuacao < 0) {
                                j->pontuacao = 0;
                            }
                            snprintf(mensagem, sizeof(mensagem), "%s errou e voltou casas.", j->nome);
                            snprintf(acao, sizeof(acao), "%s errou uma pergunta %s.", j->nome, nomeNivelGUI(nivelEscolhido));
                            adicionarAcaoHistorico(&historicoAcoes, acao);
                            iniciarMovimentoAnimado(
                                &animacao,
                                jogadores,
                                jogadorAtual,
                                -casasErroNivel(nivelEscolhido),
                                MOVIMENTO_RESPOSTA
                            );
                        }

                        telaAtual = TELA_JOGO;
                    }
                }
            }
        }
        else if (telaAtual == TELA_RANKING) {
            desenharTitulo("Ranking geral", "Dados carregados dos CSVs do jogo.");

            desenharTabelaRankingGeralGUI(40, 230);
            desenharAnaliseTemaGUI(40, 500);

            BotaoGUI voltar = {{930, 690, 220, 54}, "Voltar"};
            desenharBotao(voltar, COR_DESTAQUE);
            if (botaoClicado(voltar)) {
                telaAtual = TELA_MENU;
            }
        }
        else if (telaAtual == TELA_FIM) {
            desenharTitulo("Fim de jogo", jogoFinalizado ? "Partida salva nos arquivos CSV." : "");

            if (indiceVencedor >= 0) {
                char vencedor[120];
                snprintf(vencedor, sizeof(vencedor), "Vencedor: %s", jogadores[indiceVencedor].nome);
                DrawText(vencedor, 40, 220, 34, COR_DESTAQUE);
            }

            desenharRankingGUI(jogadores, qtdJogadores);

            DrawText("Arquivos atualizados:", 40, 310, 24, COR_TEXTO);
            DrawText("historico_respostas.csv", 60, 350, 22, COR_TEXTO_FRACO);
            DrawText("ranking_partidas.csv", 60, 380, 22, COR_TEXTO_FRACO);
            DrawText("ranking_geral.csv", 60, 410, 22, COR_TEXTO_FRACO);
            DrawText("quedas_casas.csv", 60, 440, 22, COR_TEXTO_FRACO);
            DrawText("perguntas.csv", 60, 470, 22, COR_TEXTO_FRACO);

            BotaoGUI voltar = {{40, 545, 240, 54}, "Menu inicial"};
            desenharBotao(voltar, COR_DESTAQUE);

            if (botaoClicado(voltar)) {
                telaAtual = TELA_MENU;
            }
        }

        EndDrawing();
    }

    if (arvoreQuedas != NULL) {
        liberarArvoreCasas(arvoreQuedas);
    }

    liberarTabuleiro(inicioTabuleiro);
    if (fonteUICarregada) {
        UnloadFont(fonteUI);
    }
    CloseWindow();

    return 0;
}
