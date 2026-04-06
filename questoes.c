#include "questoes.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// Funções auxiliares (internas deste módulo)
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
        printf("\n[!] Já respondeu tudo!\n");
        system("pause");
        return;
    }

    int indice, resp;
    pop(p, &indice);

    system("cls");
    printf("=== %s ===\n", nivel);
    printf("%s\n\n", banco[indice].pergunta);

    for (int i = 0; i < 4; i++) {
        printf("%d) %s\n", i + 1, banco[indice].alternativas[i]);
    }

    printf("\nSua resposta: ");
    scanf("%d", &resp);

    if (resp - 1 == banco[indice].respostaCorreta)
        printf("ACERTOU!\n");
    else
        printf("ERROU! Resposta correta: %d\n", banco[indice].respostaCorreta + 1);

    system("pause");
}

// Implementação da função principal do módulo
void QuestoesJogo() {
    srand(time(NULL));

    Questao banco[] = {
        {"Para que serve declarar int?", {"Reais", "Palavras", "Inteiros", "Imprimir"}, 2},
        {"Elemento sai de onde na pilha?", {"Baixo", "Cima", "Meio", "Aleatorio"}, 1},
        {"Topo inicial?", {"1", "-1", "0", "2"}, 1},
        {"int **p armazena?", {"Valor", "Endereco", "Array", "Erro"}, 1},
        {RED"Sintaxe correta para ponteiro de funcao (void, recebe int)?"RESET, {"void *f(int);", "void (*ptr)(int),", "func ptr(int);", "void (*ptr)(int);"}, 3},
        {"Em uma pilha implementada com vetor, o que ocorre se uma operação de push é realizada quando a pilha já está cheia?", {"O elemento substitui o topo atual", "Ocorre overflow de pilha", "A pilha é automaticamente redimensionada", " O elemento é inserido na base"}, 1}
    };

    tp_pilha f, m, d;
    inicializa_pilha(&f);
    inicializa_pilha(&m);
    inicializa_pilha(&d);

    int iF[] = {0, 1}, iM[] = {2, 3}, iD[] = {4, 5};

    embaralhar(iF, 2);
    embaralhar(iM, 2);
    embaralhar(iD, 2);

    push(&f, iF[0]); push(&f, iF[1]);
    push(&m, iM[0]); push(&m, iM[1]);
    push(&d, iD[0]); push(&d, iD[1]);

    int op;
    do {
        system("cls");
        printf("=== MENU DE DESAFIOS ===\n");
        printf("1-FACIL (%d restantes)\n", f.topo + 1);
        printf("2-MEDIA (%d restantes)\n", m.topo + 1);
        printf("3-DIFICIL (%d restantes)\n", d.topo + 1);
        printf("0-SAIR\n");
        printf("Escolha: ");

        if (scanf("%d", &op) != 1) break;

        if (op == 1) jogar(&f, banco, "FACIL");
        else if (op == 2) jogar(&m, banco, "MEDIA");
        else if (op == 3) jogar(&d, banco, "DIFICIL");

    } while (op != 0);
}