#include <stdio.h>

#include "rede.h"

static int proximoId = 1;

static void exibirPacote(Pacote pacote) {
    printf("ID: %d\n", pacote.id);
    printf("Pacote: %d\n", pacote.numeroPacote);
    printf("Tamanho: %d KB\n", pacote.tamanhoKB);
    printf("Tempo estimado: %d ms\n", pacote.tempoEstimadoMs);
    printf("Origem: %s\n", pacote.origem);
    printf("Destino: %s\n", pacote.destino);
    printf("Status: %s\n", nomeStatus(pacote.status));
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
        printf("\nAmbiente cheio. Remova a ampliacao de escopo ou reinicie com menos dispositivos.\n");
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
    lerTexto("Origem [PC-01]:", origem, TAM_TEXTO, "PC-01");
    lerTexto("Destino [app.local]:", destino, TAM_TEXTO, "app.local");

    Pacote pacote = montarPacote(proximoId, numeroPacote, tamanhoKB, origem, destino);
    proximoId++;

    enfileirarLinear(pacote);
    inserirPacoteAtivo(pacote);
}

void transmitirProximoPacote(void) {
    if (filaLinearVazia()) {
        printf("Fila vazia. Nao ha pacote aguardando transmissao.\n");
        return;
    }

    Dispositivo origem;
    Dispositivo destino;
    char relatorio[TAM_RELATORIO];
    Pacote pacote = desenfileirarLinear();

    if (pacote.id == -1) {
        return;
    }

    mostrarCabecalho("Processamento da rede");

    if (!resolverRotaPacote(&pacote, &origem, &destino, relatorio, TAM_RELATORIO)) {
        printf("%s", relatorio);
        pacote.status = STATUS_ERRO;
        atualizarStatusPacote(pacote.numeroPacote, STATUS_ERRO);
        empilhar(pacote);
        return;
    }

    printf("%s", relatorio);
    aguardarMs(1600);
    animarTransmissaoPacote(pacote, &origem, &destino);
    limparTela();
    mostrarCabecalho("Resultado da transmissao");

    atualizarStatusPacote(pacote.numeroPacote, STATUS_EM_TRANSITO);
    printf("Pacote transmitido pela fila FIFO.\n\n");
    exibirPacote(pacote);
    printf("\nEle saiu da fila e permanece na lista como pacote ativo.\n");
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

    pacote.status = STATUS_ERRO;
    removerPacoteDaFila(numeroPacote, NULL);
    atualizarStatusPacote(numeroPacote, STATUS_ERRO);
    empilhar(pacote);
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

    atualizarStatusPacote(pacote.numeroPacote, STATUS_EM_TRANSITO);
    mostrarCabecalho("Retransmissao LIFO");
    printf("Pacote retransmitido pela pilha LIFO.\n\n");
    exibirPacote(pacote);
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
    proximoId++;
    enfileirarLinear(pacote);
    inserirPacoteAtivo(pacote);
}

void executarCenarioQuestao5(void) {
    mostrarCabecalho("Cenario guiado da Questao 5");
    printf("Ambiente usado: PC-01 envia pacotes para app.local, resolvido pelo DNS local.\n\n");

    limparFilaLinear();
    limparPilha();
    limparLista();
    proximoId = 1;

    printf("1. Chegada dos quatro pacotes do enunciado.\n");
    adicionarPacoteCenario(1, 500);
    adicionarPacoteCenario(2, 300);
    adicionarPacoteCenario(3, 700);
    adicionarPacoteCenario(4, 200);

    printf("\n2. Primeira transmissao com resolucao DNS, ARP e animacao.\n");
    transmitirProximoPacote();

    atualizarStatusPacote(1, STATUS_ENTREGUE);
    if (removerPacoteEntregue(1) == 1) {
        printf("\nPacote 1 entregue e removido da lista ativa.\n");
    }

    printf("\n3. Segunda transmissao.\n");
    transmitirProximoPacote();

    printf("\n4. Simulacao de erro no Pacote 2.\n");
    Pacote pacoteErro;
    if (buscarPacotePorNumero(2, &pacoteErro)) {
        pacoteErro.status = STATUS_ERRO;
        atualizarStatusPacote(2, STATUS_ERRO);
        empilhar(pacoteErro);
    }

    printf("\n5. Estado final da micro maquina.\n");
    exibirFilaLinear();
    mostrar_pilha();
    exibirLista();
    printf("\nResposta direta: o primeiro pacote transmitido e o Pacote 1, pois a fila segue FIFO.\n");
}
