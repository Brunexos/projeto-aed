#ifndef GRAFICOS_H
#define GRAFICOS_H

#include <stdio.h>

#define RED     "\x1b[31m"
#define GREEN   "\x1b[32m"
#define YELLOW  "\x1b[33m"
#define BLUE    "\x1b[34m"
#define RESET   "\x1b[0m"

// Mostra os peões numerados para o jogador escolher
void mostrarOpcoesPeoes() {
    char *cores[] = {RED, GREEN, YELLOW, BLUE};
    char *camadas[] = {
        "  ___    ", " /   \\   ", "| ( ) |  ", " \\___/   ", 
        "  | |    ", " /___\\   ", "/     \\  ", "|       | ", "|_______| "
    };

    printf("\nEscolha seu modelo de peao:\n");
    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 4; j++) {
            printf("%s%s  ", cores[j], camadas[i]);
        }
        printf(RESET "\n");
    }
    printf("    (1)          (2)          (3)          (4)\n\n");
}

// Retorna a string da cor baseada na escolha
const char* obterCor(int opcao) {
    switch(opcao) {
        case 1: return RED;
        case 2: return GREEN;
        case 3: return YELLOW;
        case 4: return BLUE;
        default: return RESET;
    }
}

#endif