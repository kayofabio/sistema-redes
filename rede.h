#ifndef REDE_H
#define REDE_H

#define TAM_TEXTO 50
#define MAX_FILA 20
#define MAX_PILHA 20

typedef enum {
    STATUS_AGUARDANDO = 1,
    STATUS_EM_TRANSITO,
    STATUS_ENTREGUE,
    STATUS_ERRO
} StatusPacote;

typedef struct {
    int id;
    int numeroPacote;
    int tamanhoKB;
    int tempoEstimadoMs;
    char origem[TAM_TEXTO];
    char destino[TAM_TEXTO];
    StatusPacote status;
} Pacote;

typedef struct No {
    Pacote pacote;
    struct No *prox;
} No;

const char *nomeStatus(StatusPacote status);
int calcularTempoEstimadoMs(int tamanhoKB);
Pacote montarPacote(int id, int numeroPacote, int tamanhoKB, const char origem[], const char destino[]);

void enfileirarLinear(Pacote pacote);
Pacote desenfileirarLinear(void);
int removerPacoteDaFila(int numeroPacote, Pacote *saida);
void exibirFilaLinear(void);
int filaLinearVazia(void);
int filaLinearCheia(void);
void limparFilaLinear(void);

void empilhar(Pacote p);
Pacote desempilhar(void);
void mostrar_pilha(void);
int esta_vazia(void);
int esta_cheia(void);
void limparPilha(void);

void inserirPacoteAtivo(Pacote pacote);
int buscarPacotePorNumero(int numeroPacote, Pacote *saida);
int atualizarStatusPacote(int numeroPacote, StatusPacote novoStatus);
int removerPacoteEntregue(int numeroPacote);
void exibirLista(void);
void limparLista(void);

#endif
