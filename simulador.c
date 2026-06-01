#include <stdio.h>

#include "rede.h"

#define LIMITE_ERRO_CENARIO_KB 500

static int proximoId = 1;

static void exibirPacote(Pacote pacote) {
    printf("ID: %d\n", pacote.id);
    printf("Pacote: %d\n", pacote.numeroPacote);
    printf("Tamanho: %d KB\n", pacote.tamanhoKB);
    printf("Tempo estimado: %lld ms\n", pacote.tempoEstimadoMs);
    printf("Origem: %s\n", pacote.origem);
    printf("Destino: %s\n", pacote.destino);
    printf("Status: %s\n", nomeStatus(pacote.status));
}

static int registrarFalhaPacote(Pacote pacote) {
    pacote.status = STATUS_ERRO;

    if (!empilhar(pacote)) {
        return 0;
    }

    atualizarStatusPacote(pacote.numeroPacote, STATUS_ERRO);
    return 1;
}

static int resolverPacoteParaTransmissao(Pacote pacote, Dispositivo *origem, Dispositivo *destino, TipoRota *tipoRota, char relatorio[]) {
    mostrarCabecalho("Processamento da rede");

    if (!resolverRotaPacote(&pacote, origem, destino, tipoRota, relatorio, TAM_RELATORIO)) {
        printf("%s", relatorio);
        return 0;
    }

    return 1;
}

static void apresentarTransmissaoResolvida(Pacote pacote, const Dispositivo *origem, const Dispositivo *destino, TipoRota tipoRota, const char relatorio[], int simularFalha) {
    printf("%s", relatorio);
    printf("\nRevise a resolucao acima. A proxima tela mostra a animacao da PDU.\n");
    pausarTela();
    animarTransmissaoPacote(pacote, origem, destino, tipoRota);
    limparTela();
    mostrarCabecalho("Resultado da transmissao");

    if (simularFalha) {
        printf("Regra de teste do cenario: pacotes com %d KB ou mais simulam falha.\n", LIMITE_ERRO_CENARIO_KB);
        registrarFalhaPacote(pacote);
        return;
    }

    pacote.status = STATUS_EM_TRANSITO;
    atualizarStatusPacote(pacote.numeroPacote, STATUS_EM_TRANSITO);
    printf("Pacote transmitido.\n\n");
    exibirPacote(pacote);
    printf("\nEle permanece na lista como pacote ativo.\n");
}

void cadastrarDispositivoInterativo(void) {
    char nome[TAM_TEXTO];
    char ip[TAM_IP];
    char mac[TAM_MAC];
    char dominio[TAM_TEXTO];

    mostrarCabecalho("Cadastro de ambiente");
    printf("Tipos: 1 PC | 2 Switch | 3 Roteador | 4 Servidor | 5 DNS\n\n");

    int tipo = lerInteiro("Tipo:");
    if (tipo < TIPO_PC || tipo > TIPO_DNS) {
        printf("Tipo invalido. Cadastro cancelado.\n");
        return;
    }

    lerTexto("Nome:", nome, TAM_TEXTO, "PC-NOVO");
    lerTexto("IP:", ip, TAM_IP, "192.168.10.99");
    lerTexto("MAC:", mac, TAM_MAC, "02:00:00:00:99:99");
    lerTexto("Dominio [-]:", dominio, TAM_TEXTO, "-");

    if (!cadastrarDispositivo(nome, (TipoDispositivo) tipo, ip, mac, dominio)) {
        printf("\nCadastro recusado. Verifique limite do ambiente ou dados duplicados.\n");
        return;
    }

    printf("\nDispositivo cadastrado.\n\n");
    listarAmbiente();
}

void adicionarPacoteManual(void) {
    if (filaLinearCheia()) {
        printf("Fila cheia. Transmita pacotes antes de cadastrar novos.\n");
        return;
    }

    char origem[TAM_TEXTO];
    char destino[TAM_TEXTO];

    mostrarCabecalho("Adicionar pacote");
    listarAmbiente();
    printf("\nUse nome, IP ou dominio cadastrado. Exemplo de destino: app.local.\n\n");

    int numeroPacote = lerInteiro("Numero do pacote:");
    int tamanhoKB = lerInteiro("Tamanho (KB):");

    if (numeroPacote <= 0 || tamanhoKB <= 0) {
        printf("Numero e tamanho do pacote devem ser maiores que zero.\n");
        return;
    }

    if (buscarPacotePorNumero(numeroPacote, NULL)) {
        printf("Pacote %d ja existe na lista ativa. Use outro numero.\n", numeroPacote);
        return;
    }

    lerTexto("Origem [PC-01]:", origem, TAM_TEXTO, "PC-01");
    lerTexto("Destino [app.local]:", destino, TAM_TEXTO, "app.local");

    Pacote pacote = montarPacote(proximoId, numeroPacote, tamanhoKB, origem, destino);

    if (!enfileirarLinear(pacote)) {
        return;
    }

    if (!inserirPacoteAtivo(pacote)) {
        removerPacoteDaFila(numeroPacote, NULL);
        printf("Cadastro desfeito para manter fila e lista consistentes.\n");
        return;
    }

    proximoId++;
}

static void processarProximaTransmissao(int aplicarRegraCenario) {
    Pacote pacote;

    if (!consultarPrimeiroFila(&pacote)) {
        printf("Fila vazia. Nao ha pacote aguardando transmissao.\n");
        return;
    }

    Dispositivo origem;
    Dispositivo destino;
    TipoRota tipoRota;
    char relatorio[TAM_RELATORIO];
    int simularFalha = aplicarRegraCenario && pacote.tamanhoKB >= LIMITE_ERRO_CENARIO_KB;

    if (!resolverPacoteParaTransmissao(pacote, &origem, &destino, &tipoRota, relatorio)) {
        if (!registrarFalhaPacote(pacote)) {
            printf("Pacote %d permaneceu na fila para evitar perda de estado.\n", pacote.numeroPacote);
            return;
        }

        desenfileirarLinear();
        return;
    }

    if (simularFalha && esta_cheia()) {
        printf("Pilha de erros cheia. Pacote %d permaneceu na fila para evitar perda de estado.\n", pacote.numeroPacote);
        return;
    }

    desenfileirarLinear();
    apresentarTransmissaoResolvida(pacote, &origem, &destino, tipoRota, relatorio, simularFalha);
}

void transmitirProximoPacote(void) {
    processarProximaTransmissao(0);
}

void registrarErroManual(void) {
    mostrarCabecalho("Registrar erro");

    int numeroPacote = lerInteiro("Numero do pacote com erro:");
    Pacote pacote;

    if (!buscarPacotePorNumero(numeroPacote, &pacote)) {
        printf("Pacote %d nao encontrado na lista ativa.\n", numeroPacote);
        return;
    }

    if (pacote.status == STATUS_ERRO) {
        printf("Pacote %d ja esta registrado na pilha de erros.\n", numeroPacote);
        return;
    }

    if (!registrarFalhaPacote(pacote)) {
        printf("Pacote %d permaneceu no estado atual para evitar perda de estado.\n", numeroPacote);
        return;
    }

    removerPacoteDaFila(numeroPacote, NULL);
}

void retransmitirUltimoErro(void) {
    if (esta_vazia()) {
        printf("Pilha de erros vazia. Nao ha pacote para retransmitir.\n");
        return;
    }

    Pacote pacote = desempilhar();
    if (pacote.id == -1) {
        return;
    }

    Dispositivo origem;
    Dispositivo destino;
    TipoRota tipoRota;
    char relatorio[TAM_RELATORIO];

    mostrarCabecalho("Retransmissao LIFO");
    printf("Pacote %d retirado do topo da pilha para nova tentativa.\n\n", pacote.numeroPacote);

    if (!resolverPacoteParaTransmissao(pacote, &origem, &destino, &tipoRota, relatorio)) {
        registrarFalhaPacote(pacote);
        return;
    }

    apresentarTransmissaoResolvida(pacote, &origem, &destino, tipoRota, relatorio, 0);
}

void buscarPacoteAtivo(void) {
    mostrarCabecalho("Buscar pacote ativo");

    int numeroPacote = lerInteiro("Numero do pacote para buscar:");
    Pacote pacote;

    if (!buscarPacotePorNumero(numeroPacote, &pacote)) {
        printf("Pacote %d nao encontrado na lista ativa.\n", numeroPacote);
        return;
    }

    printf("\nPacote encontrado na lista encadeada:\n");
    exibirPacote(pacote);
}

void marcarEntregueERemover(void) {
    mostrarCabecalho("Entrega de pacote");

    int numeroPacote = lerInteiro("Numero do pacote entregue:");
    removerPacoteDaFila(numeroPacote, NULL);

    if (!atualizarStatusPacote(numeroPacote, STATUS_ENTREGUE)) {
        printf("Pacote %d nao encontrado na lista ativa.\n", numeroPacote);
        return;
    }

    int resultado = removerPacoteEntregue(numeroPacote);
    if (resultado == 1) {
        printf("Pacote %d entregue e removido da lista ativa.\n", numeroPacote);
    } else {
        printf("Pacote %d nao foi removido.\n", numeroPacote);
    }
}

static void adicionarPacoteCenario(int numeroPacote, int tamanhoKB) {
    Pacote pacote = montarPacote(proximoId, numeroPacote, tamanhoKB, "PC-01", "app.local");

    if (!enfileirarLinear(pacote)) {
        return;
    }

    if (!inserirPacoteAtivo(pacote)) {
        removerPacoteDaFila(numeroPacote, NULL);
        printf("Cadastro desfeito para manter fila e lista consistentes.\n");
        return;
    }

    proximoId++;
}

static void pausarEtapaCenario(void) {
    printf("\nRevise esta etapa do cenario.\n");
    pausarTela();
}

void executarCenarioQuestao5(void) {
    mostrarCabecalho("Cenario guiado da Questao 5");
    printf("Ambiente usado: PC-01 envia pacotes para app.local, resolvido pelo DNS local.\n\n");
    printf("Regra de teste: pacotes com %d KB ou mais simulam falha na primeira transmissao.\n\n", LIMITE_ERRO_CENARIO_KB);

    limparFilaLinear();
    limparPilha();
    limparLista();
    proximoId = 1;

    printf("1. Chegada dos quatro pacotes do enunciado.\n");
    adicionarPacoteCenario(1, 500);
    adicionarPacoteCenario(2, 300);
    adicionarPacoteCenario(3, 700);
    adicionarPacoteCenario(4, 200);
    pausarEtapaCenario();

    printf("\n2. Primeira transmissao com resolucao DNS, ARP e animacao.\n");
    processarProximaTransmissao(1);
    pausarEtapaCenario();

    printf("\n3. Segunda transmissao.\n");
    processarProximaTransmissao(1);

    atualizarStatusPacote(2, STATUS_ENTREGUE);
    if (removerPacoteEntregue(2) == 1) {
        printf("\nPacote 2 entregue e removido da lista ativa.\n");
    }
    pausarEtapaCenario();

    printf("\n4. Estado final da micro maquina.\n");
    exibirFilaLinear();
    mostrar_pilha();
    exibirLista();
    printf("\nResposta direta: o primeiro pacote retirado da fila e o Pacote 1, pois a fila segue FIFO.\n");
}
