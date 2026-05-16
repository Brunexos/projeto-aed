#ifndef TABULEIRO_H
#define TABULEIRO_H
#include <stdio.h>
#include <stdlib.h>
#define MAX 100



//uso do enum para definir os tipos de casa pela maior praticidade q é dada pro código.
typedef enum{
    NORMAL,
    PRISAO,
    AZAR,
    SORTE,
    //aqui são os tipos de casa, aq só tem alguns exemplos que podem ser alterados
} TipoCasa;


// struct que representa uma casa do tabuleiro
typedef struct Casa{
    int id;
    char nome[100];
    TipoCasa tipo;
    struct Casa *prox;
    struct Casa *ant;
} Casa;

#endif