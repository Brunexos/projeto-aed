#include <stdio.h>
#include "pilha.h"
void preencher_pilha(tp_pilha *p) {
    int tamanho;
    printf("Tamanho da pilha");
    scanf("%d", &tamanho);
    int valor;
    for(int i = 0; i < tamanho; i++) {
        scanf("%d", &valor);
        if (!push(p, valor)) {
            printf("Erro: Pilha cheia!\n");
            break;
        }
    }
}
int comparar_pilhas(tp_pilha *pilha1, tp_pilha *pilha2){
	tp_pilha armazena1, armazena2;
	int valor1, valor2;
	int identicas=1;
	inicializa_pilha(&armazena1);
	inicializa_pilha(&armazena2);
	if(altura_pilha(pilha1)!=altura_pilha(pilha2)){
		return 0;
	} 
	while(!pilha_vazia(pilha1) && !pilha_vazia(pilha2)){
		pop(pilha1, &valor1);
		pop(pilha2, &valor2);
		push(&armazena1, valor1);
		push(&armazena2, valor2);
		if(valor1!=valor2){
			identicas=0;
			break;
		}
	}
	while(!pilha_vazia(&armazena1) && !pilha_vazia(&armazena2)){
		pop(&armazena1, &valor1);
		pop(&armazena2, &valor2);
		push(pilha1, valor1);
		push(pilha2, valor2);
	}
	return identicas;
}

int main() {
    tp_pilha pilhaum, pilhadois;
    
    inicializa_pilha(&pilhaum);
    inicializa_pilha(&pilhadois);
    preencher_pilha(&pilhaum);
    preencher_pilha(&pilhadois);
    printf("Pilha original1");
    imprime_pilha(pilhaum);
    printf("Pilha original2");
    imprime_pilha(pilhadois);
	if(comparar_pilhas(&pilhaum, &pilhadois)){
		printf("São iguais\n");
	}else{
		printf("São diferentes\n");
	}
	printf("gozei legal");
	return 0;
}	
