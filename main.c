#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
int main(int argc, char *argv[]) {
    // Verifica se o programa foi aberto com um argumento específico
    // Se não foi, ele reabre a si mesmo em uma nova janela de CMD
    if (argc == 1) {
        char command[256];
        sprintf(command, "start cmd /k \"%s run\"", argv[0]);
        system(command);
        return 0; // Fecha a instância atual (a do terminal do VS Code)
    }
    char escolha;
    int rodando = 1;

    printf("=== BEM-VINDO AO REINO DE C ===\n");

    while (rodando) {
        printf("\n-------------------------------\n");
        printf(" Voce deseja entrar na caverna?\n");
        printf(" [Y] Sim / [N] Nao / [S] Sair\n");
        printf("-------------------------------\n");
        printf("Escolha: ");

        // Captura o caractere e limpa o buffer do teclado
        scanf(" %c", &escolha);
        escolha = tolower(escolha); // Transforma em minúsculo para facilitar

        printf("\n--- RESULTADO ---\n");
        if (escolha == 'y') {
            printf(">> Voce entrou na caverna e encontrou um tesouro!\n");
        } 
        else if (escolha == 'n') {
            printf(">> Voce decidiu ficar acampando do lado de fora.\n");
        } 
        else if (escolha == 's') {
            printf(">> Encerrando o programa... Ate logo!\n");
            rodando = 0;
        } 
        else {
            printf(">> Comando invalido! Tente Y, N ou S.\n");
        }
	}
    return 0;
}