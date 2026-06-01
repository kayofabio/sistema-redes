#include <stdio.h>
#include "rede.h"
int main() {
    int idPacote = 1;
    int opcao;
    Pacote p;

    do
    {
        printf("\n===== Sistema de Rede =====\n");
        printf("1 - Adicionar pacote\n");
        printf("2 - Transmitir proximo pacote\n");
        printf("3 - Retransmitir pacote da pilha de erros\n");
        printf("4 - Mostrar pacotes pendentes (fila)\n");
        printf("5 - Mostrar pacotes em erros (pilha)\n");
        printf("6 - Mostrar pacotes ativos no sistema (lista)\n");
        printf("0 - Sair\n");
        printf("Escolha: ");
        scanf("%d", &opcao);

        switch (opcao)
        {
        case 1:
            printf("Numero do pacote: ");
            scanf("%d", &p.numeroPacote);

            printf("Pacotes com 500 ou mais KB serao enviados para a pilha de erros!\n");
            printf("Tamanho (KB): ");
            scanf("%d", &p.tamanhoKB);

            p.tempoEstimado = p.tamanhoKB * 0.001;
            p.id = idPacote;

            enfileirarLinear(p);
            idPacote++;

            printf("Pacote adicionado com sucesso!\n");
            break;

        case 2:
            if (filaLinearVazia())
            {
                printf("Fila vazia!\n");
            }
            else
            {
                p = desenfileirarLinear();
                if (p.tamanhoKB >= 500) {
                    printf("Erro: Pacote %d muito grande. Sera necessario retransmitir!\n", p.numeroPacote);
                    printf("Pacote %d enviado para pilha de erros!\n", p.numeroPacote);
                } else {
                    printf("Pacote %d transmitido com sucesso!\n", p.numeroPacote);
                    printf("\nPacote transmitido:\n");
                    printf("ID: %d\n", p.id);
                    printf("Numero: %d\n", p.numeroPacote);
                    printf("Tamanho: %d KB\n", p.tamanhoKB);
                    printf("Tempo estimado: %.2f segundos\n", p.tempoEstimado);
                }
            }
            break;

        case 3:
            if (esta_vazia()) {
                printf("Pilha de erros vazia!\n");
            } else {
                p = desempilhar();
                printf("\nPacote retransmitido com sucesso:\n");
                printf("ID: %d\n", p.id);
                printf("Numero: %d\n", p.numeroPacote);
                printf("Tamanho: %d KB\n", p.tamanhoKB);
                printf("Tempo estimado: %.2f segundos\n", p.tempoEstimado);
            }
            break;

        case 4:
            exibirFilaLinear();
            break;

        case 5:
            mostrar_pilha();
            break;

        case 6:
            exibirLista();
            break;

        case 0:
            printf("Encerrando programa...\n");
            break;

        default:
            printf("Opcao invalida!\n");
        }

    } while (opcao != 0);

    return 0;
}