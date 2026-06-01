#include <string.h>

#include "rede.h"

#define LINK_KB_POR_SEGUNDO 100

static void copiarTexto(char destino[], const char origem[]) {
    strncpy(destino, origem, TAM_TEXTO - 1);
    destino[TAM_TEXTO - 1] = '\0';
}

const char *nomeStatus(StatusPacote status) {
    switch (status) {
        case STATUS_AGUARDANDO:
            return "aguardando";
        case STATUS_EM_TRANSITO:
            return "em_transito";
        case STATUS_ENTREGUE:
            return "entregue";
        case STATUS_ERRO:
            return "erro";
        default:
            return "desconhecido";
    }
}

long long calcularTempoEstimadoMs(int tamanhoKB) {
    if (tamanhoKB <= 0) {
        return 0;
    }

    return ((long long) tamanhoKB * 1000) / LINK_KB_POR_SEGUNDO;
}

Pacote montarPacote(int id, int numeroPacote, int tamanhoKB, const char origem[], const char destino[]) {
    Pacote pacote;

    pacote.id = id;
    pacote.numeroPacote = numeroPacote;
    pacote.tamanhoKB = tamanhoKB;
    pacote.tempoEstimadoMs = calcularTempoEstimadoMs(tamanhoKB);
    pacote.status = STATUS_AGUARDANDO;
    copiarTexto(pacote.origem, origem);
    copiarTexto(pacote.destino, destino);

    return pacote;
}
