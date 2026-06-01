# Simulação de Redes de Computadores – Gerenciamento de Pacotes

Sistema desenvolvido em C que simula o funcionamento de uma rede de computadores baseada em comutação de pacotes, criado como atividade prática da disciplina de Estrutura de Dados.

O projeto utiliza diferentes estruturas de dados implementadas manualmente, aplicando cada uma em um cenário adequado ao fluxo de transmissão de pacotes em uma rede. Além da implementação das estruturas, o projeto também inclui modularização, ponteiros e alocação dinâmica de memória.

---

## Funcionalidades

### Gerenciamento de pacotes em transmissão

* Cadastro de pacotes contendo:
  * ID
  * Número do pacote
  * Tamanho (KB)
  * Tempo estimado

* Enfileiramento de pacotes para transmissão

* Transmissão de pacotes respeitando a ordem de chegada

* Registro de pacotes com falha na transmissão

* Retransmissão de pacotes com erro

* Gerenciamento de pacotes ativos na rede

* Busca de pacotes por ID

* Atualização do status dos pacotes

* Remoção de pacotes entregues

* Cadastro de pacotes ativos no sistema:
  * ID
  * Origem
  * Destino
* Listagem de pacotes ativos

---

## Estruturas utilizadas

| Estrutura       | Aplicação                       |
| --------------- | ------------------------------- |
| Fila Linear     | Pacotes aguardando transmissão  |
| Pilha           | Pacotes com erro de transmissão |
| Lista Encadeada | Pacotes ativos na rede          |

### Regras do sistema

**Fila de Transmissão**

* Os pacotes são transmitidos na ordem em que chegam à rede.
* Utiliza a política FIFO (First In, First Out).

**Pilha de Erros**

* Pacotes que falham durante a transmissão são armazenados para retransmissão.
* Utiliza a política LIFO (Last In, First Out).

**Lista de Pacotes Ativos**

* Armazena os pacotes que estão circulando pela rede.
* Permite inserção, busca, atualização de status e remoção de pacotes entregues.

---

## Estrutura do Projeto

```text
rede/
├── rede.h      # Structs, defines e protótipos das funções
├── main.c      # Menu principal e controle do sistema
├── fila.c      # Fila linear de pacotes aguardando transmissão
├── pilha.c     # Pilha de pacotes com erro
└── lista-encadeada.c     # Lista encadeada de pacotes ativos na rede
```

---

## Compilação

```bash
gcc main.c fila.c pilha.c lista-encadeada.c -o main
```

ou

```bash
gcc *.c -o main
```

---

## Execução

Windows:

```bash
main.exe
```

ou

```bash
start main.exe
```

Linux:

```bash
./main
```

---