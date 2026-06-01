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

A opção `1` do menu executa a Questão 5 automaticamente, antes das opções de cadastro manual.

Isso reduz erro de apresentação, porque o avaliador consegue ver:

- chegada dos quatro pacotes;
- transmissão dos dois primeiros;
- pacote entregue removido da lista;
- pacote com erro enviado à pilha;
- resposta direta sobre o primeiro pacote transmitido.

Após revisão de apresentação, a demo passou a pausar entre etapas importantes.

Motivo técnico:

- A resolução DNS/ARP era apagada pela animação antes de a pessoa conseguir explicar.
- A pausa preserva a tela até o apresentador pressionar Enter.

Motivo não técnico:

- O avaliador consegue acompanhar a lógica sem depender de leitura rápida.

### Reduzir poluição visual do menu

O menu principal foi reorganizado em rotas principais:

- demo pronta;
- operação de pacotes;
- ambiente de rede;
- consultas e estruturas.

Motivo técnico:

- O `main.c` ficou responsável apenas pelo bootstrap.
- O roteamento foi isolado em `menu.c`.
- As ações continuam em `simulador.c`, preservando baixa complexidade por módulo.

Motivo não técnico:

- A primeira tela deixa claro o caminho principal sem expor doze opções ao mesmo tempo.

### Reduzir fragilidade da interface de terminal

A limpeza de tela deixou de usar `system("cls")`. No Windows, passou a usar a API de console; em sistemas POSIX, usa sequência ANSI.

Motivo técnico:

- Evita depender de comando externo do shell.
- Mantém o comportamento simples e local.
- Reduz uma prática desnecessária em código C de terminal.

Motivo não técnico:

- O programa continua parecendo um app de terminal, mas com menos risco de ambiente.

### Validar dados antes de alterar estado

Foram adicionadas validações para pacote inválido, pacote duplicado e dispositivo duplicado.

Motivo técnico:

- A lista encadeada usa o número do pacote para busca, atualização e remoção.
- Dois pacotes ativos com o mesmo número criariam ambiguidade.
- Dois dispositivos com mesmo nome, IP ou domínio prejudicariam a resolução de rota.

Motivo não técnico:

- Durante apresentação, erro de digitação deve virar mensagem clara, não estado quebrado.

### Confirmar execução com privilégio mínimo

Foi feita uma varredura no código para confirmar que o executável não usa arquivo, socket, registro do Windows, comando de shell ou API administrativa.

Motivo técnico:

- A simulação deve funcionar em máquina de laboratório sem privilégio elevado.
- O programa deve depender apenas de terminal, memória do processo e temporização local.

Motivo não técnico:

- O professor ou qualquer integrante do grupo deve conseguir executar a demonstração em um ambiente restrito, desde que exista binário compatível ou compilador C disponível.

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
- O README documenta GCC no Linux, MinGW/GCC no Windows e Clang no macOS.

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

### Problema 6 - A tabela de ambiente podia ficar ambígua

Sem validação, seria possível cadastrar dois dispositivos com o mesmo nome, IP ou domínio.

Correção adotada:

- O cadastro agora recusa duplicidade por nome, IP ou domínio.
- O relatório de rota fica mais confiável, porque cada entrada resolve para um único dispositivo.

### Problema 7 - Pacotes duplicados quebravam a explicação

Como busca, erro, retransmissão e remoção usam o número do pacote, duplicidade deixaria a apresentação confusa.

Correção adotada:

- Pacote com número já ativo é recusado.
- Número e tamanho devem ser maiores que zero.

### Problema 8 - Evidências grandes prejudicavam leitura

As primeiras imagens mostravam telas longas e difíceis de ler em apresentação.

Correção adotada:

- Foram criados recortes focados: menu, cadastro, sobrecarga, erro de destino, estado final e validação de entrada.
- Os transcripts continuam preservados em `docs/assets`.

### Problema 9 - Termos técnicos sem glossário

A documentação já citava DNS, ARP, PDU, FIFO, LIFO, compilador, Cppcheck e Valgrind, mas uma pessoa não técnica poderia se perder durante a leitura.

Correção adotada:

- Foi criado `docs/glossario-tecnico.md`.
- Os termos foram separados por estruturas de dados, rede, C, build, documentação e privilégio mínimo.

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
- Animação de PDU: `docs/assets/execucao-animacao-pdu.txt`
- Pilha LIFO: `docs/assets/execucao-pilha-lifo.txt`
- Menu principal: `docs/assets/evidencia-menu-principal.png`
- Cadastro de ambiente: `docs/assets/evidencia-cadastro-ambiente.png`
- Sobrecarga da fila: `docs/assets/evidencia-sobrecarga-fila.png`
- Erro de destino: `docs/assets/evidencia-erro-destino.png`
- Estado final da Questão 5: `docs/assets/evidencia-demo-estado-final.png`
- Validação de entrada: `docs/assets/evidencia-validacao-entrada.png`
- Demo pausada: `docs/assets/evidencia-demo-pausada.png`
- Ferramentas de validação: `docs/assets/evidencia-ferramentas-validacao.png`
- Baixo privilégio: `docs/assets/evidencia-baixo-privilegio.png`
- Aplicações no cotidiano: `docs/aplicacoes-dia-a-dia.md`
- Raio-x de qualidade: `docs/raio-x-qualidade-projeto.md`
- Portabilidade e baixo privilégio: `docs/portabilidade-baixo-privilegio.md`
- Glossário técnico: `docs/glossario-tecnico.md`
- Guia básico de execução: `docs/guia-basico-execucao.md`
- Auditoria pré-PR: `docs/auditoria-pre-pr.md`
- Gerador de evidências: `scripts/gerar-evidencias.ps1`

## Próximas melhorias possíveis

- Adicionar Makefile.
- Criar modo não interativo `--demo` ou `--test` para simplificar captura de evidências.
- Rodar Cppcheck em ambiente com a ferramenta instalada.
- Rodar Valgrind/Memcheck em Linux com compilador C.
- Criar uma variação opcional com PDCurses/ncurses.
- Reconciliar a branch com a `origin/main` antes do PR.

## Endurecimento antes do PR

Uma auditoria posterior encontrou e corrigiu falhas que não apareciam no cenário-base:

- overflow no cálculo de tempo para tamanho inteiro máximo;
- entrada textual longa contaminando o prompt seguinte;
- IP e MAC inválidos aceitos no cadastro;
- MAC duplicado aceito no ambiente;
- perda de consistência quando a pilha de erros estava cheia;
- animação de roteador exibida indevidamente em rota local;
- retransmissão que alterava estado sem refazer a resolução de rota;
- executável `main.exe` antigo e incompatível com o código-fonte atual.

Também foram adicionados `scripts/build.ps1`, `scripts/validar-projeto.ps1` e o parecer `docs/auditoria-pre-pr.md`.
