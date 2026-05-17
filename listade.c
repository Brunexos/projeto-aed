#include "listade.h"

tp_listad *inicializa_listad() {
    tp_listad *lista = (tp_listad*) malloc(sizeof(tp_listad));

    if (lista == NULL) {
        return NULL;
    }

    lista->ini = NULL;
    lista->fim = NULL;

    return lista;
}

int listad_vazia(tp_listad *lista) {
    if (lista == NULL || lista->ini == NULL) {
        return 1;
    }

    return 0;
}

tp_no *aloca_listad() {
    tp_no *pt = (tp_no*) malloc(sizeof(tp_no));
    return pt;
}

int insere_listad_no_fim(tp_listad *lista, tp_itemd e) {   
    tp_no *novo;

    if (lista == NULL) {
        return 0;
    }

    novo = aloca_listad();

    if (novo == NULL) {
        return 0;
    }

    novo->info = e;  

    if (listad_vazia(lista)) {
        novo->prox = NULL;
        novo->ant = NULL; 
        lista->ini = novo;
        lista->fim = novo;
    }  
    else {  
        novo->prox = NULL;
        novo->ant = lista->fim; 
        lista->fim->prox = novo;
        lista->fim = novo;
    }  

    return 1;   
} 

void imprime_listad(tp_listad *lista, int ordem) {
    if (lista == NULL) {
        printf("Lista nao inicializada");
    }
    else {
        tp_no *atu;     

        switch (ordem) {
            case 1:
                atu = lista->ini;

                while (atu != NULL) {
                    printf("%d ", atu->info);
                    atu = atu->prox;           
                }

                break;   

            case 2:
                atu = lista->fim;

                while (atu != NULL) {
                    printf("%d ", atu->info);
                    atu = atu->ant;           
                }

                break;   

            default:
                printf("codigo invalido");        
        }
    }

    printf("\n");
}

int remove_listad(tp_listad *lista, tp_itemd e) {   
    tp_no *atu;

    if (lista == NULL) {
        return 0;
    }

    atu = lista->ini;

    while ((atu != NULL) && (atu->info != e)) { 
        atu = atu->prox;
    }

    if (atu == NULL) {
        return 0;
    }

    if (lista->ini == lista->fim) {
        lista->ini = NULL;
        lista->fim = NULL;
    }
    else {   
        if (lista->ini == atu) {
            lista->ini = atu->prox;
            lista->ini->ant = NULL;
        }  
        else if (lista->fim == atu) {
            lista->fim = atu->ant;
            lista->fim->prox = NULL;              
        }
        else {
            atu->prox->ant = atu->ant;   
            atu->ant->prox = atu->prox;
        }
    }   

    free(atu);  

    return 1;   
}

tp_no *busca_listade(tp_listad *lista, tp_itemd e) {   
    tp_no *atu;

    if (lista == NULL) {
        return NULL;
    }

    atu = lista->ini;

    while ((atu != NULL) && (atu->info != e)) { 
        atu = atu->prox;
    }

    return atu;
}    

tp_listad *Destroi_listad(tp_listad *lista) {   
    tp_no *atu;

    if (lista == NULL) {
        return NULL;
    }

    atu = lista->ini;

    while (atu != NULL) { 
        lista->ini = atu->prox;
        free(atu);
        atu = lista->ini;
    }

    free(lista);

    return NULL;
}  

void inverte_listad(tp_listad *lista) {
    if (lista == NULL || lista->ini == NULL) {
        return;
    }

    tp_no *atu = lista->ini;
    tp_no *temp = NULL;

    while (atu != NULL) {
        temp = atu->ant;
        atu->ant = atu->prox;
        atu->prox = temp;

        atu = atu->ant;
    }

    temp = lista->ini;
    lista->ini = lista->fim;
    lista->fim = temp;
}