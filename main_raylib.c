#include "raylib.h"
// Compilar:
// gcc main_raylib.c fila.c pilha.c questoes.c tabuleiro.c historico.c listade.c arvore_casas.c ranking.c -o jogo_gui.exe -lraylib -lopengl32 -lgdi32 -lwinmm -lm
//   .\jogo_gui.exe
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
#define TAB_X 42
#define TAB_Y 202
#define TAB_TAM 82
#define TAB_GAP 18
#define INTRO_PARTICULAS 240
#define INTRO_DURACAO 15.5f
#define INTRO_AUDIO_FADE_DURACAO 5.0f

typedef enum {
    TELA_INTRO,
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

typedef struct {
    Vector2 posicao;
    Vector2 direcao;
    float velocidade;
    float raio;
    float profundidade;
    Color cor;
} ParticulaIntroGUI;

typedef struct {
    float tempo;
    int finalizada;
    TelaGUI destino;
    ParticulaIntroGUI particulas[INTRO_PARTICULAS];
} IntroGUI;

typedef enum {
    MUSICA_NENHUMA,
    MUSICA_MENU,
    MUSICA_INTRO,
    MUSICA_JOGO
} MusicaTemaGUI;

typedef struct {
    int audioPronto;
    int menuCarregada;
    int introCarregada;
    int jogoCarregada;
    float menuVolume;
    float introVolume;
    float jogoVolume;
    MusicaTemaGUI atual;
    Music menu;
    Music intro;
    Music jogo;
} AudioGUI;

static const Color COR_FUNDO = {5, 6, 8, 255};
static const Color COR_PAINEL = {16, 18, 22, 255};
static const Color COR_PAINEL_CLARO = {27, 30, 36, 255};
static const Color COR_TEXTO = {245, 247, 250, 255};
static const Color COR_TEXTO_FRACO = {178, 184, 194, 255};
static const Color COR_DESTAQUE = {255, 111, 0, 255};
static const Color COR_LARANJA_CLARO = {255, 173, 51, 255};
static const Color COR_PERGUNTA = {255, 149, 0, 255};
static const Color COR_ALERTA = {255, 191, 71, 255};
static const Color COR_ERRO = {230, 34, 34, 255};
static const Color COR_EDAG = {238, 27, 36, 255};
static const Color COR_TRILHA = {96, 58, 18, 255};
static const Color COR_AZUL_PERGUNTA = {11, 53, 114, 255};
static const Color COR_AZUL_ALTERNATIVA = {17, 66, 136, 255};
static const Color COR_AZUL_ALTERNATIVA_HOVER = {28, 94, 176, 255};

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
    DrawText("Caminho do conhecimento", 40, 30, 42, COR_TEXTO);
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
        return COR_EDAG;
    }

    if (tipo == PERGUNTA) {
        return COR_PERGUNTA;
    }

    return COR_LARANJA_CLARO;
}

static const char* nomeTipoCasa(TipoCasa tipo) {
    if (tipo == PRISAO) {
        return "EDAG";
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

static int quantidadePerguntasNivelGUI(NivelPergunta nivel) {
    return quantidadePerguntasRestantesNivel(nivel);
}

static Casa* buscarCasaGUI(Casa *inicio, int idCasa) {
    Casa *casa = inicio;

    while (casa != NULL) {
        if (casa->id == idCasa) {
            return casa;
        }

        casa = casa->prox;
    }

    return NULL;
}

static void desenharTextoQuebrado(const char *texto, Rectangle area, int tamanho, Color cor) {
    char palavra[160];
    char linha[1200] = "";
    int indice = 0;
    float y = area.y;
    float espacamentoLinha = tamanho + 8.0f;

    if (texto == NULL) {
        return;
    }

    for (int i = 0;; i++) {
        char c = texto[i];

        if (c != ' ' && c != '\0' && indice < (int)sizeof(palavra) - 1) {
            palavra[indice++] = c;
        }

        if (c == ' ' || c == '\0') {
            char teste[1400];
            palavra[indice] = '\0';

            if (linha[0] == '\0') {
                snprintf(teste, sizeof(teste), "%s", palavra);
            } else {
                snprintf(teste, sizeof(teste), "%s %s", linha, palavra);
            }

            if (MeasureText(teste, tamanho) > area.width && linha[0] != '\0') {
                DrawText(linha, (int)area.x, (int)y, tamanho, cor);
                y += espacamentoLinha;
                snprintf(linha, sizeof(linha), "%s", palavra);
            } else {
                strncpy(linha, teste, sizeof(linha) - 1);
                linha[sizeof(linha) - 1] = '\0';
            }

            indice = 0;

            if (c == '\0') {
                if (linha[0] != '\0' && y + tamanho <= area.y + area.height) {
                    DrawText(linha, (int)area.x, (int)y, tamanho, cor);
                }
                break;
            }

            if (y + tamanho > area.y + area.height) {
                break;
            }
        }
    }
}

static int encontrarPosicaoCasaGrid(int idCasa, int *linhaOut, int *colunaOut) {
    int ordem[6][6] = {
        {0, 1, 2, 3, 4, 5},
        {11, 10, 9, 8, 7, 6},
        {12, 13, 14, 15, 16, 17},
        {23, 22, 21, 20, 19, 18},
        {24, 25, 26, 27, 28, 29},
        {-1, -1, -1, -1, -1, 30}
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
        TAB_Y + linha * (TAB_TAM + TAB_GAP) + TAB_TAM / 2.0f
    };
}

static Vector2 interpolar(Vector2 a, Vector2 b, float t) {
    return (Vector2){
        a.x + (b.x - a.x) * t,
        a.y + (b.y - a.y) * t
    };
}

static void desenharTextoCentroCasa(const char *texto, Vector2 centro, int y, int tamanho, Color cor) {
    int larguraTexto = MeasureText(texto, tamanho);
    int x = (int)(centro.x - larguraTexto / 2.0f);

    DrawText(texto, x + 2, y + 2, tamanho, Fade(BLACK, 0.62f));
    DrawText(texto, x, y, tamanho, cor);
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

static float limitar01(float valor) {
    if (valor < 0.0f) {
        return 0.0f;
    }

    if (valor > 1.0f) {
        return 1.0f;
    }

    return valor;
}

static float intervaloIntro(float tempo, float inicio, float duracao) {
    if (duracao <= 0.0f) {
        return 1.0f;
    }

    return limitar01((tempo - inicio) / duracao);
}

static float suavizarIntro(float t) {
    t = limitar01(t);
    return t * t * (3.0f - 2.0f * t);
}

static void desenharTextoIntroCentralizado(const char *texto, float y, float tamanho, Color cor, float escala) {
    Vector2 medida = MeasureTextEx(fonteAtiva(), texto, tamanho * escala, 1.0f);
    DrawTextEx(
        fonteAtiva(),
        texto,
        (Vector2){(GetScreenWidth() - medida.x) / 2.0f, y},
        tamanho * escala,
        1.0f,
        cor
    );
}

static void encerrarIntro(IntroGUI *intro, TelaGUI *telaAtual) {
    if (intro == NULL || telaAtual == NULL || intro->finalizada) {
        return;
    }

    intro->finalizada = 1;

    *telaAtual = intro->destino;
}

static void reiniciarParticulaIntro(ParticulaIntroGUI *particula, int largura, int altura, int nascerNoCentro) {
    float angulo = ((float)(rand() % 6283) / 1000.0f);
    float distancia = nascerNoCentro ? (float)(rand() % 70) : (float)(rand() % 720);
    float profundidade = 0.25f + (float)(rand() % 100) / 100.0f;
    Vector2 centro = {(float)largura / 2.0f, (float)altura / 2.0f};
    int tipoCor = rand() % 8;

    particula->direcao = (Vector2){cosf(angulo), sinf(angulo)};
    particula->posicao = (Vector2){
        centro.x + particula->direcao.x * distancia,
        centro.y + particula->direcao.y * distancia
    };
    particula->profundidade = profundidade;
    particula->velocidade = 18.0f + profundidade * 185.0f + (float)(rand() % 30);
    particula->raio = 0.7f + profundidade * 1.65f;

    if (tipoCor <= 3) {
        particula->cor = COR_ALERTA;
    }
    else if (tipoCor <= 6) {
        particula->cor = COR_DESTAQUE;
    }
    else {
        particula->cor = (Color){255, 224, 154, 255};
    }
}

static void InitIntro(IntroGUI *intro) {
    if (intro == NULL) {
        return;
    }

    memset(intro, 0, sizeof(*intro));
    intro->finalizada = 1;
    intro->destino = TELA_JOGO;

    for (int i = 0; i < INTRO_PARTICULAS; i++) {
        reiniciarParticulaIntro(&intro->particulas[i], 1200, 800, 0);
    }
}

static void IniciarIntro(IntroGUI *intro, TelaGUI destino) {
    if (intro == NULL) {
        return;
    }

    intro->tempo = 0.0f;
    intro->finalizada = 0;
    intro->destino = destino;

    for (int i = 0; i < INTRO_PARTICULAS; i++) {
        reiniciarParticulaIntro(&intro->particulas[i], GetScreenWidth(), GetScreenHeight(), 0);
    }

}

static int IntroTerminou(IntroGUI *intro) {
    return intro != NULL && intro->tempo >= INTRO_DURACAO;
}

static void UpdateIntro(IntroGUI *intro, float dt, TelaGUI *telaAtual) {
    if (intro == NULL || telaAtual == NULL || intro->finalizada) {
        return;
    }

    intro->tempo += dt;

    if (
        IsKeyPressed(KEY_ENTER) ||
        IsKeyPressed(KEY_SPACE) ||
        IsMouseButtonPressed(MOUSE_LEFT_BUTTON) ||
        IntroTerminou(intro)
    ) {
        encerrarIntro(intro, telaAtual);
    }
}

static void DrawIntro(IntroGUI *intro) {
    float tempo = intro != NULL ? intro->tempo : 0.0f;
    float brilho = suavizarIntro(intervaloIntro(tempo, 0.25f, 1.2f));
    float fadeSaida = intervaloIntro(tempo, INTRO_DURACAO - 1.0f, 1.0f);
    const char *linhasIntro[] = {
        "Em uma sala de aula muito distante...",
        "",
        "Os alunos sao desafiados pelo Lorde Soussa",
        "a completarem o tabuleiro.",
        "",
        "Boa sorte a todos."
    };
    const int totalLinhasIntro = 6;

    if (intro != NULL) {
        for (int i = 0; i < INTRO_PARTICULAS; i++) {
            ParticulaIntroGUI p = intro->particulas[i];
            float alpha = 0.22f + p.profundidade * 0.58f;

            DrawCircleV(p.posicao, p.raio, Fade(p.cor, alpha));

            if (p.raio > 1.8f) {
                DrawCircleV(p.posicao, p.raio * 2.2f, Fade(p.cor, alpha * 0.08f));
            }
        }
    }

    DrawCircleGradient(
        GetScreenWidth() / 2,
        GetScreenHeight() / 2,
        190.0f + sinf(tempo * 1.5f) * 12.0f,
        Fade(COR_DESTAQUE, 0.18f * brilho),
        Fade(COR_FUNDO, 0.0f)
    );

    // Crawl em perspectiva: as linhas sobem e reduzem a escala ao se afastarem.
    {
        float progresso = suavizarIntro(intervaloIntro(tempo, 0.75f, INTRO_DURACAO - 1.8f));
        float baseY = 790.0f - progresso * 760.0f;

        for (int i = 0; i < totalLinhasIntro; i++) {
            float y = baseY + i * 68.0f;
            float profundidadeTela = limitar01((y - 60.0f) / 660.0f);
            float escala = 0.54f + profundidadeTela * 0.86f;
            float alphaEntrada = intervaloIntro(tempo, 0.75f, 0.8f);
            float alphaTopo = limitar01((y - 34.0f) / 150.0f);
            float alphaBase = limitar01((GetScreenHeight() + 60.0f - y) / 180.0f);
            float alpha = alphaEntrada * alphaTopo * alphaBase * (1.0f - fadeSaida);
            float tamanho = (i == 0 || i == totalLinhasIntro - 1) ? 34.0f : 30.0f;

            if (linhasIntro[i][0] == '\0') {
                continue;
            }

            desenharTextoIntroCentralizado(
                linhasIntro[i],
                y,
                tamanho,
                Fade(COR_ALERTA, alpha),
                escala
            );
        }
    }

    desenharTextoIntroCentralizado("ENTER, ESPACO ou clique para pular", 742.0f, 16.0f, Fade(COR_TEXTO_FRACO, 0.55f), 1.0f);

    if (tempo < 0.65f) {
        DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), Fade(BLACK, 1.0f - tempo / 0.65f));
    }

    if (fadeSaida > 0.0f) {
        DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), Fade(BLACK, fadeSaida));
    }
}

static void UnloadIntro(IntroGUI *intro) {
    if (intro == NULL) {
        return;
    }

    intro->finalizada = 1;
}

static int carregarMusicaGUI(Music *musica, const char *arquivo, float volume) {
    if (musica == NULL || arquivo == NULL || !FileExists(arquivo)) {
        return 0;
    }

    *musica = LoadMusicStream(arquivo);

    if (!IsMusicValid(*musica)) {
        return 0;
    }

    SetMusicVolume(*musica, volume);
    return 1;
}

static void InitAudioGUI(AudioGUI *audio) {
    if (audio == NULL) {
        return;
    }

    memset(audio, 0, sizeof(*audio));
    audio->atual = MUSICA_NENHUMA;
    audio->menuVolume = 0.58f;
    audio->introVolume = 0.72f;
    audio->jogoVolume = 0.54f;

    InitAudioDevice();
    audio->audioPronto = IsAudioDeviceReady();

    if (!audio->audioPronto) {
        return;
    }

    audio->menuCarregada = carregarMusicaGUI(&audio->menu, "assets/menu_theme.mp3", audio->menuVolume);
    audio->introCarregada = carregarMusicaGUI(&audio->intro, "assets/intro_theme.mp3", audio->introVolume);
    audio->jogoCarregada = carregarMusicaGUI(&audio->jogo, "assets/game_theme.mp3", audio->jogoVolume);
}

static Music* musicaAtualGUI(AudioGUI *audio) {
    if (audio == NULL) {
        return NULL;
    }

    if (audio->atual == MUSICA_MENU && audio->menuCarregada) {
        return &audio->menu;
    }

    if (audio->atual == MUSICA_INTRO && audio->introCarregada) {
        return &audio->intro;
    }

    if (audio->atual == MUSICA_JOGO && audio->jogoCarregada) {
        return &audio->jogo;
    }

    return NULL;
}

static int musicaTemaCarregadaGUI(AudioGUI *audio, MusicaTemaGUI tema) {
    if (audio == NULL || !audio->audioPronto) {
        return 0;
    }

    if (tema == MUSICA_MENU) {
        return audio->menuCarregada;
    }

    if (tema == MUSICA_INTRO) {
        return audio->introCarregada;
    }

    if (tema == MUSICA_JOGO) {
        return audio->jogoCarregada;
    }

    return 0;
}

static Music* ponteiroMusicaTemaGUI(AudioGUI *audio, MusicaTemaGUI tema) {
    if (audio == NULL) {
        return NULL;
    }

    if (tema == MUSICA_MENU) {
        return &audio->menu;
    }

    if (tema == MUSICA_INTRO) {
        return &audio->intro;
    }

    if (tema == MUSICA_JOGO) {
        return &audio->jogo;
    }

    return NULL;
}

static float volumeTemaGUI(AudioGUI *audio, MusicaTemaGUI tema) {
    if (audio == NULL) {
        return 0.0f;
    }

    if (tema == MUSICA_MENU) {
        return audio->menuVolume;
    }

    if (tema == MUSICA_INTRO) {
        return audio->introVolume;
    }

    if (tema == MUSICA_JOGO) {
        return audio->jogoVolume;
    }

    return 0.0f;
}

static void tocarMusicaGUI(AudioGUI *audio, MusicaTemaGUI tema) {
    Music *musicaAtual;
    Music *proximaMusica;

    if (audio == NULL || !audio->audioPronto || audio->atual == tema) {
        return;
    }

    musicaAtual = musicaAtualGUI(audio);

    if (musicaAtual != NULL) {
        StopMusicStream(*musicaAtual);
    }

    audio->atual = MUSICA_NENHUMA;

    if (!musicaTemaCarregadaGUI(audio, tema)) {
        return;
    }

    proximaMusica = ponteiroMusicaTemaGUI(audio, tema);

    if (proximaMusica == NULL) {
        return;
    }

    SetMusicVolume(*proximaMusica, volumeTemaGUI(audio, tema));
    PlayMusicStream(*proximaMusica);
    audio->atual = tema;
}

static void AtualizarAudioGUI(AudioGUI *audio, IntroGUI *intro) {
    Music *musica;

    if (audio == NULL || !audio->audioPronto) {
        return;
    }

    musica = musicaAtualGUI(audio);

    if (musica != NULL) {
        UpdateMusicStream(*musica);

        if (audio->atual == MUSICA_INTRO && intro != NULL) {
            float fade = 1.0f - intervaloIntro(
                intro->tempo,
                INTRO_DURACAO - INTRO_AUDIO_FADE_DURACAO,
                INTRO_AUDIO_FADE_DURACAO
            );

            SetMusicVolume(*musica, audio->introVolume * limitar01(fade));
        }
    }
}

static void sincronizarMusicaTelaGUI(AudioGUI *audio, TelaGUI telaAtual) {
    if (telaAtual == TELA_INTRO) {
        tocarMusicaGUI(audio, MUSICA_INTRO);
    }
    else if (
        telaAtual == TELA_JOGO ||
        telaAtual == TELA_ESCOLHER_DIFICULDADE ||
        telaAtual == TELA_PERGUNTA
    ) {
        tocarMusicaGUI(audio, MUSICA_JOGO);
    }
    else {
        tocarMusicaGUI(audio, MUSICA_MENU);
    }
}

static void UnloadAudioGUI(AudioGUI *audio) {
    if (audio == NULL || !audio->audioPronto) {
        return;
    }

    if (audio->menuCarregada) {
        UnloadMusicStream(audio->menu);
    }

    if (audio->introCarregada) {
        UnloadMusicStream(audio->intro);
    }

    if (audio->jogoCarregada) {
        UnloadMusicStream(audio->jogo);
    }

    CloseAudioDevice();
    audio->audioPronto = 0;
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

static void copiarTextoTabelaGUI(const char *origem, char *destino, int tamanhoDestino, int limiteCaracteres) {
    int tamanhoOrigem;

    if (destino == NULL || tamanhoDestino <= 0) {
        return;
    }

    destino[0] = '\0';

    if (origem == NULL) {
        return;
    }

    tamanhoOrigem = (int)strlen(origem);

    if (tamanhoOrigem <= limiteCaracteres || limiteCaracteres < 4) {
        strncpy(destino, origem, tamanhoDestino - 1);
        destino[tamanhoDestino - 1] = '\0';
        return;
    }

    if (limiteCaracteres >= tamanhoDestino) {
        limiteCaracteres = tamanhoDestino - 1;
    }

    strncpy(destino, origem, limiteCaracteres - 3);
    destino[limiteCaracteres - 3] = '\0';
    strncat(destino, "...", tamanhoDestino - strlen(destino) - 1);
}

static void desenharTabelaRankingGeralGUI(int x, int y) {
    LinhaRankingGUI linhas[MAX_RANKING_GUI];
    int quantidade = carregarRankingGeralGUI(linhas, MAX_RANKING_GUI);
    const int itensPorColuna = 6;
    const int larguraColuna = 535;
    const int alturaLinha = 23;

    DrawText("Ranking geral", x, y, 24, COR_TEXTO);

    if (quantidade == 0) {
        DrawText("Nenhuma partida registrada ainda.", x, y + 42, 18, COR_TEXTO_FRACO);
        return;
    }

    for (int coluna = 0; coluna < 2; coluna++) {
        int inicio = coluna * itensPorColuna;
        int cx = x + coluna * larguraColuna;

        if (inicio >= quantidade) {
            continue;
        }

        DrawText("Nome", cx, y + 42, 15, COR_ALERTA);
        DrawText("A", cx + 195, y + 42, 15, COR_ALERTA);
        DrawText("E", cx + 255, y + 42, 15, COR_ALERTA);
        DrawText("Pontos", cx + 315, y + 42, 15, COR_ALERTA);
        DrawText("Part.", cx + 405, y + 42, 15, COR_ALERTA);

        for (int local = 0; local < itensPorColuna; local++) {
            int i = inicio + local;
            int linhaY = y + 68 + local * alturaLinha;
            char buffer[40];
            char nome[32];

            if (i >= quantidade) {
                break;
            }

            copiarTextoTabelaGUI(linhas[i].nome, nome, sizeof(nome), 20);

            DrawText(nome, cx, linhaY, 15, COR_TEXTO);
            snprintf(buffer, sizeof(buffer), "%d", linhas[i].acertos);
            DrawText(buffer, cx + 195, linhaY, 15, COR_TEXTO_FRACO);
            snprintf(buffer, sizeof(buffer), "%d", linhas[i].erros);
            DrawText(buffer, cx + 255, linhaY, 15, COR_TEXTO_FRACO);
            snprintf(buffer, sizeof(buffer), "%d", linhas[i].pontos);
            DrawText(buffer, cx + 315, linhaY, 15, COR_TEXTO_FRACO);
            snprintf(buffer, sizeof(buffer), "%d", linhas[i].partidas);
            DrawText(buffer, cx + 405, linhaY, 15, COR_TEXTO_FRACO);
        }
    }
}

static void desenharAnaliseTemaGUI(int x, int y) {
    TemaResumoGUI temas[MAX_TEMAS_GUI];
    int quantidade = carregarAnaliseTemaGUI(temas, MAX_TEMAS_GUI);
    const int itensPorColuna = 6;
    const int larguraColuna = 535;
    const int alturaLinha = 23;

    DrawText("Analise por tema", x, y, 24, COR_TEXTO);

    if (quantidade == 0) {
        DrawText("Nenhuma resposta registrada ainda.", x, y + 42, 18, COR_TEXTO_FRACO);
        return;
    }

    for (int coluna = 0; coluna < 2; coluna++) {
        int inicio = coluna * itensPorColuna;
        int cx = x + coluna * larguraColuna;

        if (inicio >= quantidade) {
            continue;
        }

        DrawText("Tema", cx, y + 42, 15, COR_ALERTA);
        DrawText("A", cx + 250, y + 42, 15, COR_ALERTA);
        DrawText("E", cx + 305, y + 42, 15, COR_ALERTA);
        DrawText("Total", cx + 360, y + 42, 15, COR_ALERTA);
        DrawText("%", cx + 435, y + 42, 15, COR_ALERTA);

        for (int local = 0; local < itensPorColuna; local++) {
            int i = inicio + local;
            int linhaY = y + 68 + local * alturaLinha;
            int total;
            double aproveitamento;
            char buffer[50];
            char tema[36];

            if (i >= quantidade) {
                break;
            }

            total = temas[i].acertos + temas[i].erros;
            aproveitamento = total > 0 ? (temas[i].acertos * 100.0) / total : 0.0;
            copiarTextoTabelaGUI(temas[i].tema, tema, sizeof(tema), 26);

            DrawText(tema, cx, linhaY, 15, COR_TEXTO);
            snprintf(buffer, sizeof(buffer), "%d", temas[i].acertos);
            DrawText(buffer, cx + 250, linhaY, 15, COR_TEXTO_FRACO);
            snprintf(buffer, sizeof(buffer), "%d", temas[i].erros);
            DrawText(buffer, cx + 305, linhaY, 15, COR_TEXTO_FRACO);
            snprintf(buffer, sizeof(buffer), "%d", total);
            DrawText(buffer, cx + 360, linhaY, 15, COR_TEXTO_FRACO);
            snprintf(buffer, sizeof(buffer), "%.1f", aproveitamento);
            DrawText(buffer, cx + 435, linhaY, 15, COR_TEXTO_FRACO);
        }
    }
}

static void desenharTabuleiroGUI(
    Casa *inicio,
    JogadorGUI jogadores[],
    int qtdJogadores,
    int jogadorAtual,
    AnimacaoGUI *animacao
) {
    int jogadorPulando = (animacao != NULL && animacao->movimentoAtivo) ? animacao->jogador : -1;

    DrawRectangleRounded((Rectangle){24, 186, 666, 606}, 0.04f, 12, COR_PAINEL);

    // A trilha conecta as casas em ordem, deixando o tabuleiro com cara de percurso.
    for (int id = 0; id < CASA_FINAL; id++) {
        Vector2 a = centroCasaTabuleiro(id);
        Vector2 b = centroCasaTabuleiro(id + 1);

        DrawLineEx(a, b, 14.0f, Fade(COR_TRILHA, 0.82f));
        DrawLineEx(a, b, 5.0f, Fade(COR_DESTAQUE, 0.44f));
    }

    for (int id = 0; id <= CASA_FINAL; id++) {
        Casa *casa = buscarCasaGUI(inicio, id);
        Vector2 centro = centroCasaTabuleiro(id);
        Color corBase;
        int ocupantes = 0;
        int casaAtualDoJogador = 0;
        char idTexto[8];

        if (casa == NULL) {
            continue;
        }

        corBase = corCasa(casa->tipo);

        for (int j = 0; j < qtdJogadores; j++) {
            if (jogadores[j].casaAtual != NULL && jogadores[j].casaAtual->id == casa->id) {
                if (j == jogadorAtual) {
                    casaAtualDoJogador = 1;
                }
                ocupantes++;
            }
        }

        DrawCircleV((Vector2){centro.x + 4, centro.y + 6}, casaAtualDoJogador ? 38 : 34, Fade(BLACK, 0.34f));
        DrawCircleV(centro, casaAtualDoJogador ? 38 : 34, casaAtualDoJogador ? COR_TEXTO : Fade(COR_TEXTO, 0.16f));
        DrawCircleV(centro, casaAtualDoJogador ? 33 : 30, corBase);
        DrawCircleLines((int)centro.x, (int)centro.y, casaAtualDoJogador ? 33 : 30, Fade(BLACK, 0.55f));

        snprintf(idTexto, sizeof(idTexto), "%02d", casa->id);
        desenharTextoCentroCasa(idTexto, centro, (int)centro.y - 24, 24, WHITE);
        desenharTextoCentroCasa(nomeTipoCasa(casa->tipo), centro, (int)centro.y + 4, 13, WHITE);

        if (ocupantes > 0) {
            int desenhados = 0;

            for (int j = 0; j < qtdJogadores; j++) {
                if (j == jogadorPulando) {
                    continue;
                }

                if (jogadores[j].casaAtual != NULL && jogadores[j].casaAtual->id == casa->id) {
                    float px = centro.x - 18.0f + desenhados * 15.0f;
                    float py = centro.y + 29.0f;
                    desenharPeao((Vector2){px, py}, jogadores[j].cor, j == jogadorAtual);
                    desenhados++;
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
    Rectangle painel = {720, 230, 420, 265};
    DrawRectangleRounded(painel, 0.04f, 8, COR_PAINEL);
    DrawText("Jogadores", 744, 250, 24, COR_TEXTO);

    char grupoTexto[80];
    snprintf(grupoTexto, sizeof(grupoTexto), "Grupo do historico: %s", grupo);
    DrawText(grupoTexto, 744, 282, 15, COR_TEXTO_FRACO);

    for (int i = 0; i < qtdJogadores; i++) {
        int y = 315 + i * 42;
        Color linha = (i == jogadorAtual) ? COR_PAINEL_CLARO : Fade(COR_PAINEL_CLARO, 0.42f);
        DrawRectangleRounded((Rectangle){744, (float)y, 360, 34}, 0.08f, 8, linha);
        DrawCircle(762, y + 17, 8, jogadores[i].cor);
        DrawText(jogadores[i].nome, 778, y + 4, 17, COR_TEXTO);

        char dados[160];
        snprintf(
            dados,
            sizeof(dados),
            "Casa %02d | A:%d E:%d P:%d",
            jogadores[i].posicaoAtual,
            jogadores[i].acertos,
            jogadores[i].erros,
            jogadores[i].pontuacao
        );
        DrawText(dados, 778, y + 21, 12, COR_TEXTO_FRACO);
    }
}

static void desenharRankingFinalGUI(JogadorGUI jogadores[], int qtdJogadores, int x, int y) {
    tp_item ranking[MAX_GUI_JOGADORES];

    DrawText("Ranking da partida", x, y, 24, COR_TEXTO);
    DrawText("Pos  Nome              A  E  Pontos", x, y + 36, 15, COR_ALERTA);

    for (int i = 0; i < qtdJogadores; i++) {
        ranking[i] = converterJogadorGUI(jogadores[i], i + 1);
    }

    ordenarJogadoresPartida(ranking, qtdJogadores);

    for (int i = 0; i < qtdJogadores; i++) {
        char linha[140];
        snprintf(
            linha,
            sizeof(linha),
            "%d    %-16s %d  %d  %d",
            i + 1,
            ranking[i].nome,
            ranking[i].acertos,
            ranking[i].erros,
            ranking[i].pontuacao
        );
        DrawText(linha, x, y + 66 + i * 30, 18, COR_TEXTO_FRACO);
    }
}

static void desenharQuedasCasaGUI(Casa *inicio, NoCasa *arvoreQuedas, int x, int y) {
    DrawText("Casas visitadas", x, y, 24, COR_TEXTO);
    DrawText("Casa  Tipo       Qtd", x, y + 34, 15, COR_ALERTA);

    for (int id = 0; id <= CASA_FINAL; id++) {
        int coluna = id / 11;
        int linha = id % 11;
        int px = x + coluna * 205;
        int py = y + 62 + linha * 22;
        Casa *casa = buscarCasaGUI(inicio, id);
        int quedas = quantidadeQuedasCasa(arvoreQuedas, id);
        char texto[80];
        Color corTipo = COR_TEXTO_FRACO;

        if (casa == NULL) {
            continue;
        }

        if (casa->tipo == PRISAO) {
            corTipo = COR_EDAG;
        }
        else if (casa->tipo == PERGUNTA) {
            corTipo = COR_PERGUNTA;
        }

        snprintf(texto, sizeof(texto), "%02d    %-8s   %d", id, nomeTipoCasa(casa->tipo), quedas);
        DrawText(texto, px, py, 14, corTipo);
    }
}

static void desenharBaralhoDificuldade(Rectangle area, const char *titulo, NivelPergunta nivel, Color corBase) {
    char qtdTexto[40];
    int quantidade = quantidadePerguntasNivelGUI(nivel);
    int semCartas = quantidade <= 0;

    snprintf(qtdTexto, sizeof(qtdTexto), "%d cartas", quantidade);

    if (semCartas) {
        corBase = COR_PAINEL_CLARO;
        snprintf(qtdTexto, sizeof(qtdTexto), "Sem cartas");
    }

    DrawRectangleRounded((Rectangle){area.x + 14, area.y - 14, area.width, area.height}, 0.08f, 8, Fade(corBase, 0.34f));
    DrawRectangleRounded((Rectangle){area.x + 7, area.y - 7, area.width, area.height}, 0.08f, 8, Fade(corBase, 0.58f));
    DrawRectangleRounded(area, 0.08f, 8, corBase);
    DrawRectangleRoundedLines(area, 0.08f, 8, Fade(WHITE, 0.38f));

    DrawText(titulo, (int)area.x + 24, (int)area.y + 32, 30, semCartas ? COR_TEXTO_FRACO : COR_FUNDO);
    DrawText(qtdTexto, (int)area.x + 24, (int)area.y + 78, 20, semCartas ? COR_TEXTO_FRACO : Fade(COR_FUNDO, 0.78f));
    DrawText(semCartas ? "Indisponivel" : "Selecionar", (int)area.x + 24, (int)area.y + 132, 18, semCartas ? COR_TEXTO_FRACO : COR_FUNDO);
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

static void desenharTelaMenu(TelaGUI *telaAtual, Texture2D *patoMenu) {
    int centroX = GetScreenWidth() / 2;
    Rectangle painel = {centroX - 245.0f, 125, 490, 430};

    DrawRectangleRounded(painel, 0.06f, 12, COR_PAINEL);
    DrawRectangleRoundedLines(painel, 0.06f, 12, Fade(COR_DESTAQUE, 0.45f));

    desenharTextoCentralizado("Caminho do conhecimento", 165, 48, COR_TEXTO);
    desenharTextoCentralizado("ARSENAI", 218, 22, COR_DESTAQUE);
    desenharTextoCentralizado("Boas Vindas", 258, 24, COR_TEXTO_FRACO);

    BotaoGUI iniciar = {{centroX - 150.0f, 315, 300, 60}, "Iniciar jogo"};
    BotaoGUI ranking = {{centroX - 150.0f, 390, 300, 60}, "Ver ranking"};
    BotaoGUI sair = {{centroX - 150.0f, 465, 300, 60}, "Sair"};

    desenharBotao(iniciar, COR_DESTAQUE);
    desenharBotao(ranking, COR_PAINEL_CLARO);
    desenharBotao(sair, COR_ERRO);

    desenharTextoCentralizado("Use o mouse para jogar, rolar o dado e responder perguntas.", 590, 20, COR_TEXTO_FRACO);

    if (patoMenu != NULL && patoMenu->id != 0) {
        Rectangle origem = {0, 0, (float)patoMenu->width, (float)patoMenu->height};
        Rectangle destino = {28, 612, 150, 150};

        DrawTexturePro(*patoMenu, origem, destino, (Vector2){0, 0}, 0.0f, WHITE);
        DrawRectangleRoundedLines(destino, 0.04f, 8, Fade(COR_DESTAQUE, 0.65f));
    }

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

    IntroGUI intro;
    InitIntro(&intro);

    AudioGUI audio;
    InitAudioGUI(&audio);

    Texture2D patoMenu = {0};
    if (FileExists("assets/patotv.jpg")) {
        patoMenu = LoadTexture("assets/patotv.jpg");

        if (patoMenu.id != 0) {
            SetTextureFilter(patoMenu, TEXTURE_FILTER_BILINEAR);
        }
    }

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

        if (telaAtual == TELA_INTRO) {
            UpdateIntro(&intro, dt, &telaAtual);
        }
        else if (atrasoAcao > 0.0f) {
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

        if (telaAtual != TELA_INTRO && atrasoAcao <= 0.0f) {
            movimentoFinalizado = atualizarAnimacoesGUI(
                &animacao,
                jogadores,
                &arvoreQuedas,
                dt,
                &dadoAtual
            );
        }

        if (telaAtual != TELA_INTRO && movimentoFinalizado != MOVIMENTO_NENHUM) {
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
                    snprintf(mensagem, sizeof(mensagem), "%s caiu no EDAG e perde a proxima rodada.", j->nome);
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

        sincronizarMusicaTelaGUI(&audio, telaAtual);
        AtualizarAudioGUI(&audio, &intro);

        BeginDrawing();
        ClearBackground(COR_FUNDO);

        if (telaAtual == TELA_INTRO) {
            DrawIntro(&intro);
        }
        else if (telaAtual == TELA_MENU) {
            desenharTelaMenu(&telaAtual, &patoMenu);
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
                    reiniciarPerguntasJogo();
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

                    IniciarIntro(&intro, TELA_JOGO);
                    telaAtual = TELA_INTRO;
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
                    snprintf(mensagem, sizeof(mensagem), "%s perdeu a rodada por causa do EDAG.", j->nome);
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
            Rectangle facil = {205, 320, 230, 190};
            Rectangle medio = {485, 320, 230, 190};
            Rectangle dificil = {765, 320, 230, 190};
            Vector2 mouse = GetMousePosition();

            desenharTextoCentralizado("Escolha um baralho", 125, 42, COR_TEXTO);
            desenharTextoCentralizado(jogadores[jogadorAtual].nome, 178, 24, COR_TEXTO_FRACO);
            desenharTextoCentralizado(mensagem, 570, 18, COR_TEXTO_FRACO);

            desenharBaralhoDificuldade(facil, "Facil", FACIL, (Color){255, 177, 67, 255});
            desenharBaralhoDificuldade(medio, "Medio", MEDIO, (Color){255, 130, 0, 255});
            desenharBaralhoDificuldade(dificil, "Dificil", DIFICIL, (Color){230, 60, 34, 255});

            if (CheckCollisionPointRec(mouse, facil) || CheckCollisionPointRec(mouse, medio) || CheckCollisionPointRec(mouse, dificil)) {
                DrawCircleV(mouse, 5, COR_TEXTO);
            }

            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && (
                CheckCollisionPointRec(mouse, facil) ||
                CheckCollisionPointRec(mouse, medio) ||
                CheckCollisionPointRec(mouse, dificil)
            )) {
                if (CheckCollisionPointRec(mouse, facil)) {
                    nivelEscolhido = FACIL;
                }
                else if (CheckCollisionPointRec(mouse, medio)) {
                    nivelEscolhido = MEDIO;
                }
                else {
                    nivelEscolhido = DIFICIL;
                }

                int indicePergunta = consumirIndicePerguntaNivel(nivelEscolhido);
                const Questao *questao = obterQuestaoPorIndice(indicePergunta);

                if (questao != NULL) {
                    questaoAtual = *questao;
                    temQuestaoAtual = 1;
                    telaAtual = TELA_PERGUNTA;
                } else {
                    snprintf(mensagem, sizeof(mensagem), "Nao ha cartas %s disponiveis.", nomeNivelGUI(nivelEscolhido));
                    adicionarAcaoHistorico(&historicoAcoes, mensagem);
                }
            }
        }
        else if (telaAtual == TELA_PERGUNTA) {
            if (temQuestaoAtual) {
                Rectangle painelPergunta = {150, 95, 900, 610};
                Rectangle areaEnunciado = {195, 175, 810, 130};

                DrawRectangleRounded(painelPergunta, 0.04f, 12, COR_AZUL_PERGUNTA);
                DrawRectangleRoundedLines(painelPergunta, 0.04f, 12, Fade(COR_TEXTO, 0.28f));
                desenharTextoCentralizado("Pergunta", 123, 34, COR_TEXTO);
                desenharTextoCentralizado(nomeNivelGUI(nivelEscolhido), 160, 18, COR_ALERTA);
                desenharTextoQuebrado(questaoAtual.enunciado, areaEnunciado, 23, COR_TEXTO);

                for (int i = 0; i < 4; i++) {
                    Rectangle area = {195, (float)(330 + i * 82), 810, 62};
                    char textoAlternativa[560];
                    Vector2 mouse = GetMousePosition();
                    int hover = CheckCollisionPointRec(mouse, area);

                    snprintf(textoAlternativa, sizeof(textoAlternativa), "%c) %s", 'A' + i, questaoAtual.alternativas[i]);

                    DrawRectangleRounded(area, 0.08f, 10, hover ? COR_AZUL_ALTERNATIVA_HOVER : COR_AZUL_ALTERNATIVA);
                    DrawRectangleRoundedLines(area, 0.08f, 10, hover ? COR_DESTAQUE : Fade(WHITE, 0.14f));
                    desenharTextoQuebrado(textoAlternativa, (Rectangle){area.x + 18, area.y + 13, area.width - 36, area.height - 14}, 18, COR_TEXTO);

                    if (hover && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
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

            DrawRectangleRounded((Rectangle){40, 155, 1120, 250}, 0.04f, 10, COR_PAINEL);
            DrawRectangleRoundedLines((Rectangle){40, 155, 1120, 250}, 0.04f, 10, Fade(COR_DESTAQUE, 0.28f));
            desenharTabelaRankingGeralGUI(70, 178);

            DrawRectangleRounded((Rectangle){40, 430, 1120, 250}, 0.04f, 10, COR_PAINEL);
            DrawRectangleRoundedLines((Rectangle){40, 430, 1120, 250}, 0.04f, 10, Fade(COR_DESTAQUE, 0.28f));
            desenharAnaliseTemaGUI(70, 453);

            BotaoGUI voltar = {{930, 710, 220, 50}, "Voltar"};
            desenharBotao(voltar, COR_DESTAQUE);
            if (botaoClicado(voltar)) {
                telaAtual = TELA_MENU;
            }
        }
        else if (telaAtual == TELA_FIM) {
            desenharTextoCentralizado("Fim de jogo", 42, 42, COR_TEXTO);
            desenharTextoCentralizado(jogoFinalizado ? "Partida salva nos arquivos CSV." : "", 92, 20, COR_TEXTO_FRACO);

            if (indiceVencedor >= 0) {
                char vencedor[120];
                snprintf(vencedor, sizeof(vencedor), "Vencedor: %s", jogadores[indiceVencedor].nome);
                desenharTextoCentralizado(vencedor, 132, 28, COR_DESTAQUE);
            }

            DrawRectangleRounded((Rectangle){48, 195, 420, 260}, 0.05f, 10, COR_PAINEL);
            DrawRectangleRoundedLines((Rectangle){48, 195, 420, 260}, 0.05f, 10, Fade(COR_DESTAQUE, 0.32f));
            desenharRankingFinalGUI(jogadores, qtdJogadores, 76, 222);

            DrawRectangleRounded((Rectangle){492, 195, 660, 380}, 0.05f, 10, COR_PAINEL);
            DrawRectangleRoundedLines((Rectangle){492, 195, 660, 380}, 0.05f, 10, Fade(COR_DESTAQUE, 0.32f));
            desenharQuedasCasaGUI(inicioTabuleiro, arvoreQuedas, 520, 222);

            DrawText("Arquivos atualizados:", 76, 492, 20, COR_TEXTO);
            DrawText("historico_respostas.csv  |  ranking_partidas.csv  |  ranking_geral.csv", 76, 525, 16, COR_TEXTO_FRACO);
            DrawText("quedas_casas.csv  |  perguntas.csv", 76, 551, 16, COR_TEXTO_FRACO);

            BotaoGUI voltar = {{480, 665, 240, 58}, "Menu inicial"};
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
    UnloadIntro(&intro);
    UnloadAudioGUI(&audio);
    if (patoMenu.id != 0) {
        UnloadTexture(patoMenu);
    }
    if (fonteUICarregada) {
        UnloadFont(fonteUI);
    }
    CloseWindow();

    return 0;
}
