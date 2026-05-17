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

void posicionar(int x, int y) {
    printf("\033[%d;%dH", y, x);
}

void embaralhar(int *vetor, int n) {
    for (int i = n - 1; i > 0; i--) {
        int j = rand() % (i + 1);

        int temp = vetor[i];
        vetor[i] = vetor[j];
        vetor[j] = temp;
    }
}

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
        
        posicionar(35, 8);
        printf(WHITE "%s" RESET, banco[indice].pergunta);

        for (int i = 0; i < 4; i++) {
            posicionar(40, 11 + i);

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

void QuestoesJogo() {
    Questao banco[] = {
        {"Para que serve declarar int?", {"Reais", "Palavras", "Inteiros", "Imprimir"}, 2},
        {"Elemento sai de onde na pilha?", {"Baixo", "Cima", "Meio", "Aleatorio"}, 1},
        {"Qual o valor do topo inicial?", {"1", "-1", "0", "2"}, 1},
        {"O que int **p armazena?", {"Valor", "Endereco", "Array", "Erro"}, 1},
        {"Sintaxe correta de ponteiro?", {"void *f(int);", "void (*ptr)(int);", "func ptr(int);", "void ptr(int)*;"}, 1},
        {"Push em pilha cheia gera?", {"Substitui", "Overflow", "Redimensiona", "Insere base"}, 1}
    };

    tp_pilha f, m, d;

    inicializa_pilha(&f);
    inicializa_pilha(&m);
    inicializa_pilha(&d);

    int iF[] = {0, 1};
    int iM[] = {2, 3};
    int iD[] = {4, 5};

    embaralhar(iF, 2);
    embaralhar(iM, 2);
    embaralhar(iD, 2);

    push(&f, iF[0]);
    push(&f, iF[1]);

    push(&m, iM[0]);
    push(&m, iM[1]);

    push(&d, iD[0]);
    push(&d, iD[1]);

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
                jogar(&f, banco, "FACIL");
            }
            else if (selecionado == 2) {
                jogar(&m, banco, "MEDIA");
            }
            else if (selecionado == 3) {
                jogar(&d, banco, "DIFICIL");
            }
            else if (selecionado == 4) {
                break;
            }
        }
    }
}

int responderPerguntaNivel(NivelPergunta nivel) {
    Questao faceis[] = {
        {"Para que serve declarar int?", {"Reais", "Palavras", "Inteiros", "Imprimir"}, 2},
        {"Elemento sai de onde na pilha?", {"Baixo", "Cima", "Meio", "Aleatorio"}, 1}
    };

    Questao medias[] = {
        {"Qual o valor do topo inicial de uma pilha vazia?", {"1", "-1", "0", "2"}, 1},
        {"O que int **p armazena?", {"Valor comum", "Endereco de ponteiro", "Texto", "Erro sempre"}, 1}
    };

    Questao dificeis[] = {
        {"Sintaxe correta de ponteiro para funcao?", {"void *f(int);", "void (*ptr)(int);", "func ptr(int);", "void ptr(int)*;"}, 1},
        {"Push em pilha cheia gera?", {"Substituicao", "Overflow", "Redimensionamento automatico", "Insercao na base"}, 1}
    };

    Questao escolhida;

    if (nivel == FACIL) {
        escolhida = faceis[rand() % 2];
    }
    else if (nivel == MEDIO) {
        escolhida = medias[rand() % 2];
    }
    else {
        escolhida = dificeis[rand() % 2];
    }

    int selecionado = 0;
    int tecla = 0;

    while (1) {
        system("cls");

        posicionar(45, 5);

        if (nivel == FACIL) {
            printf(GREEN "--- PERGUNTA FACIL ---" RESET);
        }
        else if (nivel == MEDIO) {
            printf(YELLOW "--- PERGUNTA MEDIA ---" RESET);
        }
        else {
            printf(RED "--- PERGUNTA DIFICIL ---" RESET);
        }

        posicionar(30, 8);
        printf(WHITE "%s" RESET, escolhida.pergunta);

        for (int i = 0; i < 4; i++) {
            posicionar(40, 11 + i);

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