# PRD - Simulacao de redes com comutacao de pacotes

## 1. Objetivo

Complementar o repositório `sistema-redes` para atender à Questão 5 da atividade de Estrutura de Dados: uma simulação local, em C, de uma rede simples com comutação de pacotes.

O sistema deve demonstrar o uso de três estruturas:

- Fila: pacotes aguardando transmissão, em ordem de chegada.
- Pilha: pacotes com erro aguardando retransmissão.
- Lista encadeada: pacotes ativos na rede, com origem, destino e status.

A entrega precisa ser executável em terminal, documentada para apresentação acadêmica e suficientemente explícita para ser defendida por pessoas técnicas e não técnicas.

## 2. Fonte de escopo

Fonte principal: enunciado da Questão 5 da atividade da disciplina.

Resumo do enunciado:

- Cada pacote possui número do pacote, tamanho em KB e tempo estimado de transmissão.
- A fila deve permitir adicionar pacote, transmitir o próximo pacote e mostrar a fila de espera.
- A pilha deve permitir registrar pacote com erro, retransmitir o último pacote com erro e mostrar a pilha.
- A lista encadeada deve permitir inserir pacote, buscar pacote, remover pacote entregue e listar pacotes.
- A simulação prática deve cobrir:
  - chegada de quatro pacotes;
  - transmissão de dois pacotes;
  - um pacote com erro enviado para a pilha;
  - um pacote entregue removido da lista.

Pacotes obrigatórios do cenário-base:

| Pacote | Tamanho |
| --- | ---: |
| 1 | 500 KB |
| 2 | 300 KB |
| 3 | 700 KB |
| 4 | 200 KB |

Perguntas teóricas que a documentação deve responder:

- Por que a fila representa bem a transmissão de pacotes?
- Por que a pilha pode representar retransmissão?
- Por que a lista encadeada ajuda no controle de pacotes ativos?
- Qual estrutura representa melhor atraso de fila?

A `origin/main` recebeu posteriormente o arquivo `simulacao-e-respostas.pdf`. Esse documento contém uma regra adicional criada para teste: pacotes com `500 KB` ou mais vão para a pilha de erros. A integração manteve essa regra somente no cenário guiado. As operações manuais continuam com erro explícito para permitir retransmissão sem ciclo automático.

## 3. Referencia conceitual: Packet Tracer

O objetivo não é copiar o Cisco Packet Tracer nem criar uma ferramenta gráfica completa. A referência é conceitual: o programa deve ajudar o aluno a observar o caminho do pacote e a mudança de estado em cada etapa.

Elementos que serão adaptados para terminal:

- Lista de eventos: cada ação da simulação deve registrar o que ocorreu, por exemplo chegada, transmissão, falha, retransmissão e entrega.
- Avanço passo a passo: a simulação deve permitir executar um cenário guiado sem exigir que o avaliador adivinhe a sequência de comandos.
- Visão do estado da rede: o terminal deve mostrar fila, pilha e lista ativa de forma organizada.
- Animação textual: a PDU deve atravessar a topologia em quadros simples de terminal.
- Explicação didática: a documentação deve ligar cada comportamento observado à estrutura de dados correspondente.

Limite assumido:

- Não haverá simulação real de protocolos TCP/IP, roteamento, portas, sockets ou tráfego de rede.
- A rede será uma "micro máquina" didática, com estados controlados em memória.

## 4. Micro máquina proposta

Nome de trabalho: Micro Máquina de Pacotes.

Topologia didática:

```text
PC-01 -> SWITCH-01 -> ROTEADOR-01 -> SERVIDOR-01
                  |
                DNS-01
```

Estados principais:

| Estado | Estrutura | Papel |
| --- | --- | --- |
| Aguardando transmissão | Fila | Guarda a ordem de chegada dos pacotes. |
| Falha de transmissão | Pilha | Guarda o último pacote com erro no topo para retransmissão. |
| Ativo na rede | Lista encadeada | Permite buscar, listar e remover pacotes em trânsito ou entregues. |
| Registro da simulação | Saída textual | Mostra a linha do tempo da execução. |
| Ambiente de rede | Tabela de dispositivos | Permite cadastrar PC, switch, roteador, servidor e DNS. |
| Resolução de destino | Busca em tabela | Simula DNS, ARP e decisão de gateway. |

Campos mínimos de `Pacote`:

- `id`: identificador interno da simulação.
- `numero`: número apresentado no enunciado.
- `tamanhoKB`: tamanho do pacote.
- `tempoEstimadoMs`: tempo estimado de transmissão.
- `origem`: origem textual, por exemplo `PC-01`.
- `destino`: destino textual, por exemplo `SERVIDOR-01`.
- `status`: `aguardando`, `em_transito`, `entregue` ou `erro`.

## 5. Raio-x da base recebida

Repositório analisado: `kayofabio/sistema-redes`.

Arquivos atuais:

- `main.c`: menu principal.
- `rede.h`: structs e protótipos.
- `fila.c`: fila linear de pacotes.
- `pilha.c`: pilha de pacotes com erro.
- `lista-encadeada.c`: lista de pacotes ativos.
- `main.exe`: executável antigo que estava versionado no repositório.

Estado observado em 2026-06-01:

- O repositório está público, sem README e sem licença declarada.
- Não há issues ou pull requests abertos.
- O código já usa fila, pilha e lista encadeada, mas ainda não cobre todo o enunciado.
- A máquina local usada no raio-x não possui `gcc`, `clang` ou `cl` no PATH; por isso a primeira validação executou o `main.exe` existente e a validação da implementação passou a usar TinyCC portátil.

## 6. Lacunas encontradas

| Item do enunciado | Situação atual | Ajuste necessário |
| --- | --- | --- |
| Tempo estimado de transmissão | Não existe no `Pacote`. | Adicionar campo e exibir no fluxo. |
| Registrar pacote com erro | O erro é automático para pacotes maiores que 500 KB. | Permitir registro explícito de erro, sem depender apenas do tamanho. |
| Retransmitir último erro | Existe via pilha. | Ajustar mensagens e status para mostrar LIFO claramente. |
| Buscar pacote ativo | Função existe, mas não aparece no menu. | Expor operação no menu. |
| Remover pacote entregue | Função existe, mas não aparece no menu e entregues continuam listados. | Expor remoção e usar no cenário guiado. |
| Status textual | Status é número inteiro. | Mostrar status legível para apresentação. |
| Origem e destino | Fixos como `Computador` e `Servidor`. | Usar nomes de nós da micro máquina ou permitir entrada. |
| Cenário-base do PDF | Pode ser feito manualmente, mas não existe atalho guiado. | Criar opção "executar cenário da questão 5". |
| Documentação | Não há README no `sistema-redes`. | Criar README com objetivo, build, execução, screenshots e respostas teóricas. |
| Evidências | Não há capturas ou roteiro de teste. | Criar pasta de evidências com saídas e imagens da execução. |
| Tela de terminal | Menu reaparece sem limpar a tela. | Usar telas limpas por ação, como um app de terminal. |
| Visualização do pacote | Não há animação. | Criar animação ASCII da PDU passando pela topologia. |
| Rede conceitual | Não há DNS, ARP ou roteador no fluxo. | Simular resolução DNS, ARP e encaminhamento por gateway. |

## 7. Decisão sobre framework de terminal

Foram avaliadas bibliotecas do tipo curses, especialmente PDCurses/ncurses, porque elas permitem criar janelas, painéis e telas de terminal em C.

Decisão para este PR:

- Não adicionar dependência externa obrigatória no primeiro ciclo.
- Implementar uma interface textual organizada com `stdio.h`, tabelas, API de console no Windows e ANSI simples em sistemas POSIX.
- Documentar PDCurses/ncurses como melhoria futura, não como requisito da atividade.

Justificativa:

- O ganho principal da atividade está na clareza das estruturas de dados, não na renderização do terminal.
- Dependências como PDCurses melhoram interface, mas aumentam o custo de instalação em máquinas de laboratório.
- Para equidade, o avaliador deve conseguir compilar e rodar com um compilador C comum, sem preparar biblioteca gráfica.

## 8. Requisitos funcionais

RF01 - Adicionar pacote à fila.

- Entrada: número, tamanho em KB, tempo estimado, origem e destino.
- Saída: pacote entra na fila e também na lista de ativos.

RF02 - Transmitir próximo pacote.

- Deve remover o primeiro pacote da fila.
- Deve mostrar que a fila usa FIFO.
- Deve atualizar o status do pacote.

RF03 - Registrar erro de transmissão.

- Deve enviar um pacote para a pilha de erros.
- Deve atualizar o status para `erro`.
- Deve permitir demonstrar o exemplo "Pacote 2 falhou" e "Pacote 4 falhou".

RF04 - Retransmitir último pacote com erro.

- Deve remover do topo da pilha.
- Deve mostrar que a pilha usa LIFO.
- Se os pacotes 2 e 4 falharem nessa ordem, o pacote 4 deve ser retransmitido primeiro.

RF05 - Listar fila, pilha e lista ativa.

- A fila deve mostrar pacotes aguardando transmissão.
- A pilha deve mostrar erros do topo para a base.
- A lista deve mostrar id, origem, destino e status textual.

RF06 - Buscar pacote ativo.

- Entrada: id ou número do pacote.
- Saída: detalhes do pacote encontrado ou mensagem objetiva de ausência.

RF07 - Remover pacote entregue.

- Deve remover da lista encadeada um pacote com status `entregue`.
- Deve impedir remoção silenciosa de pacote inexistente.

RF08 - Executar cenário guiado da Questão 5.

- Deve inserir os quatro pacotes do enunciado.
- Deve transmitir dois pacotes.
- Deve aplicar a regra de teste do grupo: pacote com `500 KB` ou mais simula falha na primeira transmissão.
- Deve mover o Pacote 1 para a pilha.
- Deve entregar e remover o Pacote 2 da lista ativa.
- Deve remover um pacote entregue da lista.
- Deve imprimir uma linha do tempo do que aconteceu.

RF09 - Responder às perguntas teóricas no README.

- As respostas devem usar o comportamento real da simulação como exemplo.

RF10 - Cadastrar ambiente de rede.

- Deve permitir cadastrar dispositivo com nome, tipo, IP, MAC e domínio opcional.
- Deve iniciar com ambiente padrão para facilitar demonstração.

RF11 - Resolver destino antes da transmissão.

- Deve aceitar destino por nome, IP ou domínio.
- Deve mostrar consulta DNS quando o destino for domínio.
- Deve mostrar resolução ARP para o próximo salto.
- Deve mostrar uso do roteador quando origem e destino estiverem em sub-redes diferentes.

RF12 - Animar troca de pacote.

- Deve limpar a tela entre quadros.
- Deve mostrar a PDU passando pela topologia.
- Deve manter a simulação executável em terminal comum.

## 9. Requisitos não funcionais

RNF01 - Portabilidade.

- O projeto deve compilar com GCC ou Clang em Linux/macOS e MinGW no Windows.
- O README deve mostrar comandos equivalentes.

RNF02 - Clareza didática.

- A saída do terminal deve ser legível em apresentação, sem depender de fonte, tema ou terminal específico.

RNF03 - Baixo custo de ambiente.

- O primeiro PR não deve exigir banco de dados, rede real, container ou biblioteca de terminal externa.

RNF04 - Organização de código.

- Manter a separação por estrutura: fila, pilha, lista e menu.
- Evitar concentrar toda a lógica no `main.c`.
- Organizar a navegação em rotas principais e submenus para evitar poluição visual.

RNF05 - Evidência de execução.

- Incluir roteiro de teste manual.
- Incluir screenshots ou imagens de terminal no README.

RNF06 - Baixo privilégio.

- O simulador deve rodar como aplicação local de terminal.
- A execução não deve exigir administrador.
- A simulação não deve depender de rede real, arquivo, banco de dados, serviço ou configuração do sistema.

RNF07 - Vocabulário defendível.

- A documentação deve incluir glossário para termos de estrutura de dados, rede, C, build e validação.

RNF08 - Apresentação acompanhável.

- A demo guiada deve pausar nos pontos em que a tela contém explicação de rede ou mudança de estado.
- A pessoa apresentando deve conseguir ler DNS, ARP, rota e estado final antes de avançar.

## 10. Critérios de aceite

CA01 - Ao executar o cenário-base, o primeiro pacote retirado da fila deve ser o Pacote 1, pois ele foi o primeiro a entrar. A demo deve registrar sua falha conforme a regra de teste por tamanho.

CA02 - Se Pacote 2 e Pacote 4 forem registrados com erro nessa ordem, o primeiro retransmitido deve ser o Pacote 4, pois a pilha remove o último item inserido.

CA03 - A lista de ativos deve permitir localizar um pacote pelo identificador.

CA04 - Um pacote entregue deve poder ser removido da lista.

CA05 - O README deve conter:

- objetivo do projeto;
- relação com a Questão 5;
- comandos de compilação;
- comandos de execução;
- roteiro de demonstração;
- screenshots;
- respostas teóricas;
- decisões e limitações.

CA06 - O PR deve conter evidência de teste local.

## 11. Plano de implementação

1. Criar branch de trabalho.
2. Atualizar `rede.h` com os campos faltantes e enumeração de status.
3. Ajustar `fila.c` para transmitir e relatar FIFO com tempo estimado.
4. Ajustar `pilha.c` para erro e retransmissão com status textual.
5. Ajustar `lista-encadeada.c` para busca, remoção de entregues e exibição legível.
6. Reorganizar `main.c` como bootstrap e mover a navegação para `menu.c`.
7. Criar README do `sistema-redes`.
8. Criar roteiro de teste em `docs/roteiro-testes.md`.
9. Gerar evidências de execução no terminal.
10. Revisar fluxo completo antes do PR.

## 12. Testes planejados

Teste manual A - Fila:

- Adicionar pacotes 1, 2, 3 e 4.
- Listar fila.
- Transmitir duas vezes.
- Confirmar que Pacote 1 e Pacote 2 saíram primeiro.

Teste manual B - Pilha:

- Registrar erro no Pacote 2.
- Registrar erro no Pacote 4.
- Listar pilha.
- Retransmitir uma vez.
- Confirmar que Pacote 4 saiu primeiro.

Teste manual C - Lista:

- Inserir pacotes ativos.
- Buscar Pacote 3.
- Marcar Pacote 1 como entregue.
- Remover Pacote 1.
- Listar ativos e confirmar que ele não aparece mais.

Teste manual D - Cenário guiado:

- Rodar a opção `1 - Rodar demo pronta da Questao 5`.
- Conferir se a saída mostra chegada, transmissão, erro, pilha, entrega e remoção.

## 13. Evidências geradas

Arquivos criados:

- `docs/assets/evidencia-menu-principal.png`
- `docs/assets/evidencia-cadastro-ambiente.png`
- `docs/assets/evidencia-sobrecarga-fila.png`
- `docs/assets/evidencia-erro-destino.png`
- `docs/assets/evidencia-demo-estado-final.png`
- `docs/assets/evidencia-demo-pausada.png`
- `docs/assets/evidencia-validacao-entrada.png`
- `docs/assets/evidencia-ferramentas-validacao.png`
- `docs/assets/evidencia-baixo-privilegio.png`
- `docs/assets/execucao-cenario-questao-5.txt`
- `docs/assets/execucao-animacao-pdu.txt`
- `docs/assets/execucao-pilha-lifo.txt`
- `docs/raio-x-qualidade-projeto.md`
- `docs/portabilidade-baixo-privilegio.md`
- `docs/glossario-tecnico.md`
- `docs/guia-basico-execucao.md`
- `docs/auditoria-pre-pr.md`
- `scripts/build.ps1`
- `scripts/validar-projeto.ps1`
- `scripts/gerar-evidencias.ps1`

As imagens foram renderizadas a partir da saída textual local do executável, com separadores de tela normalizados para leitura. Os arquivos `.txt` preservam o transcript usado para gerar as evidências visuais. Os recortes `evidencia-*` foram adicionados para apresentação, porque imagens longas prejudicavam leitura.

## 14. Problemas já observados e decisões

Problema 1 - O projeto não possui README.

- Decisão: criar README focado em compilação, execução e explicação das estruturas.

Problema 2 - O executável `main.exe` está versionado.

- Decisão inicial: preservar até concluir o raio-x.
- Decisão após auditoria: remover do versionamento, pois o binário executava o menu antigo e não representava mais o código-fonte atual.

Problema 3 - O ambiente local não possui compilador C no PATH.

- Decisão: usar o executável atual apenas para raio-x inicial, validar a implementação local com TinyCC portátil e manter GCC/MinGW documentado como caminho principal de compilação.

Problema 4 - O código atual associa erro apenas a pacote maior que 500 KB.

- Decisão: permitir erro manual, porque o enunciado fala em pacotes que falharam, sem limitar falha a tamanho.

Problema 5 - A lista atual mantém pacotes entregues como status, mas o enunciado pede remover pacote entregue.

- Decisão: manter atualização de status e adicionar remoção explícita de entregues para demonstrar a operação da lista.

Problema 6 - O menu principal ficou poluído após a ampliação do escopo.

- Decisão: mover a navegação para `menu.c` e agrupar ações em rotas principais e submenus.

Problema 7 - Dados duplicados podiam criar ambiguidade na apresentação.

- Decisão: recusar pacote duplicado na lista ativa e dispositivo duplicado por nome, IP ou domínio.

Problema 8 - A documentação precisava registrar decisões e lacunas, não apenas instruções.

- Decisão: criar `docs/raio-x-qualidade-projeto.md` com arquitetura, testes, evidências, complexidade e próximos passos.

Problema 9 - A defesa precisava responder se o projeto roda em ambiente restrito.

- Decisão: criar `docs/portabilidade-baixo-privilegio.md` e evidência específica de varredura por chamadas com efeito externo.

Problema 10 - A documentação usava termos técnicos demais sem apoio de leitura.

- Decisão: criar `docs/glossario-tecnico.md` com termos de rede, estruturas, C, build, documentação e privilégio mínimo.

Problema 11 - A demo apagava informações rápido demais para apresentação.

- Decisão: pausar a resolução DNS/ARP antes da animação e pausar o cenário entre etapas.

Problema 12 - Uma pessoa não técnica precisava de um caminho operacional completo.

- Decisão: criar `docs/guia-basico-execucao.md` com comandos, explicação de cada comando, erros comuns, ferramentas e ordem de apresentação.

Problema 13 - A `origin/main` avançou durante a implementação.

- Decisão: não abrir PR antes de reconciliar os quatro commits novos, preservar `simulacao-e-respostas.pdf` e repetir a bateria de regressão.

Problema 14 - O PDF novo do grupo acrescenta uma regra que não está no enunciado original.

- Decisão: aplicar a regra por tamanho somente no cenário guiado. O modo manual preserva erro explícito e retransmissão funcional.

## 15. Fora de escopo

- Interface gráfica semelhante ao Cisco Packet Tracer.
- Simulação real de roteadores, switches ou protocolos.
- Uso obrigatório de PDCurses/ncurses.
- Pipeline de CI no primeiro PR.
- Testes de estresse ou desempenho sensível.

## 16. Referências consultadas

- Cisco Packet Tracer Data Sheet: https://www.cisco.com/c/dam/en_us/training-events/netacad/course_catalog/docs/Cisco_PacketTracer_DS.pdf
- Packet Tracer Help - Operating Modes: https://tutorials.ptnetacad.net/help/default/operatingModes.htm
- RFC 826 - Address Resolution Protocol: https://www.rfc-editor.org/rfc/rfc826
- RFC 1035 - Domain Names: https://www.rfc-editor.org/rfc/rfc1035
- PDCurses User Guide: https://pdcurses.org/docs/USERS.html
- PDCurses for Windows console: https://pdcurses.org/wincon/
