#include <ctype.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>

#include "rede.h"

static Dispositivo dispositivos[MAX_DISPOSITIVOS];
static int totalDispositivos = 0;

static void copiarCampo(char destino[], int tamanho, const char origem[]) {
    strncpy(destino, origem, tamanho - 1);
    destino[tamanho - 1] = '\0';
}

static int textosIguais(const char a[], const char b[]) {
    int i = 0;

    while (a[i] != '\0' && b[i] != '\0') {
        if (tolower((unsigned char) a[i]) != tolower((unsigned char) b[i])) {
            return 0;
        }
        i++;
    }

    return a[i] == '\0' && b[i] == '\0';
}

static int dominioValido(const char dominio[]) {
    return dominio[0] != '\0' && !textosIguais(dominio, "-");
}

static void anexarRelatorio(char relatorio[], int tamanho, const char *formato, ...) {
    int usado = (int) strlen(relatorio);
    if (usado >= tamanho - 1) {
        return;
    }

    va_list args;
    va_start(args, formato);
    vsnprintf(relatorio + usado, (size_t) (tamanho - usado), formato, args);
    va_end(args);
}

static int buscarPorEntrada(const char entrada[], Dispositivo *saida, char modo[]) {
    for (int i = 0; i < totalDispositivos; i++) {
        if (textosIguais(entrada, dispositivos[i].nome)) {
            *saida = dispositivos[i];
            copiarCampo(modo, TAM_TEXTO, "nome");
            return 1;
        }

        if (textosIguais(entrada, dispositivos[i].ip)) {
            *saida = dispositivos[i];
            copiarCampo(modo, TAM_TEXTO, "ip");
            return 1;
        }

        if (dominioValido(dispositivos[i].dominio) && textosIguais(entrada, dispositivos[i].dominio)) {
            *saida = dispositivos[i];
            copiarCampo(modo, TAM_TEXTO, "dominio");
            return 1;
        }
    }

    return 0;
}

static int buscarPrimeiroTipo(TipoDispositivo tipo, Dispositivo *saida) {
    for (int i = 0; i < totalDispositivos; i++) {
        if (dispositivos[i].tipo == tipo) {
            *saida = dispositivos[i];
            return 1;
        }
    }

    return 0;
}

static int mesmaRede24(const char ipA[], const char ipB[]) {
    int a1, a2, a3, a4;
    int b1, b2, b3, b4;

    if (sscanf(ipA, "%d.%d.%d.%d", &a1, &a2, &a3, &a4) != 4) {
        return 0;
    }

    if (sscanf(ipB, "%d.%d.%d.%d", &b1, &b2, &b3, &b4) != 4) {
        return 0;
    }

    return a1 == b1 && a2 == b2 && a3 == b3;
}

const char *nomeTipoDispositivo(TipoDispositivo tipo) {
    switch (tipo) {
        case TIPO_PC:
            return "PC";
        case TIPO_SWITCH:
            return "Switch";
        case TIPO_ROTEADOR:
            return "Roteador";
        case TIPO_SERVIDOR:
            return "Servidor";
        case TIPO_DNS:
            return "DNS";
        default:
            return "Desconhecido";
    }
}

int cadastrarDispositivo(const char nome[], TipoDispositivo tipo, const char ip[], const char mac[], const char dominio[]) {
    if (totalDispositivos == MAX_DISPOSITIVOS) {
        return 0;
    }

    copiarCampo(dispositivos[totalDispositivos].nome, TAM_TEXTO, nome);
    copiarCampo(dispositivos[totalDispositivos].ip, TAM_IP, ip);
    copiarCampo(dispositivos[totalDispositivos].mac, TAM_MAC, mac);
    copiarCampo(dispositivos[totalDispositivos].dominio, TAM_TEXTO, dominio);
    dispositivos[totalDispositivos].tipo = tipo;
    totalDispositivos++;

    return 1;
}

void inicializarAmbientePadrao(void) {
    totalDispositivos = 0;
    cadastrarDispositivo("PC-01", TIPO_PC, "192.168.10.10", "02:00:00:00:10:10", "-");
    cadastrarDispositivo("SWITCH-01", TIPO_SWITCH, "0.0.0.0", "02:00:00:00:00:02", "-");
    cadastrarDispositivo("ROTEADOR-01", TIPO_ROTEADOR, "192.168.10.1", "02:00:00:00:10:01", "-");
    cadastrarDispositivo("DNS-01", TIPO_DNS, "192.168.10.53", "02:00:00:00:10:53", "dns.local");
    cadastrarDispositivo("SERVIDOR-01", TIPO_SERVIDOR, "192.168.20.20", "02:00:00:00:20:20", "app.local");
}

void listarAmbiente(void) {
    if (totalDispositivos == 0) {
        printf("Nenhum dispositivo cadastrado.\n");
        return;
    }

    printf("+-------------+----------+----------------+-------------------+-------------+\n");
    printf("| Nome        | Tipo     | IP             | MAC               | Dominio     |\n");
    printf("+-------------+----------+----------------+-------------------+-------------+\n");

    for (int i = 0; i < totalDispositivos; i++) {
        printf("| %-11s | %-8s | %-14s | %-17s | %-11s |\n",
               dispositivos[i].nome,
               nomeTipoDispositivo(dispositivos[i].tipo),
               dispositivos[i].ip,
               dispositivos[i].mac,
               dispositivos[i].dominio);
    }

    printf("+-------------+----------+----------------+-------------------+-------------+\n");
}

int resolverRotaPacote(const Pacote *pacote, Dispositivo *origem, Dispositivo *destino, char relatorio[], int tamanhoRelatorio) {
    char modoOrigem[TAM_TEXTO] = "";
    char modoDestino[TAM_TEXTO] = "";
    Dispositivo dns;
    Dispositivo roteador;

    relatorio[0] = '\0';

    if (!buscarPorEntrada(pacote->origem, origem, modoOrigem)) {
        snprintf(relatorio, (size_t) tamanhoRelatorio, "Origem `%s` nao encontrada no ambiente.\n", pacote->origem);
        return 0;
    }

    if (!buscarPorEntrada(pacote->destino, destino, modoDestino)) {
        snprintf(relatorio, (size_t) tamanhoRelatorio, "Destino `%s` nao encontrado por nome, IP ou dominio.\n", pacote->destino);
        return 0;
    }

    anexarRelatorio(relatorio, tamanhoRelatorio, "1. Host de origem localizado: %s (%s, %s).\n", origem->nome, origem->ip, origem->mac);

    if (textosIguais(modoDestino, "dominio")) {
        if (buscarPrimeiroTipo(TIPO_DNS, &dns)) {
            anexarRelatorio(relatorio, tamanhoRelatorio,
                            "2. Consulta DNS: %s pergunta ao %s por `%s`; resposta: %s.\n",
                            origem->nome,
                            dns.nome,
                            pacote->destino,
                            destino->ip);
        } else {
            anexarRelatorio(relatorio, tamanhoRelatorio,
                            "2. Consulta DNS simulada: `%s` foi resolvido para %s.\n",
                            pacote->destino,
                            destino->ip);
        }
    } else {
        anexarRelatorio(relatorio, tamanhoRelatorio,
                        "2. Destino informado por %s: %s (%s).\n",
                        modoDestino,
                        destino->nome,
                        destino->ip);
    }

    if (mesmaRede24(origem->ip, destino->ip)) {
        anexarRelatorio(relatorio, tamanhoRelatorio,
                        "3. ARP local: %s resolve %s para o MAC %s.\n",
                        origem->nome,
                        destino->ip,
                        destino->mac);
        anexarRelatorio(relatorio, tamanhoRelatorio,
                        "4. Encaminhamento: quadro passa pelo switch ate %s.\n",
                        destino->nome);
    } else if (buscarPrimeiroTipo(TIPO_ROTEADOR, &roteador)) {
        anexarRelatorio(relatorio, tamanhoRelatorio,
                        "3. Sub-redes diferentes: %s envia para o gateway %s (%s).\n",
                        origem->nome,
                        roteador.nome,
                        roteador.ip);
        anexarRelatorio(relatorio, tamanhoRelatorio,
                        "4. ARP do proximo salto: gateway %s usa MAC %s.\n",
                        roteador.nome,
                        roteador.mac);
        anexarRelatorio(relatorio, tamanhoRelatorio,
                        "5. Roteador encaminha o pacote para a rede do destino %s.\n",
                        destino->nome);
    } else {
        anexarRelatorio(relatorio, tamanhoRelatorio,
                        "3. Destino esta fora da sub-rede e nao ha roteador cadastrado.\n");
        return 0;
    }

    return 1;
}
