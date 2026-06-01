//FILA***********************************************
#include <stdio.h>
#include <string.h>
#include "rede.h"
 
#define MAX_SIZE 5 
 
Pacote fila[MAX_SIZE];  
int frente = 0;
int tras = -1; 
int pacotesTotal = 0;
 
// Verifica se a fila está vazia
int filaLinearVazia() {
    return frente > tras;
}
 
// Verifica se a fila está cheia (limite do vetor)
int filaLinearCheia() {
    return tras == MAX_SIZE - 1;
}
 
// Enfileirar (enqueue)
void enfileirarLinear(Pacote pacote) {
    if (filaLinearCheia()) {
        printf("Erro: Fila linear cheia! (Limite do vetor atingido)\n");
        return;
    }
    tras++; 
    fila[tras] = pacote;
    printf("Enfileirado: Pacote %d -> %d KB\n", pacote.numeroPacote, pacote.tamanhoKB);
    pacotesTotal++;
    if (pacotesTotal == 1) {
        inserirInicio(pacote.id, "Computador", "Servidor");
    } else {
        inserirFinal(pacote.id, "Computador", "Servidor");
    }
}
 
// Desenfileirar (dequeue)
Pacote desenfileirarLinear() {
    int statusPacote;
    if (filaLinearVazia()) {
        printf("Erro: Fila vazia!\n");
        Pacote vazio = {-1};
        return vazio;
    }
    Pacote pacote = fila[frente]; 
    frente++; // Simplesmente avança a frente
    if (pacote.tamanhoKB > 500) {
        empilhar(pacote); // Envia para a pilha (erros) se for maior que 500 KB
        statusPacote = 3; // Atualiza status para cancelado
        atualizar(pacote.id, statusPacote); // Atualiza o status na lista encadeada
    } else {
        statusPacote = 2; // Atualiza status para entregue
        atualizar(pacote.id, statusPacote); // Atualiza o status na lista encadeada
    }
    return pacote;
}
 
// Exibir a fila
void exibirFilaLinear() {
    if (filaLinearVazia()) {
        printf("Fila vazia!\n");
        return;
    }
    printf("Fila:\n");
    for (int i = frente; i <= tras; i++) {
        printf("ID: %d, Pacote %d -> %d KB\n", fila[i].id, fila[i].numeroPacote, fila[i].tamanhoKB);
    }
}