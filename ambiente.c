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

static int campoPreenchido(const char texto[]) {
    return texto[0] != '\0';
}

static int tipoValido(TipoDispositivo tipo) {
    return tipo >= TIPO_PC && tipo <= TIPO_DNS;
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
            if (modo != NULL) {
                copiarCampo(modo, TAM_TEXTO, "nome");
            }
            return 1;
        }

        if (textosIguais(entrada, dispositivos[i].ip)) {
            *saida = dispositivos[i];
            if (modo != NULL) {
                copiarCampo(modo, TAM_TEXTO, "ip");
            }
            return 1;
        }

        if (dominioValido(dispositivos[i].dominio) && textosIguais(entrada, dispositivos[i].dominio)) {
            *saida = dispositivos[i];
            if (modo != NULL) {
                copiarCampo(modo, TAM_TEXTO, "dominio");
            }
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

static int lerIpv4(const char ip[], int octetos[4]) {
    char sobra;

    if (sscanf(ip,
               "%d.%d.%d.%d%c",
               &octetos[0],
               &octetos[1],
               &octetos[2],
               &octetos[3],
               &sobra) != 4) {
        return 0;
    }

    for (int i = 0; i < 4; i++) {
        if (octetos[i] < 0 || octetos[i] > 255) {
            return 0;
        }
    }

    return 1;
}

static int mesmaRede24(const char ipA[], const char ipB[]) {
    int a[4];
    int b[4];

    if (!lerIpv4(ipA, a)) {
        return 0;
    }

    if (!lerIpv4(ipB, b)) {
        return 0;
    }

    return a[0] == b[0] && a[1] == b[1] && a[2] == b[2];
}

static int macValido(const char mac[]) {
    if (strlen(mac) != TAM_MAC - 1) {
        return 0;
    }

    for (int i = 0; i < TAM_MAC - 1; i++) {
        if ((i + 1) % 3 == 0) {
            if (mac[i] != ':') {
                return 0;
            }
        } else if (!isxdigit((unsigned char) mac[i])) {
            return 0;
        }
    }

    return 1;
}

static int dominioCadastroValido(const char dominio[]) {
    if (!dominioValido(dominio)) {
        return textosIguais(dominio, "-");
    }

    for (int i = 0; dominio[i] != '\0'; i++) {
        unsigned char c = (unsigned char) dominio[i];
        if (!isalnum(c) && c != '.' && c != '-') {
            return 0;
        }
    }

    return 1;
}

static int dispositivoJaCadastrado(const char nome[], const char ip[], const char mac[], const char dominio[]) {
    for (int i = 0; i < totalDispositivos; i++) {
        if (textosIguais(dispositivos[i].nome, nome)
            || textosIguais(dispositivos[i].ip, ip)
            || textosIguais(dispositivos[i].mac, mac)) {
            return 1;
        }

        if (dominioValido(dominio) && textosIguais(dispositivos[i].dominio, dominio)) {
            return 1;
        }
    }

    return 0;
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
    int octetos[4];

    if (totalDispositivos == MAX_DISPOSITIVOS) {
        return 0;
    }

    if (!campoPreenchido(nome)
        || !tipoValido(tipo)
        || !lerIpv4(ip, octetos)
        || !macValido(mac)
        || !dominioCadastroValido(dominio)) {
        return 0;
    }

    if (dispositivoJaCadastrado(nome, ip, mac, dominio)) {
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

    printf("+-----------------+----------+----------------+-------------------+-----------------+\n");
    printf("| Nome            | Tipo     | IP             | MAC               | Dominio         |\n");
    printf("+-----------------+----------+----------------+-------------------+-----------------+\n");

    for (int i = 0; i < totalDispositivos; i++) {
        char nome[16];
        char dominio[16];
        resumirTexto(dispositivos[i].nome, nome, (int) sizeof(nome));
        resumirTexto(dispositivos[i].dominio, dominio, (int) sizeof(dominio));

        printf("| %-15s | %-8s | %-14s | %-17s | %-15s |\n",
               nome,
               nomeTipoDispositivo(dispositivos[i].tipo),
               dispositivos[i].ip,
               dispositivos[i].mac,
               dominio);
    }

    printf("+-----------------+----------+----------------+-------------------+-----------------+\n");
}

static void relatarResolucaoDestino(const Pacote *pacote, const Dispositivo *origem, const Dispositivo *destino, const char modoDestino[], char relatorio[], int tamanhoRelatorio) {
    Dispositivo dns;

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
}

static int relatarEncaminhamento(const Dispositivo *origem, const Dispositivo *destino, TipoRota *tipoRota, char relatorio[], int tamanhoRelatorio) {
    Dispositivo roteador;

    if (mesmaRede24(origem->ip, destino->ip)) {
        *tipoRota = ROTA_LOCAL;
        anexarRelatorio(relatorio, tamanhoRelatorio,
                        "3. ARP local: %s resolve %s para o MAC %s.\n",
                        origem->nome,
                        destino->ip,
                        destino->mac);
        anexarRelatorio(relatorio, tamanhoRelatorio,
                        "4. Encaminhamento: quadro passa pelo switch ate %s.\n",
                        destino->nome);
        return 1;
    }

    if (buscarPrimeiroTipo(TIPO_ROTEADOR, &roteador)) {
        *tipoRota = ROTA_VIA_ROTEADOR;
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
        return 1;
    }

    anexarRelatorio(relatorio, tamanhoRelatorio,
                    "3. Destino esta fora da sub-rede e nao ha roteador cadastrado.\n");
    return 0;
}

int resolverRotaPacote(const Pacote *pacote, Dispositivo *origem, Dispositivo *destino, TipoRota *tipoRota, char relatorio[], int tamanhoRelatorio) {
    char modoDestino[TAM_TEXTO] = "";

    relatorio[0] = '\0';

    if (!buscarPorEntrada(pacote->origem, origem, NULL)) {
        snprintf(relatorio, (size_t) tamanhoRelatorio, "Origem `%s` nao encontrada no ambiente.\n", pacote->origem);
        return 0;
    }

    if (!buscarPorEntrada(pacote->destino, destino, modoDestino)) {
        snprintf(relatorio, (size_t) tamanhoRelatorio, "Destino `%s` nao encontrado por nome, IP ou dominio.\n", pacote->destino);
        return 0;
    }

    anexarRelatorio(relatorio, tamanhoRelatorio, "1. Host de origem localizado: %s (%s, %s).\n", origem->nome, origem->ip, origem->mac);
    relatarResolucaoDestino(pacote, origem, destino, modoDestino, relatorio, tamanhoRelatorio);
    return relatarEncaminhamento(origem, destino, tipoRota, relatorio, tamanhoRelatorio);
}
