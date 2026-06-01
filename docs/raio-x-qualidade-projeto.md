# Raio-x de qualidade do projeto

Data do raio-x: 2026-06-01.

Escopo analisado: implementação local da Questão 5 em `sistema-redes`, com fila, pilha, lista encadeada, cadastro de ambiente, simulação DNS/ARP, animação textual de PDU e documentação de apresentação.

## 1. Estado atual

O projeto já deixou de ser apenas um conjunto de estruturas isoladas. A versão local agora funciona como uma micro máquina de pacotes:

- a demo da Questão 5 roda de primeira, sem cadastro manual;
- o menu principal tem quatro rotas principais, em vez de uma lista longa de ações;
- o código foi dividido por responsabilidade;
- a documentação tem PRD, workflow, relatório, roteiro de testes, aplicações no dia a dia e este raio-x;
- a execução foi avaliada pelo critério de baixo privilégio;
- as evidências visuais foram reduzidas para recortes menores e legíveis.

Não foi feito envio de PR nesta etapa. O objetivo aqui foi fechar qualidade local antes de publicar.

## 2. Arquitetura atual

| Arquivo | Papel | Observação de qualidade |
| --- | --- | --- |
| `main.c` | Entrada do programa | Ficou apenas como bootstrap. |
| `menu.c` | Navegação principal e submenus | Removeu a poluição visual de doze opções na primeira tela. |
| `simulador.c` | Ações de uso: demo, cadastro, transmissão, erro, busca e remoção | Concentra as regras de interação com o usuário. |
| `ambiente.c` | Dispositivos, DNS didático, ARP didático e decisão de rota | É o módulo mais denso e deve ser o próximo alvo se houver nova refatoração. |
| `interface.c` | Tela, leitura, pausa e animação ASCII | A limpeza de tela deixou de usar comando de shell. |
| `pacote.c` | Montagem do pacote, status e tempo estimado | Mantém a regra de tempo em um único ponto. |
| `fila.c` | Fila FIFO | Usa limite explícito `MAX_FILA`. |
| `pilha.c` | Pilha LIFO de erros | Usa limite explícito `MAX_PILHA`. |
| `lista-encadeada.c` | Lista de pacotes ativos | Verifica `malloc` e libera nós ao encerrar. |
| `rede.h` | Contratos compartilhados | Centraliza structs, constantes e protótipos. |

## 3. Decisões técnicas

### Terminal em vez de interface gráfica

A decisão foi manter o projeto em terminal com `stdio.h`, tabelas textuais e animação ASCII. Bibliotecas como PDCurses/ncurses poderiam melhorar a tela, mas aumentariam o custo de instalação em laboratório.

O objetivo não é copiar o Cisco Packet Tracer. A referência é o comportamento didático: criar PDU, observar eventos, avançar a simulação e ver dados de origem/destino. Esse recorte aparece nas atividades da Cisco/NetAcad sobre PDU em modo de simulação.

### Demo pronta antes do cadastro

O avaliador precisa ver o enunciado funcionando antes de mexer em cadastro. Por isso a primeira opção do menu executa o cenário-base:

- Pacote 1: 500 KB;
- Pacote 2: 300 KB;
- Pacote 3: 700 KB;
- Pacote 4: 200 KB.

Depois disso, o cadastro manual fica disponível para demonstrar domínio, IP, MAC, DNS, roteador e erro.

### Vetores fixos onde o limite é parte da didática

Fila, pilha e ambiente usam limites explícitos:

- `MAX_FILA = 20`;
- `MAX_PILHA = 20`;
- `MAX_DISPOSITIVOS = 12`.

Isso não tenta simular uma rede real. A escolha foi manter comportamento previsível para apresentação e permitir teste claro de sobrecarga. A lista encadeada, que é exigida pelo enunciado, usa alocação dinâmica.

### DNS, ARP e roteador simulados em memória

O programa não abre socket e não consulta rede real. Ele usa uma tabela local de dispositivos para explicar:

- destino por domínio;
- resolução para IP;
- decisão de sub-rede `/24`;
- próximo salto via gateway;
- MAC do próximo salto.

Essa decisão evita risco de ambiente e mantém a entrega alinhada com Estrutura de Dados.

### Menu extraído para `menu.c`

O menu anterior crescia como uma lista única de opções. A extração para `menu.c` reduziu a responsabilidade do `main.c` e criou rotas:

- demo pronta;
- operações de pacote;
- ambiente de rede;
- consultas e estruturas.

Essa divisão melhora apresentação, leitura do código e manutenção.

### Limpeza de tela sem `system("cls")`

A interface deixou de chamar comando de shell para limpar tela. No Windows, usa a API de console com `GetStdHandle`, `FillConsoleOutputCharacter`, `FillConsoleOutputAttribute` e `SetConsoleCursorPosition`. Em sistemas POSIX, usa sequência ANSI.

Isso reduz dependência do shell e mantém a execução compatível com usuário comum.

### Entradas rejeitadas antes de alterar estado

Foram adicionadas validações para:

- número de pacote menor ou igual a zero;
- tamanho de pacote menor ou igual a zero;
- pacote duplicado na lista ativa;
- dispositivo duplicado por nome, IP ou domínio.

Esse ponto protege a consistência entre fila, pilha, lista e tabela de ambiente.

## 4. Medição de complexidade local

Medição simples feita em 2026-06-01 por arquivo, contando linhas e decisões básicas (`if`, `for`, `while`, `switch`, `case`):

| Arquivo | Linhas | Decisões simples |
| --- | ---: | ---: |
| `main.c` | 6 | 0 |
| `menu.c` | 212 | 29 |
| `pacote.c` | 47 | 6 |
| `ambiente.c` | 344 | 46 |
| `interface.c` | 201 | 18 |
| `simulador.c` | 301 | 25 |
| `fila.c` | 116 | 12 |
| `pilha.c` | 70 | 4 |
| `lista-encadeada.c` | 135 | 15 |
| `rede.h` | 107 | 0 |

Leitura da medição:

- `main.c` está correto como entrada mínima.
- `menu.c` tem muitas decisões porque roteia opções, mas as ações ficam fora dele.
- `ambiente.c` é o módulo mais sensível porque concentra validação de cadastro, busca, DNS didático, ARP didático e montagem de relatório.
- `resolverRotaPacote` já foi dividido em funções menores de resolução de destino e encaminhamento.
- Nenhuma função ficou acima de 43 linhas na medição local.

## 5. Testes executados

| Teste | Objetivo | Resultado |
| --- | --- | --- |
| Build com TinyCC | Confirmar que o projeto compila localmente no Windows | Passou com `tcc -Wall -Werror`. |
| Demo da Questão 5 | Validar fila, pilha, lista, DNS, ARP e PDU | Passou. Pacote 1 foi transmitido primeiro; Pacote 2 foi para a pilha. |
| Cadastro de ambiente | Confirmar inclusão de PC adicional | Passou com `PC-02`. |
| Sobrecarga de fila | Confirmar limite de `MAX_FILA = 20` | Passou. A tentativa seguinte foi recusada. |
| Destino inexistente | Confirmar erro controlado de rota | Passou. Pacote foi marcado como erro e empilhado. |
| Entrada inválida | Rejeitar pacote com número/tamanho inválido | Passou. Estado não foi alterado. |
| Pacote duplicado | Evitar ambiguidade na lista ativa | Passou. Segundo pacote com mesmo número foi recusado. |
| Dispositivo duplicado | Evitar ambiguidade na resolução de ambiente | Passou. Cadastro duplicado foi recusado. |
| IP e MAC inválidos | Rejeitar cadastro malformado | Passou. Cadastros inválidos foram recusados. |
| MAC duplicado | Evitar duas identidades físicas iguais | Passou. Cadastro duplicado foi recusado. |
| Entrada textual longa | Evitar contaminação do prompt seguinte | Passou. A entrada foi descartada e solicitada novamente. |
| Tamanho inteiro máximo | Evitar overflow no tempo estimado | Passou. `2147483647 KB` gera `21474836470 ms`. |
| Rota local | Não exibir roteador quando origem e destino estão na mesma sub-rede | Passou. A animação mostra origem, switch e destino. |
| Pilha cheia | Evitar perda de pacote em falha sem capacidade | Passou. O pacote permanece na fila e na lista. |
| Retransmissão inválida | Refazer resolução antes de alterar estado | Passou. Destino inexistente volta para a pilha. |
| `git diff --check` | Detectar whitespace problemático | Passou; apareceram apenas avisos de normalização LF/CRLF. |

Comando de build usado:

```powershell
.\scripts\validar-projeto.ps1 -Compiler .\build\tools\tcc-0.9.27\tcc\tcc.exe
```

Ferramentas encontradas no ambiente local:

| Ferramenta | Situação |
| --- | --- |
| TinyCC | Disponível localmente em `build/tools/tcc-0.9.27/tcc/tcc.exe`, versão 0.9.27. |
| GCC | Indisponível no PATH. |
| Clang | Indisponível no PATH. |
| MSVC `cl` | Indisponível no PATH. |
| Cppcheck | Indisponível no PATH. |
| Valgrind | Indisponível no PATH. |
| Make | Indisponível no PATH. |

## 6. Evidências visuais

As imagens abaixo são recortes legíveis gerados a partir de saídas locais do executável. Elas não usam dados sensíveis: os IPs são privados, os MACs são didáticos e os nomes são de laboratório.

| Evidência | Arquivo |
| --- | --- |
| Menu principal agrupado | `docs/assets/evidencia-menu-principal.png` |
| Cadastro de ambiente | `docs/assets/evidencia-cadastro-ambiente.png` |
| Sobrecarga da fila | `docs/assets/evidencia-sobrecarga-fila.png` |
| Destino inexistente tratado como erro | `docs/assets/evidencia-erro-destino.png` |
| Estado final da demo da Questão 5 | `docs/assets/evidencia-demo-estado-final.png` |
| Demo pausada para apresentação | `docs/assets/evidencia-demo-pausada.png` |
| Validação de entrada | `docs/assets/evidencia-validacao-entrada.png` |
| Ferramentas de validação | `docs/assets/evidencia-ferramentas-validacao.png` |
| Baixo privilégio | `docs/assets/evidencia-baixo-privilegio.png` |

Os transcripts correspondentes ficam na mesma pasta com extensão `.txt`.

## 7. Confiabilidade do código

Pontos já cobertos:

- cópias de texto usam limite e terminador nulo;
- relatórios usam `snprintf` e `vsnprintf`;
- leitura de texto usa `fgets`;
- leitura numérica limpa o restante do buffer;
- `malloc` é verificado antes de inserir na lista;
- a lista é liberada com `limparLista`;
- limites de fila, pilha e ambiente são checados antes de inserir;
- pacote com erro só sai da fila depois que a pilha aceita o registro;
- pacote entregue sai da lista ativa;
- entradas inválidas são recusadas antes de alterar estado.
- retransmissão refaz a resolução de rota antes de atualizar o status;
- rota local e rota via roteador possuem relatórios e animações coerentes;
- o cálculo de tempo usa `long long` para não estourar com entrada inteira válida;
- `scripts/validar-projeto.ps1` reproduz a bateria curta de regressão.

Pontos que ainda precisam de validação externa:

- compilar também com GCC/MinGW e Clang;
- rodar análise estática com Cppcheck;
- rodar verificação dinâmica com Valgrind/Memcheck em ambiente Linux;
- validar o script em outro Windows com compilador disponível no PATH.

## 8. Portabilidade e baixo privilégio

O simulador roda como aplicação local de terminal. Ele não exige administrador para executar porque não usa recurso protegido do sistema operacional.

Varredura feita em código C/H:

```powershell
rg -n "\b(fopen|remove|rename|system|CreateFile|RegOpen|socket|connect|bind|listen|accept|send|recv|ShellExecute)\s*\(" -g "*.c" -g "*.h"
```

Resultado: sem ocorrências.

Leitura prática:

- executar o programa exige apenas um binário compatível e terminal comum;
- compilar exige um compilador C e permissão de escrita na pasta de saída;
- o programa pode rodar offline;
- não há dependência de rede real, arquivo, banco de dados, serviço, registro do Windows ou privilégio administrativo;
- políticas externas da instituição ainda podem bloquear execução de binários baixados, mas isso não vem do código.

## 9. Lacunas restantes

1. Reconciliar a branch com a `origin/main`, que avançou quatro commits e possui conflitos nos módulos centrais.
2. Alinhar com o grupo a regra adicional do PDF novo da `origin/main`: enviar pacotes de `500 KB` ou mais para erro.
3. Rodar Cppcheck quando a ferramenta estiver disponível.
4. Rodar Valgrind/Memcheck quando houver ambiente Linux disponível.
5. Validar com GCC/MinGW e Clang em ambientes compatíveis.
6. Criar modo `--demo` ou `--test` se o grupo quiser capturas sem prompts interativos.

## 10. Pesquisa usada como base

Referências consultadas para orientar decisões, não para copiar texto:

- Cisco/NetAcad, atividade "Explore Network Functionality Using PDUs": https://contenthub.netacad.com/legacy/I2PT/1.1/en/course/files/3.1.1.3%20Packet%20Tracer%20-%20Explore%20Network%20Functionality%20Using%20PDUs.pdf
- Cisco/NetAcad, atividade "Learn to Use Packet Tracer": https://contenthub.netacad.com/legacy/NetEss/1.0/en/course/files/3.5.2.4%20Packet%20Tracer%20-%20Learn%20to%20Use%20Packet%20Tracer.pdf
- SEI CERT C Coding Standard: https://wiki.sei.cmu.edu/confluence/display/c
- GCC Warning Options: https://gcc.gnu.org/onlinedocs/gcc-15.1.0/gcc/Warning-Options.html
- Cppcheck Manual: https://cppcheck.sourceforge.io/manual.html
- Valgrind Memcheck Manual: https://valgrind.org/docs/manual/mc-manual.html
- Diátaxis documentation framework: https://diataxis.fr/

## 11. Conclusão do raio-x

O projeto está apresentável e funcional para a Questão 5, com build e regressão local reproduzíveis. O usuário abre o terminal, roda a demo, entende fila, pilha e lista e consegue enxergar DNS, ARP e roteador em uma micro rede.

Ainda não deve ser publicado como PR: primeiro é necessário reconciliar a branch com a `origin/main`, alinhar a regra adicional presente no PDF novo do grupo e repetir a validação após essa integração.
