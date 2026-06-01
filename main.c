#include <stdio.h>
#include <string.h>

#include "rede.h"

static int proximoId = 1;

static void limparBufferEntrada(void) {
    int c;

    while ((c = getchar()) != '\n' && c != EOF) {
    }
}

static int lerInteiro(const char *rotulo) {
    int valor;

    while (1) {
        printf("%s", rotulo);

        int resultado = scanf("%d", &valor);
        if (resultado == 1) {
            limparBufferEntrada();
            return valor;
        }

        if (resultado == EOF) {
            return 0;
        }

        printf("Entrada invalida. Digite um numero inteiro.\n");
        limparBufferEntrada();
    }
}

static void lerTexto(const char *rotulo, char texto[], int tamanho, const char *valorPadrao) {
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

static void exibirPacote(Pacote pacote) {
    printf("ID: %d\n", pacote.id);
    printf("Pacote: %d\n", pacote.numeroPacote);
    printf("Tamanho: %d KB\n", pacote.tamanhoKB);
    printf("Tempo estimado: %d ms\n", pacote.tempoEstimadoMs);
    printf("Origem: %s\n", pacote.origem);
    printf("Destino: %s\n", pacote.destino);
    printf("Status: %s\n", nomeStatus(pacote.status));
}

static void adicionarPacoteManual(void) {
    if (filaLinearCheia()) {
        printf("Fila cheia. Transmita pacotes antes de cadastrar novos.\n");
        return;
    }

    char origem[TAM_TEXTO];
    char destino[TAM_TEXTO];
    int numeroPacote = lerInteiro("Numero do pacote: ");
    int tamanhoKB = lerInteiro("Tamanho (KB): ");

    lerTexto("Origem [PC-01]: ", origem, TAM_TEXTO, "PC-01");
    lerTexto("Destino [SERVIDOR-01]: ", destino, TAM_TEXTO, "SERVIDOR-01");

    Pacote pacote = montarPacote(proximoId, numeroPacote, tamanhoKB, origem, destino);
    proximoId++;

    enfileirarLinear(pacote);
    inserirPacoteAtivo(pacote);
}

static void transmitirProximoPacote(void) {
    if (filaLinearVazia()) {
        printf("Fila vazia. Nao ha pacote aguardando transmissao.\n");
        return;
    }

    Pacote pacote = desenfileirarLinear();
    if (pacote.id == -1) {
        return;
    }

    atualizarStatusPacote(pacote.numeroPacote, STATUS_EM_TRANSITO);

    printf("\nPacote transmitido pela fila FIFO:\n");
    exibirPacote(pacote);
    printf("Observacao: ele saiu da fila e permanece na lista como pacote ativo.\n");
}

static void registrarErroManual(void) {
    int numeroPacote = lerInteiro("Numero do pacote com erro: ");
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

static void retransmitirUltimoErro(void) {
    if (esta_vazia()) {
        printf("Pilha de erros vazia. Nao ha pacote para retransmitir.\n");
        return;
    }

    Pacote pacote = desempilhar();
    if (pacote.id == -1) {
        return;
    }

    atualizarStatusPacote(pacote.numeroPacote, STATUS_EM_TRANSITO);

    printf("\nPacote retransmitido pela pilha LIFO:\n");
    exibirPacote(pacote);
}

static void buscarPacoteAtivo(void) {
    int numeroPacote = lerInteiro("Numero do pacote para buscar: ");
    Pacote pacote;

    if (!buscarPacotePorNumero(numeroPacote, &pacote)) {
        printf("Pacote %d nao encontrado na lista ativa.\n", numeroPacote);
        return;
    }

    printf("\nPacote encontrado na lista encadeada:\n");
    exibirPacote(pacote);
}

static void marcarEntregueERemover(void) {
    int numeroPacote = lerInteiro("Numero do pacote entregue: ");

    removerPacoteDaFila(numeroPacote, NULL);

    if (!atualizarStatusPacote(numeroPacote, STATUS_ENTREGUE)) {
        printf("Pacote %d nao encontrado na lista ativa.\n", numeroPacote);
        return;
    }

    int resultado = removerPacoteEntregue(numeroPacote);

    if (resultado == 1) {
        printf("Pacote %d entregue e removido da lista ativa.\n", numeroPacote);
    } else if (resultado == -1) {
        printf("Pacote %d ainda nao esta marcado como entregue.\n", numeroPacote);
    } else {
        printf("Pacote %d nao encontrado para remocao.\n", numeroPacote);
    }
}

static void adicionarPacoteCenario(int numeroPacote, int tamanhoKB) {
    Pacote pacote = montarPacote(proximoId, numeroPacote, tamanhoKB, "PC-01", "SERVIDOR-01");
    proximoId++;

    enfileirarLinear(pacote);
    inserirPacoteAtivo(pacote);
}

static void executarCenarioQuestao5(void) {
    printf("\nCenario guiado da Questao 5\n");
    printf("Reiniciando fila, pilha e lista para uma execucao limpa.\n");

    limparFilaLinear();
    limparPilha();
    limparLista();
    proximoId = 1;

    printf("\n1. Chegada dos quatro pacotes do enunciado.\n");
    adicionarPacoteCenario(1, 500);
    adicionarPacoteCenario(2, 300);
    adicionarPacoteCenario(3, 700);
    adicionarPacoteCenario(4, 200);

    exibirFilaLinear();
    exibirLista();

    printf("\n2. Transmissao de dois pacotes pela fila.\n");
    Pacote primeiro = desenfileirarLinear();
    atualizarStatusPacote(primeiro.numeroPacote, STATUS_EM_TRANSITO);
    printf("Primeiro transmitido: Pacote %d.\n", primeiro.numeroPacote);

    atualizarStatusPacote(primeiro.numeroPacote, STATUS_ENTREGUE);
    if (removerPacoteEntregue(primeiro.numeroPacote) == 1) {
        printf("Pacote %d entregue e removido da lista ativa.\n", primeiro.numeroPacote);
    }

    Pacote segundo = desenfileirarLinear();
    atualizarStatusPacote(segundo.numeroPacote, STATUS_EM_TRANSITO);
    printf("Segundo transmitido: Pacote %d.\n", segundo.numeroPacote);

    printf("\n3. Um pacote apresenta erro e vai para a pilha.\n");
    segundo.status = STATUS_ERRO;
    atualizarStatusPacote(segundo.numeroPacote, STATUS_ERRO);
    empilhar(segundo);

    printf("\n4. Estado final da micro maquina.\n");
    exibirFilaLinear();
    mostrar_pilha();
    exibirLista();

    printf("\nResposta direta: o primeiro pacote transmitido e o Pacote 1, pois a fila segue FIFO.\n");
}

static void exibirMenu(void) {
    printf("\n===== MICRO MAQUINA DE PACOTES =====\n");
    printf("1  - Adicionar pacote a fila\n");
    printf("2  - Transmitir proximo pacote\n");
    printf("3  - Registrar pacote com erro\n");
    printf("4  - Retransmitir ultimo erro\n");
    printf("5  - Buscar pacote ativo\n");
    printf("6  - Marcar entregue e remover da lista\n");
    printf("7  - Mostrar fila de transmissao\n");
    printf("8  - Mostrar pilha de erros\n");
    printf("9  - Mostrar lista de pacotes ativos\n");
    printf("10 - Executar cenario guiado da Questao 5\n");
    printf("0  - Sair\n");
}

int main(void) {
    int opcao;

    do {
        exibirMenu();
        opcao = lerInteiro("Escolha:");

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
                buscarPacoteAtivo();
                break;
            case 6:
                marcarEntregueERemover();
                break;
            case 7:
                exibirFilaLinear();
                break;
            case 8:
                mostrar_pilha();
                break;
            case 9:
                exibirLista();
                break;
            case 10:
                executarCenarioQuestao5();
                break;
            case 0:
                printf("Encerrando simulacao.\n");
                break;
            default:
                printf("Opcao invalida.\n");
                break;
        }
    } while (opcao != 0);

    limparFilaLinear();
    limparPilha();
    limparLista();

    return 0;
}
