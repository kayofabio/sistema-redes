# Glossário técnico

Este glossário reúne os termos usados no código, na documentação e na apresentação do simulador. O objetivo é permitir que uma pessoa técnica ou não técnica acompanhe a defesa do projeto sem depender de conhecimento prévio.

## Estruturas de dados

| Termo | Significado no projeto |
| --- | --- |
| Estrutura de dados | Forma de organizar dados para resolver uma operação. O projeto usa fila, pilha e lista encadeada. |
| Fila | Estrutura em que o primeiro item a entrar é o primeiro a sair. Representa pacotes aguardando transmissão. |
| FIFO | Sigla para `First In, First Out`. No projeto, explica por que o Pacote 1 é transmitido antes dos demais. |
| Pilha | Estrutura em que o último item a entrar é o primeiro a sair. Representa pacotes com erro aguardando retransmissão. |
| LIFO | Sigla para `Last In, First Out`. No projeto, explica por que o erro mais recente fica no topo da pilha. |
| Lista encadeada | Estrutura formada por nós ligados por ponteiros. Representa pacotes ativos acompanhados pela simulação. |
| Nó | Unidade da lista encadeada. Guarda um pacote e o ponteiro para o próximo nó. |
| Ponteiro | Variável que guarda um endereço de memória. É usado para ligar os nós da lista encadeada. |
| Vetor | Área contínua com posições fixas. O projeto usa vetor para fila, pilha e tabela de ambiente. |
| Limite fixo | Quantidade máxima definida no código. Exemplos: `MAX_FILA`, `MAX_PILHA` e `MAX_DISPOSITIVOS`. |
| Inserção | Operação de adicionar um item em uma estrutura. |
| Remoção | Operação de retirar um item de uma estrutura. |
| Busca | Operação de procurar um item, como localizar um pacote pelo número. |
| Estado | Situação atual de um pacote: aguardando, em trânsito, entregue ou erro. |

## Rede

| Termo | Significado no projeto |
| --- | --- |
| Rede | Conjunto de dispositivos que trocam dados. No projeto, é uma simulação local em memória. |
| Pacote | Unidade didática de dados enviada pela rede simulada. Tem número, tamanho, tempo estimado, origem, destino e status. |
| PDU | Unidade de dados de protocolo. No simulador, é representada pela animação textual do pacote atravessando a topologia. |
| Origem | Dispositivo que envia o pacote. O padrão da demo é `PC-01`. |
| Destino | Dispositivo, IP ou domínio que recebe o pacote. O padrão da demo é `app.local`. |
| IP | Endereço lógico usado para identificar um dispositivo na rede. Exemplo: `192.168.10.10`. |
| MAC | Endereço físico da interface de rede. No projeto, é usado para explicar ARP. |
| Domínio | Nome amigável que representa um serviço. Exemplo: `app.local`. |
| DNS | Serviço que transforma domínio em IP. No projeto, `DNS-01` resolve `app.local`. |
| ARP | Processo usado em rede local para descobrir o MAC associado a um IP ou ao próximo salto. |
| Gateway | Equipamento usado para sair da rede local. No projeto, é representado por `ROTEADOR-01`. |
| Roteador | Dispositivo que encaminha pacotes entre redes diferentes. |
| Switch | Dispositivo que conecta equipamentos dentro de uma rede local. No projeto, aparece como parte da topologia didática. |
| Servidor | Dispositivo que fornece um serviço. No projeto, `SERVIDOR-01` representa o destino da aplicação. |
| Sub-rede | Parte lógica de uma rede IP. O projeto usa uma comparação didática `/24`. |
| `/24` | Máscara de rede em que os três primeiros octetos do IP identificam a rede. Exemplo: `192.168.10.x`. |
| Próximo salto | Próximo dispositivo para onde o pacote deve ser enviado. Pode ser o destino local ou o gateway. |
| Rota | Decisão sobre por onde o pacote deve seguir. |
| Topologia | Desenho lógico da rede. No projeto: `PC-01 -> SWITCH-01 -> ROTEADOR-01 -> SERVIDOR-01`. |
| Tráfego real | Dados de rede enviados de verdade. O projeto não faz tráfego real; tudo é simulado localmente. |
| Socket | Recurso usado para comunicação real de rede em programas. O projeto não usa socket. |
| Latência | Tempo de espera percebido na comunicação. No projeto, aparece de forma didática pelo tempo estimado. |
| Retransmissão | Nova tentativa de envio após erro. No projeto, é demonstrada pela pilha. |
| Timeout | Tempo máximo de espera antes de considerar uma operação falha. É citado como conceito, mas não implementado como rede real. |

## Código C

| Termo | Significado no projeto |
| --- | --- |
| C | Linguagem usada no projeto. Permite trabalhar com memória, structs, ponteiros e compilação nativa. |
| `struct` | Tipo composto que agrupa campos. O projeto usa `Pacote`, `Dispositivo` e `No`. |
| `enum` | Tipo com valores nomeados. O projeto usa enum para tipo de dispositivo e status do pacote. |
| Protótipo | Declaração de função colocada no `.h` para permitir uso entre arquivos `.c`. |
| Header | Arquivo `.h`. No projeto, `rede.h` centraliza contratos compartilhados. |
| Módulo | Arquivo com responsabilidade específica. Exemplo: `fila.c` cuida da fila. |
| `malloc` | Função que aloca memória dinamicamente. Usada na lista encadeada. |
| `free` | Função que libera memória alocada. Usada ao remover nós e limpar a lista. |
| `NULL` | Valor usado para representar ausência de ponteiro válido. |
| `printf` | Função que imprime texto no terminal. |
| `fgets` | Função que lê texto com limite de tamanho. Usada por todas as entradas interativas. |
| `strtol` | Função que converte texto para inteiro. Usada por `lerInteiro` com validação de erro e faixa. |
| `sscanf` | Função de leitura formatada. Usada somente para separar os quatro octetos durante a validação de IPv4. |
| `strncpy` | Função para copiar texto com limite. O projeto completa com terminador nulo. |
| `snprintf` | Função para formatar texto respeitando o tamanho do buffer. |
| `vsnprintf` | Variação de `snprintf` usada com argumentos variáveis. |
| Buffer | Área de memória usada para armazenar texto ou dados temporários. |
| Terminador nulo | Caractere `\0` que marca o fim de uma string em C. |
| `stdin` | Entrada padrão do processo. Normalmente é o teclado no terminal. |
| `stdout` | Saída padrão do processo. Normalmente é a tela do terminal. |
| `Sleep` | Função do Windows usada para pausa em milissegundos. |
| `usleep` | Função POSIX usada para pausa em microssegundos. |
| API de console do Windows | Funções locais do Windows usadas para limpar o terminal sem chamar comando de shell. |
| Sequência ANSI | Código textual que controla terminal, usado em sistemas POSIX para limpar tela sem comando de shell. |

## Build e ferramentas

| Termo | Significado no projeto |
| --- | --- |
| Build | Processo de transformar código-fonte em executável. |
| Compilador | Ferramenta que transforma C em binário. Exemplos: TinyCC, GCC, Clang, MinGW e MSVC. |
| TinyCC | Compilador C leve usado na validação local deste projeto. |
| GCC | Compilador C comum em Linux e MinGW. Está documentado como caminho principal de build. |
| Clang | Compilador C comum em macOS e Linux. |
| MinGW | Ambiente que permite gerar executáveis Windows com ferramentas GNU. |
| MSVC `cl` | Compilador C/C++ da Microsoft. |
| Make | Ferramenta de automação de build. Ainda não está configurada no projeto. |
| `Makefile` | Arquivo que define comandos de build para o `make`. É melhoria futura recomendada. |
| PATH | Lista de pastas onde o sistema procura comandos. Se `gcc` não está no PATH, o terminal não o encontra diretamente. |
| Binário | Arquivo executável gerado após compilação. |
| `.exe` | Extensão comum de executável no Windows. |
| `-Wall` | Opção de compilador que habilita vários avisos úteis. |
| Warning | Aviso de compilação. Indica possível problema, mesmo quando o build termina. |
| Cppcheck | Ferramenta de análise estática para C/C++. Ainda não foi rodada porque não está no PATH local. |
| Valgrind | Ferramenta de análise dinâmica usada para encontrar erros de memória em ambientes compatíveis. |
| Memcheck | Ferramenta do Valgrind focada em problemas de memória. |
| `git diff --check` | Comando que detecta problemas simples de whitespace antes de commit. |
| `scripts/build.ps1` | Script PowerShell que padroniza a compilação local. |
| `scripts/validar-projeto.ps1` | Script PowerShell que compila e executa a bateria curta de regressão. |
| LF/CRLF | Formatos de quebra de linha. LF é comum em Linux/macOS; CRLF é comum em Windows. |

## Documentação e entrega

| Termo | Significado no projeto |
| --- | --- |
| README | Documento inicial com objetivo, build, execução, evidências e explicação geral. |
| PRD | Documento de requisitos do produto. No projeto, detalha a Questão 5 e critérios de aceite. |
| Workflow | Fluxo de trabalho seguido para análise, implementação, teste e documentação. |
| Roteiro de testes | Lista de cenários que precisam ser executados para validar a entrega. |
| Raio-x | Auditoria técnica do projeto, com arquitetura, decisões, testes e lacunas. |
| Evidência | Arquivo que comprova execução ou validação. Pode ser transcript `.txt` ou imagem `.png`. |
| Transcript | Registro textual da saída do terminal. |
| Screenshot | Imagem usada para mostrar resultado visual. No projeto, as imagens são renderizadas a partir de transcripts. |
| Glossário | Lista de termos com explicação. Este documento é o glossário técnico da entrega. |
| Critério de aceite | Condição usada para dizer se o requisito foi cumprido. |
| Lacuna | Ponto ainda não resolvido ou que precisa de validação futura. |
| Pipeline | Automação de build/teste em CI. O projeto possui validação local automatizada, mas ainda não possui CI remoto. |
| PR | Pull request. Entrega enviada ao repositório para revisão. |

## Segurança e privilégio mínimo

| Termo | Significado no projeto |
| --- | --- |
| Privilégio mínimo | Princípio de usar apenas as permissões necessárias. O simulador roda sem administrador. |
| Administrador | Conta com permissões elevadas para alterar o sistema. O projeto não exige isso para executar. |
| Permissão de escrita | Direito de criar ou alterar arquivos em uma pasta. Só é necessária para gerar o executável durante build. |
| Instalação global | Instalação feita no sistema inteiro. O projeto não exige instalação global para rodar. |
| Compilador portátil | Compilador executado a partir de uma pasta do usuário, sem instalação administrativa. |
| Registro do Windows | Banco de configurações do Windows. O projeto não acessa o registro. |
| Serviço do sistema | Programa que roda em segundo plano gerenciado pelo sistema operacional. O projeto não cria serviço. |
| Firewall | Controle de tráfego de rede. O projeto não depende de regra de firewall porque não usa rede real. |
| Offline | Sem internet. O simulador pode rodar offline depois de compilado. |

## Termos específicos da apresentação

| Termo | Significado no projeto |
| --- | --- |
| Micro máquina de pacotes | Nome dado à simulação local com fila, pilha, lista e ambiente de rede. |
| Demo pronta | Opção que executa automaticamente o cenário-base da Questão 5. |
| Ambiente padrão | Dispositivos cadastrados ao iniciar: PC, switch, roteador, DNS e servidor. |
| Cadastro de ambiente | Tela para adicionar novos dispositivos simulados. |
| Sobrecarga da fila | Teste que preenche `MAX_FILA` e confirma recusa do próximo pacote. |
| Erro de destino | Cenário em que o destino não existe e o pacote vai para a pilha de erros. |
| Validação de entrada | Regras que impedem número/tamanho inválido, pacote duplicado e dispositivo duplicado. |
| Estado final | Visão da fila, pilha e lista após a execução da demo. |
