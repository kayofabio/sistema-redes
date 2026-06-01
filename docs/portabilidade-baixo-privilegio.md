# Portabilidade e baixo privilégio

Este documento responde uma pergunta prática: o simulador roda em uma máquina comum, com o menor privilégio possível?

Resposta curta: sim, para execução local em terminal, desde que exista um executável compatível com o sistema operacional ou um compilador C disponível. O programa não exige administrador, rede, banco de dados, serviço, instalação global nem permissão especial.

## 1. Diferença entre compilar e executar

Executar:

- precisa apenas do executável gerado para o sistema operacional correto;
- usa terminal comum;
- não cria arquivo;
- não acessa internet;
- não abre porta de rede;
- não altera configuração do sistema.

Compilar:

- precisa de um compilador C;
- pode usar GCC, MinGW, Clang, TinyCC ou outro compilador compatível;
- precisa de permissão de escrita apenas na pasta onde o binário será gerado, por exemplo `build/`;
- não precisa de administrador se o compilador estiver instalado ou disponível em modo portátil.

O mesmo binário não serve para todos os sistemas. Um `.exe` gerado para Windows não roda nativamente em Linux ou macOS. Nesses casos, o código precisa ser recompilado no sistema de destino.

## 2. Privilégios necessários

| Ação | Precisa de administrador? | Motivo |
| --- | --- | --- |
| Rodar o executável | Não | O programa usa apenas terminal e memória do processo. |
| Rodar a demo da Questão 5 | Não | A simulação é local e didática. |
| Cadastrar pacote ou dispositivo | Não | Os dados ficam apenas em memória. |
| Usar DNS/ARP da simulação | Não | DNS e ARP são simulados em tabela local. |
| Compilar em uma pasta do usuário | Não | O arquivo de saída é gravado no diretório do projeto. |
| Instalar compilador global | Pode precisar | Depende da política da máquina, mas não é requisito do projeto. |

## 3. O que o código não faz

Foi feita uma varredura nos arquivos `.c` e `.h` procurando chamadas associadas a privilégios ou efeitos externos:

```powershell
rg -n "\b(fopen|remove|rename|system|CreateFile|RegOpen|socket|connect|bind|listen|accept|send|recv|ShellExecute)\s*\(" -g "*.c" -g "*.h"
```

Resultado: sem ocorrências.

Isso significa que o código atual:

- não usa `system`;
- não abre arquivo;
- não remove arquivo;
- não renomeia arquivo;
- não acessa registro do Windows;
- não abre socket;
- não envia pacote real pela rede;
- não cria serviço;
- não altera permissão do sistema.

## 4. O que o código usa

| Recurso | Onde aparece | Justificativa |
| --- | --- | --- |
| `stdin` e `stdout` | Entrada e saída do terminal | Interface do usuário. |
| `printf`, `fgets`, `strtol` | `interface.c` e módulos de exibição | Leitura limitada, conversão numérica validada e impressão simples. |
| `sscanf` | `ambiente.c` | Separar os quatro octetos durante a validação de IPv4. |
| `Sleep` no Windows | `interface.c` | Pausa da animação. |
| `usleep` em sistemas POSIX | `interface.c` | Pausa da animação. |
| `malloc` e `free` | `lista-encadeada.c` | Lista encadeada exigida pelo enunciado. |
| Vetores fixos | Fila, pilha e ambiente | Limite previsível para apresentação. |
| API de console do Windows | `interface.c` | Limpar a tela localmente no Windows sem comando de shell. |
| Sequência ANSI | `interface.c` em POSIX | Limpar a tela sem comando de shell em Linux e macOS. |

## 5. Matriz de execução esperada

| Ambiente | Situação esperada |
| --- | --- |
| Windows 10/11 com terminal comum | Deve rodar com executável `.exe` compatível. |
| Windows sem administrador | Deve rodar se a política local permitir executar programas na pasta do usuário. |
| Linux com GCC ou Clang | Deve compilar e rodar como binário local. |
| macOS com Clang | Deve compilar e rodar como binário local. |
| Máquina sem compilador | Roda apenas se já houver executável compatível. |
| Laboratório que não permite scripts | Deve receber um `.exe` Windows preparado anteriormente; a execução da demo não depende de `.ps1`. |
| Pasta sem permissão de escrita | Pode rodar executável existente, mas não compilar novo binário ali. |
| Ambiente que bloqueia executáveis baixados | Pode exigir compilar localmente ou liberar o arquivo conforme política da instituição. |

## 6. Limitações de portabilidade

- O executável é específico por sistema operacional e arquitetura.
- A limpeza de tela usa a API de console do Windows localmente. Em Linux e macOS, usa sequência ANSI compatível com terminais modernos.
- TinyCC foi usado na validação local porque GCC, Clang, MSVC, Cppcheck, Valgrind e Make não estavam no PATH.
- A validação com GCC/Clang e análise com Cppcheck/Valgrind ainda deve ser feita em outro ambiente quando essas ferramentas estiverem disponíveis.

O build local pode ser padronizado pelo script:

```powershell
.\scripts\build.ps1
```

O script é uma conveniência para a máquina de desenvolvimento. Ele não é necessário para executar a demo em uma máquina restrita quando o `.exe` compatível já foi preparado.

## 7. Evidências relacionadas

- Ferramentas de validação: `docs/assets/evidencia-ferramentas-validacao.png`
- Baixo privilégio: `docs/assets/evidencia-baixo-privilegio.png`
- Raio-x completo: `docs/raio-x-qualidade-projeto.md`

## 8. Conclusão

O simulador foi desenhado para rodar com privilégio mínimo. Ele não depende de permissões administrativas porque não toca em recursos protegidos do sistema. A única exigência real é operacional: ter um executável compatível ou conseguir compilar o código em uma pasta onde o usuário tenha permissão de escrita.
