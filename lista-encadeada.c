#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "rede.h"

// Ponteiro para início da lista
struct No *inicio = NULL;

// Inserir no início
void inserirInicio(int id, char origem[], char destino[]) {
    struct No *novo = (struct No*) malloc(sizeof(struct No));

    if (novo == NULL) {
        printf("Erro de memoria!\n");
        return;
    }

    novo->id = id;
    strcpy(novo->origem, origem);
    strcpy(novo->destino, destino);
    novo->status = 1; // Status inicial: em trânsito
    novo->prox = inicio;
    inicio = novo;
}

// Inserir no final
void inserirFinal(int id, char origem[], char destino[]) {
    struct No *novo = (struct No*) malloc(sizeof(struct No));
    struct No *atual;

    if (novo == NULL) {
        printf("Erro de memoria!\n");
        return;
    }

    novo->id = id;
    strcpy(novo->origem, origem);
    strcpy(novo->destino, destino);
    novo->status = 1; // Status inicial: em trânsito
    novo->prox = NULL;

    if (inicio == NULL) {
        inicio = novo;
        return;
    }

    atual = inicio;

    while (atual->prox != NULL) {
        atual = atual->prox;
    }

    atual->prox = novo;
}

// Remover elemento
void remover(int id) {
    struct No *atual = inicio;
    struct No *anterior = NULL;

    if (inicio == NULL) {
        printf("Lista vazia!\n");
        return;
    }

    while (atual != NULL && atual->id != id) {
        anterior = atual;
        atual = atual->prox;
    }

    if (atual == NULL) {
        printf("Elemento nao encontrado!\n");
        return;
    }

    if (anterior == NULL) {
        inicio = atual->prox;
    } else {
        anterior->prox = atual->prox;
    }

    free(atual);
}

void buscarPacote(int id) {

    struct No *atual = inicio;

    while (atual != NULL) {

        if (atual->id == id) {

            printf("\nPacote encontrado:\n");
            printf("ID: %d\n", atual->id);
            printf("Origem: %s\n", atual->origem);
            printf("Destino: %s\n", atual->destino);
            printf("Status: %d\n", atual->status);

            return;
        }

        atual = atual->prox;
    }

    printf("Pacote nao encontrado!\n");
}

void atualizar(int idPacote, int novoStatus) {
    struct No *atual = inicio;

    while (atual != NULL) {
        if (atual->id == idPacote) {
            atual->status = novoStatus;
            return;
        }

        atual = atual->prox;
    }

    printf("Pacote nao encontrado!\n");
}

// Exibir lista
void exibirLista() {
    struct No *atual = inicio;

    if (inicio == NULL) {
        printf("Lista vazia!\n");
        return;
    }

    printf("Lista:\n");
    printf("Status: 1 - Em transito | 2 - Entregue | 3 - Erro\n");

    while (atual != NULL) {
        printf("ID: %d\n", atual->id);
        printf("Origem: %s\n", atual->origem);
        printf("Destino: %s\n", atual->destino);
        printf("Status: %d\n\n", atual->status);
        atual = atual->prox;
    }

    printf("NULL\n");
}