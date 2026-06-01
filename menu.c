#include <stdio.h>

#include "rede.h"

static void exibirMenuPrincipal(void) {
    mostrarCabecalho("Micro Maquina de Pacotes");
    printf("Demo pronta: Pacotes 1=500KB, 2=300KB, 3=700KB, 4=200KB\n");
    printf("Origem: PC-01 | Destino: app.local | Fluxo: DNS, ARP, roteador e PDU\n\n");
    printf("1 - Rodar demo pronta da Questao 5\n");
    printf("2 - Operar pacotes\n");
    printf("3 - Ambiente de rede\n");
    printf("4 - Consultas e estruturas\n");
    printf("0 - Sair\n");
}

static void exibirMenuPacotes(void) {
    mostrarCabecalho("Operar pacotes");
    printf("1 - Adicionar pacote manualmente\n");
    printf("2 - Transmitir proximo pacote com animacao\n");
    printf("3 - Registrar pacote com erro\n");
    printf("4 - Retransmitir ultimo erro\n");
    printf("5 - Marcar entregue e remover da lista\n");
    printf("0 - Voltar\n");
}

static void exibirMenuAmbiente(void) {
    mostrarCabecalho("Ambiente de rede");
    printf("1 - Mostrar ambiente cadastrado\n");
    printf("2 - Cadastrar dispositivo\n");
    printf("0 - Voltar\n");
}

static void exibirMenuConsultas(void) {
    mostrarCabecalho("Consultas e estruturas");
    printf("1 - Buscar pacote ativo\n");
    printf("2 - Mostrar fila de transmissao\n");
    printf("3 - Mostrar pilha de erros\n");
    printf("4 - Mostrar lista de pacotes ativos\n");
    printf("0 - Voltar\n");
}

static void pausarAposAcao(int opcao) {
    if (opcao != 0) {
        pausarTela();
    }
}

static void mostrarFila(void) {
    mostrarCabecalho("Fila de transmissao");
    exibirFilaLinear();
}

static void mostrarPilha(void) {
    mostrarCabecalho("Pilha de erros");
    mostrar_pilha();
}

static void mostrarLista(void) {
    mostrarCabecalho("Lista de pacotes ativos");
    exibirLista();
}

static void mostrarAmbiente(void) {
    mostrarCabecalho("Ambiente de rede");
    listarAmbiente();
}

static void executarOpcaoPacotes(int opcao) {
    switch (opcao) {
        case 1:
            adicionarPacoteManual();
            break;
        case 2:
            transmitirProximoPacote();
            break;
        case 3:
            registrarErroManual();
            break;
        case 4:
            retransmitirUltimoErro();
            break;
        case 5:
            marcarEntregueERemover();
            break;
        case 0:
            break;
        default:
            printf("Opcao invalida.\n");
            break;
    }
}

static void executarOpcaoAmbiente(int opcao) {
    switch (opcao) {
        case 1:
            mostrarAmbiente();
            break;
        case 2:
            cadastrarDispositivoInterativo();
            break;
        case 0:
            break;
        default:
            printf("Opcao invalida.\n");
            break;
    }
}

static void executarOpcaoConsultas(int opcao) {
    switch (opcao) {
        case 1:
            buscarPacoteAtivo();
            break;
        case 2:
            mostrarFila();
            break;
        case 3:
            mostrarPilha();
            break;
        case 4:
            mostrarLista();
            break;
        case 0:
            break;
        default:
            printf("Opcao invalida.\n");
            break;
    }
}

static void executarMenuPacotes(void) {
    int opcao;

    do {
        limparTela();
        exibirMenuPacotes();
        opcao = lerInteiro("Escolha:");
        limparTela();
        executarOpcaoPacotes(opcao);
        pausarAposAcao(opcao);
    } while (opcao != 0);
}

static void executarMenuAmbiente(void) {
    int opcao;

    do {
        limparTela();
        exibirMenuAmbiente();
        opcao = lerInteiro("Escolha:");
        limparTela();
        executarOpcaoAmbiente(opcao);
        pausarAposAcao(opcao);
    } while (opcao != 0);
}

static void executarMenuConsultas(void) {
    int opcao;

    do {
        limparTela();
        exibirMenuConsultas();
        opcao = lerInteiro("Escolha:");
        limparTela();
        executarOpcaoConsultas(opcao);
        pausarAposAcao(opcao);
    } while (opcao != 0);
}

static int executarOpcaoPrincipal(int opcao) {
    switch (opcao) {
        case 1:
            executarCenarioQuestao5();
            return 1;
        case 2:
            executarMenuPacotes();
            return 0;
        case 3:
            executarMenuAmbiente();
            return 0;
        case 4:
            executarMenuConsultas();
            return 0;
        case 0:
            printf("Encerrando simulacao.\n");
            return 0;
        default:
            printf("Opcao invalida.\n");
            return 1;
    }
}

void executarAplicacao(void) {
    int opcao;

    inicializarAmbientePadrao();

    do {
        limparTela();
        exibirMenuPrincipal();
        opcao = lerInteiro("Escolha:");
        limparTela();

        if (executarOpcaoPrincipal(opcao)) {
            pausarTela();
        }
    } while (opcao != 0);

    limparFilaLinear();
    limparPilha();
    limparLista();
}
