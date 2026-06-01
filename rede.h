#ifndef REDE_H
#define REDE_H

#define TAM_TEXTO 50
#define MAX_FILA 20
#define MAX_PILHA 20
#define MAX_DISPOSITIVOS 12
#define TAM_IP 16
#define TAM_MAC 18
#define TAM_RELATORIO 1200

typedef enum {
    TIPO_PC = 1,
    TIPO_SWITCH,
    TIPO_ROTEADOR,
    TIPO_SERVIDOR,
    TIPO_DNS
} TipoDispositivo;

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

typedef struct {
    char nome[TAM_TEXTO];
    TipoDispositivo tipo;
    char ip[TAM_IP];
    char mac[TAM_MAC];
    char dominio[TAM_TEXTO];
} Dispositivo;

const char *nomeStatus(StatusPacote status);
int calcularTempoEstimadoMs(int tamanhoKB);
Pacote montarPacote(int id, int numeroPacote, int tamanhoKB, const char origem[], const char destino[]);

void limparTela(void);
void pausarTela(void);
void aguardarMs(int milissegundos);
void mostrarCabecalho(const char titulo[]);
int lerInteiro(const char *rotulo);
void lerTexto(const char *rotulo, char texto[], int tamanho, const char *valorPadrao);
void animarTransmissaoPacote(Pacote pacote, const Dispositivo *origem, const Dispositivo *destino);

const char *nomeTipoDispositivo(TipoDispositivo tipo);
void inicializarAmbientePadrao(void);
int cadastrarDispositivo(const char nome[], TipoDispositivo tipo, const char ip[], const char mac[], const char dominio[]);
void listarAmbiente(void);
int resolverRotaPacote(const Pacote *pacote, Dispositivo *origem, Dispositivo *destino, char relatorio[], int tamanhoRelatorio);

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

void cadastrarDispositivoInterativo(void);
void adicionarPacoteManual(void);
void transmitirProximoPacote(void);
void registrarErroManual(void);
void retransmitirUltimoErro(void);
void buscarPacoteAtivo(void);
void marcarEntregueERemover(void);
void executarCenarioQuestao5(void);

#endif
