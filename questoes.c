#include "questoes.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <conio.h>
#include <string.h>

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

#define QTD_FACEIS 6
#define QTD_MEDIAS 6
#define QTD_DIFICEIS 6
#define TOTAL_PERGUNTAS 18

void posicionar(int x, int y) {
    printf("\033[%d;%dH", y, x);
}

// Banco geral de perguntas
// As pilhas de jogo sao montadas pelo campo "dificuldade".
// Assim, novas perguntas podem manter a sequencia de IDs sem depender da posicao no vetor.
Questao bancoPerguntas[TOTAL_PERGUNTAS] = {
    // =====================================================
    // FACEIS
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
    // MEDIAS
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
    // DIFICIES
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
    },
    {
        13,
        "3",
        "Arvore Binaria de Busca",
        "Percurso em BST",
        "Facil",
        "(Arvore Binaria de Busca - BST) Qual metodo de percurso (traversal) em uma Arvore Binaria de Busca deve ser utilizado para visitar/imprimir todos os elementos em ordem estritamente crescente?",
        {
            "Pre-ordem (Pre-order).",
            "Pos-ordem (Post-order).",
            "Em ordem (In-order).",
            "Em nivel (Level-order / Busca em Largura)."
        },
        2
    },
    {
        14,
        "3",
        "Arvores B",
        "Split",
        "Facil",
        "(Arvores B) O que acontece obrigatoriamente em uma Arvore B quando um no atinge sua capacidade maxima de chaves permitidas e uma nova chave precisa ser inserida nele?",
        {
            "A arvore e completamente reconstruida a partir do zero em um novo arquivo para acomodar a nova capacidade.",
            "O no sofre um processo chamado split (divisao), onde e partido em dois, e a chave mediana e promovida para o no pai.",
            "A nova chave e descartada e a operacao falha, retornando um erro de Arvore Cheia.",
            "O no se transforma temporariamente em uma lista encadeada para abrigar a chave excedente sem alterar o resto da arvore."
        },
        1
    },
    {
        15,
        "3",
        "Tabela Hash",
        "Encadeamento Separado",
        "Medio",
        "(Tabela Hash) Qual das alternativas abaixo descreve corretamente o metodo de Encadeamento Separado (Separate Chaining) utilizado para tratar colisoes em uma Tabela Hash?",
        {
            "Quando ocorre uma colisao, o elemento procura a proxima posicao vazia no proprio vetor sequencialmente (Sondagem Linear).",
            "Cada posicao do vetor da tabela hash funciona como um ponteiro para uma lista encadeada, onde todos os elementos que colidiram naquele indice sao armazenados.",
            "O vetor principal e redimensionado automaticamente para o dobro do tamanho sempre que uma colisao unica acontece.",
            "Uma segunda funcao de hash e aplicada repetidamente na chave ate encontrar um espaco vazio no vetor original."
        },
        1
    },
    {
        16,
        "3",
        "Arvores B e B+",
        "Diferenca entre B e B+",
        "Medio",
        "(Arvores B e B+) Ao estudar os conceitos de Arvores B e Arvores B+, qual e a principal caracteristica arquitetural que diferencia de forma vital a Arvore B+ de uma Arvore B tradicional?",
        {
            "Na Arvore B+, os nos nao tem um numero maximo de filhos, permitindo uma largura infinita por nivel.",
            "Na Arvore B+, todos os registros reais ficam apenas nas folhas, e os nos internos guardam apenas chaves de roteamento/guia.",
            "A Arvore B+ e estritamente binaria, enquanto a Arvore B pode ter multiplos filhos.",
            "A Arvore B cresce de cima para baixo, enquanto a Arvore B+ cresce inserindo apenas na raiz."
        },
        1
    },
    {
        17,
        "3",
        "Arvore AVL",
        "Balanceamento",
        "Dificil",
        "(Arvore AVL) Em uma Arvore AVL estritamente balanceada, sabemos que as operacoes sao limitadas no pior caso a O(log n). Qual propriedade exata e permanente da estrutura impoe e garante essa limitacao de tempo?",
        {
            "O fato de que todas as subarvores esquerda e direita nao possuem limite de tamanho, desde que haja memoria disponivel no sistema.",
            "O fator de balanceamento, onde a diferenca de altura entre a subarvore esquerda e direita de qualquer no da arvore nunca ultrapassa o modulo de 1, ou seja, e -1, 0 ou 1.",
            "A garantia de que, em todos os nos internos, a quantidade exata de descendentes a esquerda e identica a quantidade de descendentes a direita.",
            "A propriedade de realizar obrigatoriamente quatro rotacoes complexas sempre que um valor for pesquisado e nao for encontrado na raiz."
        },
        1
    },
    {
        18,
        "3",
        "Manipulacao de Arquivos",
        "Modo Append",
        "Dificil",
        "(Manipulacao de Arquivos) Ao trabalhar com manipulacao de arquivos em linguagens como C, qual e o efeito principal de abrir um arquivo utilizando o modo 'a' (append / anexar)?",
        {
            "O arquivo e aberto apenas para leitura, e o ponteiro e posicionado no final dele.",
            "Ele apaga todo o conteudo anterior do arquivo e comeca a gravar dados novos a partir do inicio.",
            "Ele abre o arquivo para escrita e adiciona os novos dados no final, preservando o conteudo existente.",
            "O arquivo e carregado inteiramente para a memoria RAM (vetor) para ser editado no meio de seu conteudo."
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

static int questaoPertenceAoNivel(const Questao *questao, NivelPergunta nivel) {
    if (questao == NULL) {
        return 0;
    }

    if (nivel == FACIL) {
        return strcmp(questao->dificuldade, "Facil") == 0;
    }

    if (nivel == MEDIO) {
        return strcmp(questao->dificuldade, "Medio") == 0;
    }

    return strcmp(questao->dificuldade, "Dificil") == 0;
}

static void montarPilhaPerguntas(tp_pilha *p, NivelPergunta nivel) {
    int indices[TOTAL_PERGUNTAS];
    int qtd = 0;

    inicializa_pilha(p);

    for (int i = 0; i < TOTAL_PERGUNTAS; i++) {
        if (questaoPertenceAoNivel(&bancoPerguntas[i], nivel)) {
            indices[qtd] = i;
            qtd++;
        }
    }

    embaralhar(indices, qtd);

    for (int i = 0; i < qtd; i++) {
        push(p, indices[i]);
    }
}

void inicializarPilhasPerguntas() {
    inicializarRandomUmaVez();

    montarPilhaPerguntas(&pilhaFacil, FACIL);
    montarPilhaPerguntas(&pilhaMedio, MEDIO);
    montarPilhaPerguntas(&pilhaDificil, DIFICIL);

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
        montarPilhaPerguntas(&pilhaFacil, FACIL);
    }
    else if (nivel == MEDIO) {
        montarPilhaPerguntas(&pilhaMedio, MEDIO);
    }
    else {
        montarPilhaPerguntas(&pilhaDificil, DIFICIL);
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
    int indices[TOTAL_PERGUNTAS];
    int qtd = 0;

    inicializarRandomUmaVez();

    for (int i = 0; i < TOTAL_PERGUNTAS; i++) {
        if (questaoPertenceAoNivel(&bancoPerguntas[i], nivel)) {
            indices[qtd] = i;
            qtd++;
        }
    }

    if (qtd == 0) {
        return -1;
    }

    return indices[rand() % qtd];
}

int quantidadePerguntasRestantesNivel(NivelPergunta nivel) {
    if (!pilhasInicializadas) {
        inicializarPilhasPerguntas();
    }

    return altura_pilha(selecionarPilhaPorNivel(nivel));
}

int consumirIndicePerguntaNivel(NivelPergunta nivel) {
    int indicePergunta = -1;
    tp_pilha *pilhaEscolhida;

    if (!pilhasInicializadas) {
        inicializarPilhasPerguntas();
    }

    pilhaEscolhida = selecionarPilhaPorNivel(nivel);

    if (pilha_vazia(pilhaEscolhida)) {
        return -1;
    }

    if (!pop(pilhaEscolhida, &indicePergunta)) {
        return -1;
    }

    return indicePergunta;
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

    montarPilhaPerguntas(&f, FACIL);
    montarPilhaPerguntas(&m, MEDIO);
    montarPilhaPerguntas(&d, DIFICIL);

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
