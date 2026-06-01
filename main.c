#include <stdio.h>

#include "rede.h"

static void exibirMenu(void) {
    mostrarCabecalho("Micro Maquina de Pacotes");
    printf("1  - Cadastrar dispositivo no ambiente\n");
    printf("2  - Mostrar ambiente de rede\n");
    printf("3  - Adicionar pacote a fila\n");
    printf("4  - Transmitir proximo pacote com animacao\n");
    printf("5  - Registrar pacote com erro\n");
    printf("6  - Retransmitir ultimo erro\n");
    printf("7  - Buscar pacote ativo\n");
    printf("8  - Marcar entregue e remover da lista\n");
    printf("9  - Mostrar fila de transmissao\n");
    printf("10 - Mostrar pilha de erros\n");
    printf("11 - Mostrar lista de pacotes ativos\n");
    printf("12 - Executar cenario guiado da Questao 5\n");
    printf("0  - Sair\n");
}

int main(void) {
    int opcao;

    inicializarAmbientePadrao();

    do {
        limparTela();
        exibirMenu();
        opcao = lerInteiro("Escolha:");
        limparTela();

        switch (opcao) {
            case 1:
                cadastrarDispositivoInterativo();
                break;
            case 2:
                mostrarCabecalho("Ambiente de rede");
                listarAmbiente();
                break;
            case 3:
                adicionarPacoteManual();
                break;
            case 4:
                transmitirProximoPacote();
                break;
            case 5:
                registrarErroManual();
                break;
            case 6:
                retransmitirUltimoErro();
                break;
            case 7:
                buscarPacoteAtivo();
                break;
            case 8:
                marcarEntregueERemover();
                break;
            case 9:
                mostrarCabecalho("Fila de transmissao");
                exibirFilaLinear();
                break;
            case 10:
                mostrarCabecalho("Pilha de erros");
                mostrar_pilha();
                break;
            case 11:
                mostrarCabecalho("Lista de pacotes ativos");
                exibirLista();
                break;
            case 12:
                executarCenarioQuestao5();
                break;
            case 0:
                printf("Encerrando simulacao.\n");
                break;
            default:
                printf("Opcao invalida.\n");
                break;
        }

        if (opcao != 0) {
            pausarTela();
        }
    } while (opcao != 0);

    limparFilaLinear();
    limparPilha();
    limparLista();

    return 0;
}
