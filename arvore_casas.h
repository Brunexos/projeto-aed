#ifndef ARVORE_CASAS_H
#define ARVORE_CASAS_H

typedef struct NoCasa {
    int idCasa;
    int quantidadeQuedas;
    int altura;
    struct NoCasa *esquerda;
    struct NoCasa *direita;
} NoCasa;

void registrarQuedaCasa(NoCasa **raiz, int idCasa);
int quantidadeQuedasCasa(NoCasa *raiz, int idCasa);
void exibirQuedasCasas(NoCasa *raiz);
void salvarQuedasCasasCSV(NoCasa *raiz, const char *nomeArquivo);
void liberarArvoreCasas(NoCasa *raiz);

#endif
