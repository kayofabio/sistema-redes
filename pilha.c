#include <stdio.h>
#include <string.h>
#include "rede.h"
#define MAX 100

Pacote pilha[MAX]; // vetor que representa a pilha
int topo = -1; // controla o topo da pilha

// Função para empilhar (push)
void empilhar(Pacote p) {
    if (topo == MAX - 1) {
        printf("Erro: Pilha cheia!\n");
        return;
    }
    topo++;
    pilha[topo] = p;
    printf("Empilhado: Pacote %d -> %d KB\n", p.numeroPacote, p.tamanhoKB);
}

// Função para desempilhar (pop)
Pacote desempilhar() {
    if (topo == -1) {
        printf("Erro: Pilha vazia!\n");
        Pacote vazio = {-1};
        return vazio;   // valor de erro
    }
    Pacote valor = pilha[topo];
    int statusPacote = 2; 
    atualizar(valor.id, statusPacote); 
    topo--;
    return valor;
}

// Função para consultar o topo (peek)
Pacote consultar_topo() {
    if (topo == -1) {
        printf("Erro: Pilha vazia!\n");
        Pacote vazio = {-1};
        return vazio;
    }
    return pilha[topo];
}

// Verifica se a pilha está vazia
int esta_vazia() {
    return topo == -1;
}

// Verifica se a pilha está cheia
int esta_cheia() {
    return topo == MAX - 1;
}

// Mostrar toda a pilha (do topo até a base)
void mostrar_pilha() {
    if (esta_vazia()) {
        printf("Pilha vazia!\n");
        return;
    }

    printf("Pilha (do topo para a base):\n");
    for (int i = topo; i >= 0; i--) {
        printf("ID: %d, Pacote %d -> %d KB\n", pilha[i].id, pilha[i].numeroPacote, pilha[i].tamanhoKB);
    }
}