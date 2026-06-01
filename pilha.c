#include <stdio.h>

#include "rede.h"

static Pacote pilha[MAX_PILHA];
static int topo = -1;

int empilhar(Pacote p) {
    if (esta_cheia()) {
        printf("Pilha de erros cheia. O pacote %d nao foi registrado.\n", p.numeroPacote);
        return 0;
    }

    p.status = STATUS_ERRO;
    topo++;
    pilha[topo] = p;

    printf("Pilha: pacote %d registrado para retransmissao.\n", p.numeroPacote);
    return 1;
}

Pacote desempilhar(void) {
    Pacote pacoteVazio = {-1};

    if (esta_vazia()) {
        printf("Pilha de erros vazia. Nao ha pacote para retransmitir.\n");
        return pacoteVazio;
    }

    Pacote pacote = pilha[topo];
    topo--;
    pacote.status = STATUS_EM_TRANSITO;

    return pacote;
}

int esta_vazia(void) {
    return topo == -1;
}

int esta_cheia(void) {
    return topo == MAX_PILHA - 1;
}

void mostrar_pilha(void) {
    if (esta_vazia()) {
        printf("Pilha de erros vazia.\n");
        return;
    }

    printf("\nPilha de erros (topo para base, ordem LIFO)\n");
    printf("+------------+------------+------------+--------------+-------------+\n");
    printf("| ID         | Pacote     | Tamanho KB | Tempo ms     | Status      |\n");
    printf("+------------+------------+------------+--------------+-------------+\n");

    for (int i = topo; i >= 0; i--) {
        printf("| %10d | %10d | %10d | %12lld | %-11s |\n",
               pilha[i].id,
               pilha[i].numeroPacote,
               pilha[i].tamanhoKB,
               pilha[i].tempoEstimadoMs,
               nomeStatus(pilha[i].status));
    }

    printf("+------------+------------+------------+--------------+-------------+\n");
}

void limparPilha(void) {
    topo = -1;
}
