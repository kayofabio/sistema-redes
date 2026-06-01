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
8. Não havia cadastro do ambiente de rede, resolução de destino nem visualização do deslocamento do pacote.

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

### Aprofundar a rede sem criar um projeto grande

Foi adicionada uma camada didática de rede com cadastro de dispositivos, resolução por DNS, resolução IP para MAC por ARP e animação ASCII da PDU.

Motivo técnico:

- A simulação passa a mostrar o que acontece antes do pacote sair da fila.
- O usuário consegue ver por que um roteador aparece quando origem e destino estão em sub-redes diferentes.
- A interface continua sendo um programa C de terminal, sem dependência externa obrigatória.

Motivo não técnico:

- A apresentação fica mais próxima de uma ferramenta de laboratório: o usuário cadastra o ambiente, escolhe uma ação e vê uma tela de resultado.

### Conectar a simulação com situações do cotidiano

Foi adicionada uma documentação específica para mostrar onde esse fluxo aparece fora do exercício.

Motivo técnico:

- DNS, ARP, roteador, fila e retransmissão ficam mais fáceis de defender quando ligados a ações reais.
- A explicação deixa de ser apenas "rede de computadores" e passa a mostrar casos observáveis.

Motivo não técnico:

- Pessoas que não trabalham com infraestrutura conseguem relacionar a simulação com abrir um site, usar Wi-Fi, imprimir em rede, fazer pagamento online ou participar de chamada de vídeo.

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

### Problema 5 - O fluxo não explicava DNS, ARP e roteador

O projeto anterior só mostrava fila, pilha e lista. Isso atendia a estrutura de dados, mas deixava fraca a explicação de rede.

Correção adotada:

- Foi criado `ambiente.c` para cadastrar PC, switch, roteador, DNS e servidor.
- O destino pode ser informado por nome, IP ou domínio.
- Quando o destino é um domínio, o programa mostra uma consulta DNS simulada.
- Quando o pacote vai sair da origem, o programa mostra a resolução ARP do próximo salto.
- Quando origem e destino estão em sub-redes diferentes, o programa usa o roteador cadastrado.

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

O ambiente de rede usa uma tabela simples em memória. Essa tabela não substitui DNS, ARP ou roteamento real; ela representa esses conceitos para que o aluno consiga explicar a sequência de decisão.

## Evidências

- Cenário guiado: `docs/assets/execucao-cenario-questao-5.txt`
- Imagem do cenário: `docs/assets/cenario-questao-5.png`
- Animação de PDU: `docs/assets/execucao-animacao-pdu.txt`
- Imagem da animação: `docs/assets/animacao-pdu.png`
- Pilha LIFO: `docs/assets/execucao-pilha-lifo.txt`
- Imagem da pilha: `docs/assets/pilha-lifo.png`
- Aplicações no cotidiano: `docs/aplicacoes-dia-a-dia.md`

## Próximas melhorias possíveis

- Adicionar Makefile.
- Remover `main.exe` do versionamento em uma limpeza separada.
- Criar uma variação opcional com PDCurses/ncurses.
- Adicionar testes automatizados simples para as estruturas.
