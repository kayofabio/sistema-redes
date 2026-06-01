#include <stdio.h>
#include <stdlib.h>

#include "rede.h"

static No *inicio = NULL;

int inserirPacoteAtivo(Pacote pacote) {
    No *novo = (No *) malloc(sizeof(No));

    if (novo == NULL) {
        printf("Erro de memoria ao inserir o pacote %d na lista.\n", pacote.numeroPacote);
        return 0;
    }

    novo->pacote = pacote;
    novo->prox = NULL;

    if (inicio == NULL) {
        inicio = novo;
        return 1;
    }

    No *atual = inicio;
    while (atual->prox != NULL) {
        atual = atual->prox;
    }

    atual->prox = novo;
    return 1;
}

int buscarPacotePorNumero(int numeroPacote, Pacote *saida) {
    No *atual = inicio;

    while (atual != NULL) {
        if (atual->pacote.numeroPacote == numeroPacote) {
            if (saida != NULL) {
                *saida = atual->pacote;
            }

            return 1;
        }

        atual = atual->prox;
    }

    return 0;
}

int atualizarStatusPacote(int numeroPacote, StatusPacote novoStatus) {
    No *atual = inicio;

    while (atual != NULL) {
        if (atual->pacote.numeroPacote == numeroPacote) {
            atual->pacote.status = novoStatus;
            return 1;
        }

        atual = atual->prox;
    }

    return 0;
}

int removerPacoteEntregue(int numeroPacote) {
    No *atual = inicio;
    No *anterior = NULL;

    while (atual != NULL && atual->pacote.numeroPacote != numeroPacote) {
        anterior = atual;
        atual = atual->prox;
    }

    if (atual == NULL) {
        return 0;
    }

    if (atual->pacote.status != STATUS_ENTREGUE) {
        return -1;
    }

    if (anterior == NULL) {
        inicio = atual->prox;
    } else {
        anterior->prox = atual->prox;
    }

    free(atual);
    return 1;
}

void exibirLista(void) {
    No *atual = inicio;

    if (inicio == NULL) {
        printf("Lista de pacotes ativos vazia.\n");
        return;
    }

    printf("\nLista encadeada de pacotes ativos\n");
    printf("+------------+------------+--------------------+--------------------+-------------+\n");
    printf("| ID         | Pacote     | Origem             | Destino            | Status      |\n");
    printf("+------------+------------+--------------------+--------------------+-------------+\n");

    while (atual != NULL) {
        char origem[19];
        char destino[19];
        resumirTexto(atual->pacote.origem, origem, (int) sizeof(origem));
        resumirTexto(atual->pacote.destino, destino, (int) sizeof(destino));

        printf("| %10d | %10d | %-18s | %-18s | %-11s |\n",
               atual->pacote.id,
               atual->pacote.numeroPacote,
               origem,
               destino,
               nomeStatus(atual->pacote.status));

        atual = atual->prox;
    }

    printf("+------------+------------+--------------------+--------------------+-------------+\n");
}

void limparLista(void) {
    No *atual = inicio;

    while (atual != NULL) {
        No *proximo = atual->prox;
        free(atual);
        atual = proximo;
    }

    inicio = NULL;
}
