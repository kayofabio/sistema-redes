#include <stdio.h>

#include "rede.h"

static Pacote fila[MAX_FILA];
static int quantidadeFila = 0;

int filaLinearVazia(void) {
    return quantidadeFila == 0;
}

int filaLinearCheia(void) {
    return quantidadeFila == MAX_FILA;
}

int enfileirarLinear(Pacote pacote) {
    if (filaLinearCheia()) {
        printf("Fila cheia. Nao foi possivel adicionar o pacote %d.\n", pacote.numeroPacote);
        return 0;
    }

    pacote.status = STATUS_AGUARDANDO;
    fila[quantidadeFila] = pacote;
    quantidadeFila++;

    printf("Fila: pacote %d aguardando transmissao (%d KB, %lld ms estimados).\n",
           pacote.numeroPacote,
           pacote.tamanhoKB,
           pacote.tempoEstimadoMs);

    return 1;
}

int consultarPrimeiroFila(Pacote *saida) {
    if (filaLinearVazia()) {
        return 0;
    }

    if (saida != NULL) {
        *saida = fila[0];
    }

    return 1;
}

Pacote desenfileirarLinear(void) {
    Pacote pacoteVazio = {-1};

    if (filaLinearVazia()) {
        printf("Fila vazia. Nao ha pacote para transmitir.\n");
        return pacoteVazio;
    }

    Pacote pacote = fila[0];
    pacote.status = STATUS_EM_TRANSITO;

    for (int i = 1; i < quantidadeFila; i++) {
        fila[i - 1] = fila[i];
    }

    quantidadeFila--;
    return pacote;
}

int removerPacoteDaFila(int numeroPacote, Pacote *saida) {
    int indice = -1;

    for (int i = 0; i < quantidadeFila; i++) {
        if (fila[i].numeroPacote == numeroPacote) {
            indice = i;
            break;
        }
    }

    if (indice == -1) {
        return 0;
    }

    if (saida != NULL) {
        *saida = fila[indice];
    }

    for (int i = indice + 1; i < quantidadeFila; i++) {
        fila[i - 1] = fila[i];
    }

    quantidadeFila--;
    return 1;
}

void exibirFilaLinear(void) {
    if (filaLinearVazia()) {
        printf("Fila de transmissao vazia.\n");
        return;
    }

    printf("\nFila de transmissao (ordem FIFO)\n");
    printf("+------------+------------+------------+--------------+-------------+\n");
    printf("| ID         | Pacote     | Tamanho KB | Tempo ms     | Status      |\n");
    printf("+------------+------------+------------+--------------+-------------+\n");

    for (int i = 0; i < quantidadeFila; i++) {
        printf("| %10d | %10d | %10d | %12lld | %-11s |\n",
               fila[i].id,
               fila[i].numeroPacote,
               fila[i].tamanhoKB,
               fila[i].tempoEstimadoMs,
               nomeStatus(fila[i].status));
    }

    printf("+------------+------------+------------+--------------+-------------+\n");
}

void limparFilaLinear(void) {
    quantidadeFila = 0;
}
