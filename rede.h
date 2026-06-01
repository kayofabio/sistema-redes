#ifndef REDE_H
#define REDE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Fila  e Pilha */
typedef struct {
    int id;
    int numeroPacote;
    int tamanhoKB;
    float tempoEstimado;
} Pacote;

/* Lista Encadeada */
typedef struct No {
    int id;
    char origem[50];
    char destino[50];
    int status; // 1 = em trânsito | 2 = entregue | 3 = cancelado
    struct No *prox;
} No;

/* Lista */
void inserirInicio(int id, char origem[], char destino[]);
void inserirFinal(int id, char origem[], char destino[]);
void buscarPacote(int id);
void atualizar(int idPacote, int novoStatus);
void remover(int id);
void exibirLista();

/* Fila */
void enfileirarLinear(Pacote pacote);
Pacote desenfileirarLinear();
void exibirFilaLinear();
int filaLinearVazia();
int filaLinearCheia();

/* Pilha */
void empilhar(Pacote p);
Pacote desempilhar();
void mostrar_pilha();
int esta_vazia();
int esta_cheia();

#endif