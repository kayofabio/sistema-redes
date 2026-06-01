#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif

#include "rede.h"

void limparTela(void) {
#ifdef _WIN32
    system("cls");
#else
    printf("\033[2J\033[H");
#endif
}

void aguardarMs(int milissegundos) {
#ifdef _WIN32
    Sleep((DWORD) milissegundos);
#else
    usleep((useconds_t) milissegundos * 1000);
#endif
}

void pausarTela(void) {
    printf("\nPressione Enter para continuar...");
    fflush(stdout);

    int c;
    while ((c = getchar()) != '\n' && c != EOF) {
    }
}

void mostrarCabecalho(const char titulo[]) {
    printf("===== %s =====\n\n", titulo);
}

int lerInteiro(const char *rotulo) {
    int valor;

    while (1) {
        printf("%s", rotulo);

        int resultado = scanf("%d", &valor);
        if (resultado == 1) {
            int c;
            while ((c = getchar()) != '\n' && c != EOF) {
            }
            return valor;
        }

        if (resultado == EOF) {
            return 0;
        }

        printf("Entrada invalida. Digite um numero inteiro.\n");

        int c;
        while ((c = getchar()) != '\n' && c != EOF) {
        }
    }
}

void lerTexto(const char *rotulo, char texto[], int tamanho, const char *valorPadrao) {
    printf("%s", rotulo);

    if (fgets(texto, tamanho, stdin) == NULL) {
        strncpy(texto, valorPadrao, tamanho - 1);
        texto[tamanho - 1] = '\0';
        return;
    }

    texto[strcspn(texto, "\n")] = '\0';

    if (texto[0] == '\0') {
        strncpy(texto, valorPadrao, tamanho - 1);
        texto[tamanho - 1] = '\0';
    }
}

static void desenharRota(const char *etapa, const char *marcador, const char *origem, const char *destino) {
    printf("Etapa: %s\n\n", etapa);
    printf("[%s]----[SWITCH-01]----[ROTEADOR-01]----[%s]\n", origem, destino);
    printf("%s\n", marcador);
}

void animarTransmissaoPacote(Pacote pacote, const Dispositivo *origem, const Dispositivo *destino) {
    const char *etapas[] = {
        "Aplicacao monta PDU",
        "Quadro sai da origem",
        "Switch consulta tabela MAC",
        "Roteador avalia proximo salto",
        "Destino recebe e processa"
    };
    const char *marcadores[] = {
        "  <PDU>",
        "      ----<PDU>",
        "                   ----<PDU>",
        "                                  ----<PDU>",
        "                                                   ----<PDU>"
    };

    for (int i = 0; i < 5; i++) {
        limparTela();
        mostrarCabecalho("Animacao de troca de pacote");
        printf("Pacote %d | %d KB | %d ms estimados\n", pacote.numeroPacote, pacote.tamanhoKB, pacote.tempoEstimadoMs);
        printf("Origem: %s (%s)\n", origem->nome, origem->ip);
        printf("Destino: %s (%s)\n\n", destino->nome, destino->ip);
        desenharRota(etapas[i], marcadores[i], origem->nome, destino->nome);
        aguardarMs(550);
    }
}
