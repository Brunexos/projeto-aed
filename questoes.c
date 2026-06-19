#include "questoes.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <conio.h>

// Cores
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

#define QTD_FACEIS 4
#define QTD_MEDIAS 4
#define QTD_DIFICEIS 4
#define TOTAL_PERGUNTAS 12

void posicionar(int x, int y) {
    printf("\033[%d;%dH", y, x);
}

// Banco geral de perguntas
// FACEIS:   indices 0 a 3
// MEDIAS:   indices 4 a 7
// DIFICIES: indices 8 a 11
Questao bancoPerguntas[TOTAL_PERGUNTAS] = {
    // =====================================================
    // FACEIS - indices 0 a 3
    // =====================================================
    {
        1,
        "1",
        "Tipos de Dados",
        "Inteiros",
        "Facil",
        "Para que serve declarar int?",
        {"Reais", "Palavras", "Inteiros", "Imprimir"},
        2
    },
    {
        2,
        "1",
        "Pilha",
        "LIFO",
        "Facil",
        "Elemento sai de onde na pilha?",
        {"Baixo", "Cima", "Meio", "Aleatorio"},
        1
    },
    {
        3,
        "2",
        "Lista Encadeada",
        "Estrutura do no",
        "Facil",
        "Qual e a principal caracteristica de uma lista simplesmente encadeada?",
        {
            "O tamanho da lista e fixo e deve ser definido no momento de sua criacao.",
            "Os elementos sao armazenados em posicoes contiguas de memoria.",
            "Cada no contem um valor e um ponteiro para o proximo no.",
            "Cada no possui ponteiros para o anterior e para o proximo."
        },
        2
    },
    {
        4,
        "2",
        "Lista Duplamente Encadeada",
        "Ponteiro anterior",
        "Facil",
        "Em uma lista duplamente encadeada padrao, para onde aponta o ponteiro anterior do primeiro no?",
        {
            "Para si mesmo.",
            "Para o ultimo no da lista.",
            "Para o segundo no da lista.",
            "Para nulo."
        },
        3
    },

    // =====================================================
    // MEDIAS - indices 4 a 7
    // =====================================================
    {
        5,
        "1",
        "Pilha",
        "Topo",
        "Medio",
        "Qual o valor do topo inicial de uma pilha vazia?",
        {"1", "-1", "0", "2"},
        1
    },
    {
        6,
        "1",
        "Ponteiros",
        "Ponteiro para ponteiro",
        "Medio",
        "O que int **p armazena?",
        {"Valor comum", "Endereco de ponteiro", "Texto", "Erro sempre"},
        1
    },
    {
        7,
        "2",
        "Lista Encadeada",
        "Insercao no inicio",
        "Medio",
        "Qual e a complexidade para inserir um no no inicio de uma lista simplesmente encadeada, tendo referencia para a cabeca?",
        {
            "O(n^2)",
            "O(n)",
            "O(1)",
            "O(log n)"
        },
        2
    },
    {
        8,
        "2",
        "Lista Duplamente Encadeada",
        "Remocao",
        "Medio",
        "Ao remover um no do meio de uma lista duplamente encadeada, quantos ponteiros de nos vizinhos precisam ser alterados?",
        {
            "4 ou mais ponteiros",
            "Exatamente 2 ponteiros",
            "Apenas 1 ponteiro",
            "Exatamente 3 ponteiros"
        },
        1
    },

    // =====================================================
    // DIFICIES - indices 8 a 11
    // =====================================================
    {
        9,
        "1",
        "Ponteiros",
        "Ponteiro para funcao",
        "Dificil",
        "Sintaxe correta de ponteiro para funcao?",
        {"void *f(int);", "void (*ptr)(int);", "func ptr(int);", "void ptr(int)*;"},
        1
    },
    {
        10,
        "1",
        "Pilha",
        "Overflow",
        "Dificil",
        "Push em pilha cheia gera?",
        {"Substituicao", "Overflow", "Redimensionamento automatico", "Insercao na base"},
        1
    },
    {
        11,
        "2",
        "Lista Encadeada",
        "Ciclo",
        "Dificil",
        "O algoritmo de Floyd, tambem chamado de ponteiro rapido e lento, e usado principalmente para:",
        {
            "Detectar se a lista possui um ciclo.",
            "Inverter a ordem da lista usando O(1) de memoria extra.",
            "Encontrar o menor e o maior valor em uma unica iteracao.",
            "Converter uma lista simples em lista dupla."
        },
        0
    },
    {
        12,
        "2",
        "Lista Duplamente Encadeada",
        "Memoria",
        "Dificil",
        "Comparando com listas simplesmente encadeadas, qual e a principal desvantagem das listas duplamente encadeadas?",
        {
            "E impossivel implementar uma fila eficiente com ela.",
            "O acesso aleatorio fica mais lento por causa do ponteiro extra.",
            "Maior consumo de memoria por no, pois armazena dois enderecos em vez de um.",
            "A insercao no final passa a ser O(n), mesmo com referencia para a cauda."
        },
        2
    }
};

// Pilhas usadas durante o jogo principal
static tp_pilha pilhaFacil;
static tp_pilha pilhaMedio;
static tp_pilha pilhaDificil;

static int pilhasInicializadas = 0;
static int randomInicializado = 0;

static void inicializarRandomUmaVez() {
    if (!randomInicializado) {
        srand((unsigned int) time(NULL));
        randomInicializado = 1;
    }
}

void embaralhar(int *vetor, int n) {
    for (int i = n - 1; i > 0; i--) {
        int j = rand() % (i + 1);

        int temp = vetor[i];
        vetor[i] = vetor[j];
        vetor[j] = temp;
    }
}

static void montarPilhaPerguntas(tp_pilha *p, int inicio, int fim) {
    int indices[4];
    int qtd = 0;

    inicializa_pilha(p);

    for (int i = inicio; i <= fim; i++) {
        indices[qtd] = i;
        qtd++;
    }

    embaralhar(indices, qtd);

    for (int i = 0; i < qtd; i++) {
        push(p, indices[i]);
    }
}

void inicializarPilhasPerguntas() {
    inicializarRandomUmaVez();

    montarPilhaPerguntas(&pilhaFacil, 0, 3);
    montarPilhaPerguntas(&pilhaMedio, 4, 7);
    montarPilhaPerguntas(&pilhaDificil, 8, 11);

    pilhasInicializadas = 1;
}

// Opcional: use essa funcao se quiser reiniciar as perguntas ao comecar uma nova partida
void reiniciarPerguntasJogo() {
    inicializarPilhasPerguntas();
}

static tp_pilha* selecionarPilhaPorNivel(NivelPergunta nivel) {
    if (nivel == FACIL) {
        return &pilhaFacil;
    }
    else if (nivel == MEDIO) {
        return &pilhaMedio;
    }
    else {
        return &pilhaDificil;
    }
}

static void reembaralharNivel(NivelPergunta nivel) {
    if (nivel == FACIL) {
        montarPilhaPerguntas(&pilhaFacil, 0, 3);
    }
    else if (nivel == MEDIO) {
        montarPilhaPerguntas(&pilhaMedio, 4, 7);
    }
    else {
        montarPilhaPerguntas(&pilhaDificil, 8, 11);
    }
}

static const char* nomeNivelPergunta(NivelPergunta nivel) {
    if (nivel == FACIL) {
        return "FACIL";
    }
    else if (nivel == MEDIO) {
        return "MEDIA";
    }
    else {
        return "DIFICIL";
    }
}

static const char* corNivelPergunta(NivelPergunta nivel) {
    if (nivel == FACIL) {
        return GREEN;
    }
    else if (nivel == MEDIO) {
        return YELLOW;
    }
    else {
        return RED;
    }
}

int totalPerguntasJogo() {
    return TOTAL_PERGUNTAS;
}

const Questao* obterQuestaoPorIndice(int indice) {
    if (indice < 0 || indice >= TOTAL_PERGUNTAS) {
        return NULL;
    }

    return &bancoPerguntas[indice];
}

int sortearIndicePerguntaNivel(NivelPergunta nivel) {
    inicializarRandomUmaVez();

    if (nivel == FACIL) {
        return rand() % QTD_FACEIS;
    }

    if (nivel == MEDIO) {
        return QTD_FACEIS + (rand() % QTD_MEDIAS);
    }

    return QTD_FACEIS + QTD_MEDIAS + (rand() % QTD_DIFICEIS);
}

static void copiarCampoQuestoesCSV(char *destino, const char *origem, int tamanhoDestino) {
    int i = 0;

    if (tamanhoDestino <= 0) {
        return;
    }

    if (origem == NULL) {
        destino[0] = '\0';
        return;
    }

    while (origem[i] != '\0' && i < tamanhoDestino - 1) {
        if (origem[i] == ';' || origem[i] == '\n' || origem[i] == '\r') {
            destino[i] = ' ';
        } else {
            destino[i] = origem[i];
        }

        i++;
    }

    destino[i] = '\0';
}

void salvarPerguntasCSV() {
    FILE *arquivo = fopen("perguntas.csv", "w");

    if (arquivo == NULL) {
        printf("Erro ao criar o arquivo perguntas.csv.\n");
        return;
    }

    fprintf(arquivo, "id_pergunta;unidade;tema;subtema;dificuldade;enunciado\n");

    for (int i = 0; i < TOTAL_PERGUNTAS; i++) {
        char idPergunta[10];
        char unidadeCSV[100];
        char temaCSV[100];
        char subtemaCSV[100];
        char dificuldadeCSV[30];
        char enunciadoCSV[500];

        snprintf(idPergunta, sizeof(idPergunta), "P%03d", bancoPerguntas[i].id);
        copiarCampoQuestoesCSV(unidadeCSV, bancoPerguntas[i].unidade, sizeof(unidadeCSV));
        copiarCampoQuestoesCSV(temaCSV, bancoPerguntas[i].tema, sizeof(temaCSV));
        copiarCampoQuestoesCSV(subtemaCSV, bancoPerguntas[i].subtema, sizeof(subtemaCSV));
        copiarCampoQuestoesCSV(dificuldadeCSV, bancoPerguntas[i].dificuldade, sizeof(dificuldadeCSV));
        copiarCampoQuestoesCSV(enunciadoCSV, bancoPerguntas[i].enunciado, sizeof(enunciadoCSV));

        fprintf(
            arquivo,
            "%s;%s;%s;%s;%s;%s\n",
            idPergunta,
            unidadeCSV,
            temaCSV,
            subtemaCSV,
            dificuldadeCSV,
            enunciadoCSV
        );
    }

    fclose(arquivo);
}

static int mostrarPerguntaNaTela(Questao escolhida, NivelPergunta nivel, int *respostaJogador) {
    int selecionado = 0;
    int tecla = 0;

    while (1) {
        system("cls");

        posicionar(45, 5);
        printf("%s--- PERGUNTA %s ---%s", corNivelPergunta(nivel), nomeNivelPergunta(nivel), RESET);

        posicionar(18, 8);
        printf(WHITE "%s" RESET, escolhida.enunciado);

        for (int i = 0; i < 4; i++) {
            posicionar(22, 11 + i);

            if (selecionado == i) {
                printf(CYAN " > %s" RESET, escolhida.alternativas[i]);
            } else {
                printf("   %s", escolhida.alternativas[i]);
            }
        }

        tecla = _getch();

        if (tecla == 224 || tecla == 0) {
            tecla = _getch();

            if (tecla == SETA_CIMA) {
                selecionado = (selecionado <= 0) ? 3 : selecionado - 1;
            }
            else if (tecla == SETA_BAIXO) {
                selecionado = (selecionado >= 3) ? 0 : selecionado + 1;
            }
        }
        else if (tecla == ENTER) {
            posicionar(45, 17);

            if (respostaJogador != NULL) {
                *respostaJogador = selecionado;
            }

            if (selecionado == escolhida.respostaCorreta) {
                printf(GREEN "CORRETO!" RESET);
                _getch();
                return 1;
            } else {
                printf(RED "ERRADO!" RESET);
                _getch();
                return 0;
            }
        }
    }
}

// Essa funcao continua servindo para o menu separado de questoes
void jogar(tp_pilha *p, Questao *banco, char *nivel) {
    if (pilha_vazia(p)) {
        posicionar(45, 15);
        printf(RED "[!] Nivel concluido!" RESET);
        _getch();
        return;
    }

    int indice;
    pop(p, &indice);

    int selecionado = 0;
    int tecla = 0;

    while (1) {
        system("cls");

        posicionar(45, 5);
        printf(YELLOW "--- DESAFIO: %s ---" RESET, nivel);

        posicionar(18, 8);
        printf(WHITE "%s" RESET, banco[indice].enunciado);

        for (int i = 0; i < 4; i++) {
            posicionar(22, 11 + i);

            if (selecionado == i) {
                printf(CYAN " > %s" RESET, banco[indice].alternativas[i]);
            } else {
                printf("   %s", banco[indice].alternativas[i]);
            }
        }

        tecla = _getch();

        if (tecla == 224 || tecla == 0) {
            tecla = _getch();

            if (tecla == SETA_CIMA) {
                selecionado = (selecionado <= 0) ? 3 : selecionado - 1;
            }
            else if (tecla == SETA_BAIXO) {
                selecionado = (selecionado >= 3) ? 0 : selecionado + 1;
            }
        }
        else if (tecla == ENTER) {
            posicionar(45, 17);

            if (selecionado == banco[indice].respostaCorreta) {
                printf(GREEN "CORRETO!" RESET);
            } else {
                printf(RED "ERRADO!" RESET);
            }

            _getch();
            break;
        }
    }
}

// Menu separado de questoes
void QuestoesJogo() {
    inicializarRandomUmaVez();

    tp_pilha f, m, d;

    inicializa_pilha(&f);
    inicializa_pilha(&m);
    inicializa_pilha(&d);

    int iF[] = {0, 1, 2, 3};
    int iM[] = {4, 5, 6, 7};
    int iD[] = {8, 9, 10, 11};

    embaralhar(iF, 4);
    embaralhar(iM, 4);
    embaralhar(iD, 4);

    for (int i = 0; i < 4; i++) {
        push(&f, iF[i]);
    }

    for (int i = 0; i < 4; i++) {
        push(&m, iM[i]);
    }

    for (int i = 0; i < 4; i++) {
        push(&d, iD[i]);
    }

    int selecionado = 1;
    int tecla = 0;

    while (1) {
        system("cls");

        posicionar(52, 4);
        printf(CYAN "Q U E S T O E S" RESET);

        posicionar(48, 5);
        printf("-----------------");

        posicionar(45, 8);
        printf("%s 1. NIVEL FACIL   (%d)" RESET, (selecionado == 1 ? GREEN " >" : "  "), f.topo + 1);

        posicionar(45, 9);
        printf("%s 2. NIVEL MEDIO   (%d)" RESET, (selecionado == 2 ? YELLOW " >" : "  "), m.topo + 1);

        posicionar(45, 10);
        printf("%s 3. NIVEL DIFICIL (%d)" RESET, (selecionado == 3 ? RED " >" : "  "), d.topo + 1);

        posicionar(45, 12);
        printf("%s 0. SAIR" RESET, (selecionado == 4 ? WHITE " >" : "  "));

        tecla = _getch();

        if (tecla == 224 || tecla == 0) {
            tecla = _getch();

            if (tecla == SETA_CIMA) {
                selecionado = (selecionado <= 1) ? 4 : selecionado - 1;
            }
            else if (tecla == SETA_BAIXO) {
                selecionado = (selecionado >= 4) ? 1 : selecionado + 1;
            }
        }
        else if (tecla == ENTER) {
            if (selecionado == 1) {
                jogar(&f, bancoPerguntas, "FACIL");
            }
            else if (selecionado == 2) {
                jogar(&m, bancoPerguntas, "MEDIO");
            }
            else if (selecionado == 3) {
                jogar(&d, bancoPerguntas, "DIFICIL");
            }
            else if (selecionado == 4) {
                break;
            }
        }
    }
}

// Essa e a funcao usada dentro da partida
// Agora ela usa PILHA, nao sorteio direto
int responderPerguntaNivelDetalhado(NivelPergunta nivel, ResultadoPergunta *resultado) {
    if (!pilhasInicializadas) {
        inicializarPilhasPerguntas();
    }

    tp_pilha *pilhaEscolhida = selecionarPilhaPorNivel(nivel);

    // Se acabarem as perguntas daquele nivel, reembaralha aquele nivel
    if (pilha_vazia(pilhaEscolhida)) {
        reembaralharNivel(nivel);
        pilhaEscolhida = selecionarPilhaPorNivel(nivel);
    }

    int indicePergunta;
    pop(pilhaEscolhida, &indicePergunta);

    Questao escolhida = bancoPerguntas[indicePergunta];
    int respostaJogador = -1;
    int acertou = mostrarPerguntaNaTela(escolhida, nivel, &respostaJogador);

    if (resultado != NULL) {
        resultado->questao = escolhida;
        resultado->respostaJogador = respostaJogador;
        resultado->acertou = acertou;
    }

    return acertou;
}

int responderPerguntaNivel(NivelPergunta nivel) {
    return responderPerguntaNivelDetalhado(nivel, NULL);
}
