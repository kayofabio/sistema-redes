#include <ctype.h>
#include <errno.h>
#include <limits.h>
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
    HANDLE saida = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO info;
    COORD origem = {0, 0};
    DWORD escritos;

    if (saida != INVALID_HANDLE_VALUE && GetConsoleScreenBufferInfo(saida, &info)) {
        DWORD celulas = (DWORD) info.dwSize.X * (DWORD) info.dwSize.Y;
        FillConsoleOutputCharacter(saida, ' ', celulas, origem, &escritos);
        FillConsoleOutputAttribute(saida, info.wAttributes, celulas, origem, &escritos);
        SetConsoleCursorPosition(saida, origem);
    }
#else
    printf("\033[2J\033[H");
#endif
    fflush(stdout);
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

static int concluirLinha(void) {
    int c = getchar();
    if (c == '\n' || c == EOF) {
        return 1;
    }

    while ((c = getchar()) != '\n' && c != EOF) {
    }

    return 0;
}

static int lerLinha(char texto[], int tamanho) {
    if (fgets(texto, tamanho, stdin) == NULL) {
        return 0;
    }

    char *novaLinha = strchr(texto, '\n');
    if (novaLinha != NULL) {
        *novaLinha = '\0';
        return 1;
    }

    return concluirLinha() ? 1 : -1;
}

int lerInteiro(const char *rotulo) {
    char entrada[64];

    while (1) {
        printf("%s", rotulo);

        int resultado = lerLinha(entrada, (int) sizeof(entrada));
        if (resultado == 0) {
            return 0;
        }

        if (resultado == 1) {
            char *fim;
            errno = 0;
            long valor = strtol(entrada, &fim, 10);

            while (isspace((unsigned char) *fim)) {
                fim++;
            }

            if (fim != entrada && *fim == '\0' && errno != ERANGE && valor >= INT_MIN && valor <= INT_MAX) {
                return (int) valor;
            }
        }

        printf("Entrada invalida. Digite um numero inteiro.\n");
    }
}

void lerTexto(const char *rotulo, char texto[], int tamanho, const char *valorPadrao) {
    while (1) {
        printf("%s", rotulo);

        int resultado = lerLinha(texto, tamanho);
        if (resultado == 0) {
            strncpy(texto, valorPadrao, tamanho - 1);
            texto[tamanho - 1] = '\0';
            return;
        }

        if (resultado == -1) {
            printf("Texto muito longo. Digite no maximo %d caracteres.\n", tamanho - 1);
            continue;
        }

        if (texto[0] == '\0') {
            strncpy(texto, valorPadrao, tamanho - 1);
            texto[tamanho - 1] = '\0';
        }

        return;
    }
}

void resumirTexto(const char texto[], char resumo[], int tamanho) {
    int comprimento = (int) strlen(texto);

    if (comprimento < tamanho) {
        strncpy(resumo, texto, tamanho - 1);
        resumo[tamanho - 1] = '\0';
        return;
    }

    strncpy(resumo, texto, tamanho - 4);
    strcpy(resumo + tamanho - 4, "...");
}

static void desenharRota(const char *etapa, const char *marcador, const char *origem, const char *destino, TipoRota tipoRota) {
    printf("Etapa: %s\n\n", etapa);

    if (tipoRota == ROTA_LOCAL) {
        printf("[%s]----[SWITCH-01]----[%s]\n", origem, destino);
    } else {
        printf("[%s]----[SWITCH-01]----[ROTEADOR-01]----[%s]\n", origem, destino);
    }

    printf("%s\n", marcador);
}

void animarTransmissaoPacote(Pacote pacote, const Dispositivo *origem, const Dispositivo *destino, TipoRota tipoRota) {
    const char *etapasRoteador[] = {
        "Aplicacao monta PDU",
        "Quadro sai da origem",
        "Switch consulta tabela MAC",
        "Roteador avalia proximo salto",
        "Destino recebe e processa"
    };
    const char *marcadoresRoteador[] = {
        "  <PDU>",
        "      ----<PDU>",
        "                   ----<PDU>",
        "                                  ----<PDU>",
        "                                                   ----<PDU>"
    };
    const char *etapasLocais[] = {
        "Aplicacao monta PDU",
        "Quadro sai da origem",
        "Switch consulta tabela MAC",
        "Destino recebe e processa"
    };
    const char *marcadoresLocais[] = {
        "  <PDU>",
        "      ----<PDU>",
        "                   ----<PDU>",
        "                                  ----<PDU>"
    };
    const char **etapas = tipoRota == ROTA_LOCAL ? etapasLocais : etapasRoteador;
    const char **marcadores = tipoRota == ROTA_LOCAL ? marcadoresLocais : marcadoresRoteador;
    int totalEtapas = tipoRota == ROTA_LOCAL ? 4 : 5;

    for (int i = 0; i < totalEtapas; i++) {
        limparTela();
        mostrarCabecalho("Animacao de troca de pacote");
        printf("Pacote %d | %d KB | %lld ms estimados\n", pacote.numeroPacote, pacote.tamanhoKB, pacote.tempoEstimadoMs);
        printf("Origem: %s (%s)\n", origem->nome, origem->ip);
        printf("Destino: %s (%s)\n\n", destino->nome, destino->ip);
        desenharRota(etapas[i], marcadores[i], origem->nome, destino->nome, tipoRota);
        aguardarMs(550);
    }
}
