# Relatório de implementação

## Contexto

O repositório já tinha uma base em C com fila, pilha e lista encadeada. A implementação, porém, ainda estava mais próxima de um protótipo de estruturas isoladas do que de uma simulação completa da Questão 5.

O trabalho foi direcionado para transformar o código em uma micro simulação de rede executável no terminal, com um fluxo que possa ser apresentado e explicado em sala.

## O que estava fraco no projeto

1. O pacote não guardava tempo estimado de transmissão.
2. A origem e o destino eram fixos e não apareciam como parte do pacote principal.
3. O erro era decidido apenas pelo tamanho maior que 500 KB, regra que não está no enunciado.
4. A busca e a remoção na lista existiam parcialmente, mas não estavam acessíveis pelo menu.
5. O pacote entregue continuava na lista, embora a atividade pedisse remoção de pacote entregue.
6. Não havia README, roteiro de teste ou evidência de execução.
7. Não havia uma opção que reproduzisse a situação prática da Questão 5 do começo ao fim.

## Decisões tomadas

### Manter terminal em vez de biblioteca gráfica

Foi avaliado o uso de bibliotecas do tipo PDCurses/ncurses para criar uma interface mais visual. A decisão foi manter uma interface textual simples no primeiro PR.

Motivo técnico:

- A atividade avalia estruturas de dados.
- Uma biblioteca externa aumentaria o risco de erro de instalação.
- O terminal funciona melhor em máquinas diferentes da turma.

Motivo não técnico:

- Qualquer pessoa consegue rodar e acompanhar a simulação sem preparar ambiente gráfico.

### Criar uma micro máquina de pacotes

A solução passou a tratar o programa como uma micro máquina com três áreas:

- fila de transmissão;
- pilha de erros;
- lista de pacotes ativos.

Essa divisão deixa claro onde cada estrutura de dados aparece e evita explicar o projeto apenas por arquivos de código.

### Criar um cenário guiado

A opção `10` do menu executa a Questão 5 automaticamente.

Isso reduz erro de apresentação, porque o avaliador consegue ver:

- chegada dos quatro pacotes;
- transmissão dos dois primeiros;
- pacote entregue removido da lista;
- pacote com erro enviado à pilha;
- resposta direta sobre o primeiro pacote transmitido.

## Problemas encontrados e correções

### Problema 1 - Ambiente sem compilador C no PATH

Durante o raio-x, a máquina local não tinha `gcc`, `clang` nem `cl` disponíveis no PATH. O WSL estava instalado, mas sem compilador C e sem `sudo` não interativo.

Correção adotada:

- Foi usado TinyCC portátil apenas para validação local.
- O README documenta GCC/MinGW como caminho principal de compilação.

### Problema 2 - Pacote com erro podia ficar duplicado no fluxo

Ao registrar erro em um pacote que ainda estava na fila, ele poderia aparecer ao mesmo tempo na fila e na pilha.

Correção adotada:

- Foi criada a função `removerPacoteDaFila`.
- Ao registrar erro, o pacote sai da fila, entra na pilha e permanece na lista com status `erro`.

### Problema 3 - Status numérico dificultava apresentação

O código anterior mostrava status como números. Isso exigia decorar uma legenda durante a apresentação.

Correção adotada:

- Foi criado `StatusPacote` com nomes de estado.
- A saída passou a mostrar `aguardando`, `em_transito`, `entregue` e `erro`.

### Problema 4 - Lista não demonstrava remoção de entregue

A atividade pedia remover pacote entregue. O código apenas atualizava status.

Correção adotada:

- Foi criada a função `removerPacoteEntregue`.
- O cenário guiado entrega o Pacote 1 e remove esse pacote da lista ativa.

## Explicação para pessoas não técnicas

O programa pode ser entendido como uma fila de atendimento de pacotes:

- A fila é a espera: quem chegou primeiro é atendido primeiro.
- A pilha é a mesa de problemas: o problema mais recente fica em cima e é resolvido primeiro.
- A lista é o controle geral: mostra quais pacotes ainda estão sendo acompanhados pela rede.

O objetivo não é provar velocidade real de internet. O objetivo é mostrar, com uma simulação pequena, como cada estrutura organiza os dados.

## Explicação técnica

O `Pacote` passou a concentrar os dados do enunciado:

- identificador interno;
- número do pacote;
- tamanho;
- tempo estimado;
- origem;
- destino;
- status.

A fila usa vetor fixo com deslocamento após remoção. Para o tamanho do exercício, essa escolha é suficiente e facilita visualizar FIFO.

A pilha usa vetor fixo com índice `topo`. O último erro registrado é o primeiro removido.

A lista encadeada usa alocação dinâmica para manter os pacotes ativos. Ela permite busca por número, atualização de status e remoção de nó entregue.

## Evidências

- Cenário guiado: `docs/assets/execucao-cenario-questao-5.txt`
- Imagem do cenário: `docs/assets/cenario-questao-5.png`
- Pilha LIFO: `docs/assets/execucao-pilha-lifo.txt`
- Imagem da pilha: `docs/assets/pilha-lifo.png`

## Próximas melhorias possíveis

- Adicionar Makefile.
- Remover `main.exe` do versionamento em uma limpeza separada.
- Criar uma variação opcional com PDCurses/ncurses.
- Adicionar testes automatizados simples para as estruturas.
