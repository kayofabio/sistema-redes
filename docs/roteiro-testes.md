# Roteiro de testes locais

Este roteiro registra os testes mínimos para apresentar a implementação da Questão 5 e os testes adicionais de limite, erro e consistência.

## Ambiente usado na validação

- Sistema: Windows com execução em terminal PowerShell.
- Compilador usado no teste: TinyCC portátil 0.9.27.
- Comando de validação reproduzível:

```powershell
.\scripts\validar-projeto.ps1 -Compiler .\build\tools\tcc-0.9.27\tcc\tcc.exe
```

O projeto também deve compilar com GCC/MinGW:

```bash
gcc main.c menu.c pacote.c ambiente.c interface.c simulador.c fila.c pilha.c lista-encadeada.c -o sistema-redes
```

## Teste 1 - Cenário guiado da Questão 5

Entrada:

```text
1
0
```

Durante a demo, pressionar Enter sempre que aparecer `Pressione Enter para continuar...`.

Validações esperadas:

- Pacotes 1, 2, 3 e 4 entram na fila.
- O Pacote 1 é retirado da fila primeiro.
- O Pacote 1 simula falha porque possui `500 KB`.
- O Pacote 2 é transmitido depois, entregue e removido da lista ativa.
- Dois pacotes são removidos da fila por transmissão.
- Um pacote entregue é removido da lista.
- Um pacote com erro aparece na pilha.
- A fila final mantém Pacotes 3 e 4 aguardando.
- A saída mostra resolução DNS, ARP do próximo salto e passagem pelo roteador.
- A demo pausa entre etapas para permitir leitura durante apresentação.

Evidência:

- `docs/assets/execucao-cenario-questao-5.txt`
- `docs/assets/evidencia-demo-estado-final.txt`
- `docs/assets/evidencia-demo-estado-final.png`
- `docs/assets/evidencia-demo-pausada.txt`
- `docs/assets/evidencia-demo-pausada.png`

## Teste 2 - Pilha LIFO

Entrada:

```text
2
1
2
300


1
4
200


3
2
3
4
0
4
3

0
2
4

0
4
4

0
0
```

Validações esperadas:

- Pacote 2 é registrado com erro.
- Pacote 4 é registrado com erro depois.
- A pilha mostra Pacote 4 no topo.
- A retransmissão remove primeiro o Pacote 4.
- A lista mostra Pacote 4 como `em_transito` após a retransmissão.

Evidência:

- `docs/assets/execucao-pilha-lifo.txt`

## Teste 3 - Busca na lista

Fluxo manual:

1. Adicionar um pacote.
2. Selecionar `4 - Consultas e estruturas`.
3. Selecionar `1 - Buscar pacote ativo`.
4. Informar o número do pacote.

Resultado esperado:

- O programa imprime ID, número, tamanho, tempo estimado, origem, destino e status.

## Teste 4 - Remoção de entregue

Fluxo manual:

1. Adicionar um pacote.
2. Transmitir o próximo pacote.
3. Selecionar `2 - Operar pacotes`.
4. Selecionar `5 - Marcar entregue e remover da lista`.
5. Informar o número do pacote transmitido.
6. Listar a lista de ativos.

Resultado esperado:

- O pacote marcado como entregue não aparece mais na lista de ativos.

## Teste 5 - Cadastro de ambiente

Fluxo manual:

1. Selecionar `3 - Ambiente de rede`.
2. Selecionar `2 - Cadastrar dispositivo`.
3. Informar tipo `1`, nome `PC-02`, IP `192.168.10.11`, MAC `02:00:00:00:10:11` e domínio `-`.
4. Selecionar `3 - Ambiente de rede`.
5. Selecionar `1 - Mostrar ambiente cadastrado`.

Resultado esperado:

- O novo dispositivo aparece na tabela do ambiente.

Evidência:

- `docs/assets/evidencia-cadastro-ambiente.txt`
- `docs/assets/evidencia-cadastro-ambiente.png`

## Teste 6 - Transmissão animada com DNS e ARP

Fluxo manual:

1. Selecionar `2 - Operar pacotes`.
2. Selecionar `1 - Adicionar pacote manualmente`.
3. Informar pacote `1`, tamanho `500`, origem `PC-01` e destino `app.local`.
4. Selecionar `2 - Transmitir proximo pacote com animacao`.

Resultado esperado:

- O programa mostra a consulta DNS para `app.local`.
- O programa mostra ARP para o próximo salto.
- O terminal alterna telas da animação ASCII até o destino.

Evidência:

- `docs/assets/execucao-animacao-pdu.txt`

## Teste 7 - Sobrecarga da fila

Fluxo manual:

1. Selecionar `2 - Operar pacotes`.
2. Selecionar `1 - Adicionar pacote manualmente`.
3. Repetir o cadastro até completar `MAX_FILA = 20`.
4. Tentar cadastrar mais um pacote.

Resultado esperado:

- Os 20 primeiros pacotes entram na fila.
- O pacote seguinte é recusado com mensagem de fila cheia.
- O programa não deve solicitar nem gravar dados adicionais para o pacote recusado.

Evidência:

- `docs/assets/evidencia-sobrecarga-fila.txt`
- `docs/assets/evidencia-sobrecarga-fila.png`

## Teste 8 - Destino inexistente

Fluxo manual:

1. Selecionar `2 - Operar pacotes`.
2. Selecionar `1 - Adicionar pacote manualmente`.
3. Informar destino `destino-inexistente`.
4. Selecionar `2 - Transmitir proximo pacote com animacao`.

Resultado esperado:

- O destino não é encontrado por nome, IP ou domínio.
- O pacote é marcado como erro.
- O pacote entra na pilha de retransmissão.

Evidência:

- `docs/assets/evidencia-erro-destino.txt`
- `docs/assets/evidencia-erro-destino.png`

## Teste 9 - Validação de pacote inválido e duplicado

Fluxo manual:

1. Tentar adicionar pacote com número menor ou igual a zero.
2. Tentar adicionar pacote com tamanho menor ou igual a zero.
3. Adicionar um pacote válido.
4. Tentar adicionar outro pacote com o mesmo número enquanto o primeiro ainda está ativo.

Resultado esperado:

- Número e tamanho inválidos são recusados.
- Pacote duplicado é recusado.
- Fila, pilha e lista não ficam com estado ambíguo.

Evidência:

- `docs/assets/evidencia-validacao-entrada.txt`
- `docs/assets/evidencia-validacao-entrada.png`

## Teste 10 - Cadastro duplicado de dispositivo

Fluxo manual:

1. Selecionar `3 - Ambiente de rede`.
2. Selecionar `2 - Cadastrar dispositivo`.
3. Tentar cadastrar nome, IP ou domínio já existente.

Resultado esperado:

- O cadastro é recusado.
- A tabela de ambiente continua sem duas entradas para o mesmo nome, IP ou domínio.

Evidência:

- `docs/assets/evidencia-validacao-entrada.txt`
- `docs/assets/evidencia-validacao-entrada.png`

## Teste 11 - Ferramentas de validação

Fluxo:

1. Verificar compilador disponível.
2. Compilar com TinyCC portátil.
3. Rodar `git diff --check`.
4. Registrar ferramentas ausentes no PATH para não confundir limitação de ambiente com falha do projeto.

Resultado esperado:

- TinyCC informa versão.
- Build termina com exit code `0`.
- `git diff --check` termina com exit code `0`.
- GCC, Clang, Cppcheck, Valgrind e Make ficam documentados como validação externa pendente quando não estiverem no PATH local.

Evidência:

- `docs/assets/evidencia-ferramentas-validacao.txt`
- `docs/assets/evidencia-ferramentas-validacao.png`

## Teste 12 - Baixo privilégio

Fluxo:

1. Varrer os arquivos `.c` e `.h` procurando chamadas com efeito externo ou privilégio elevado.
2. Confirmar que a execução usa apenas terminal, memória do processo e pausa local.
3. Confirmar que o programa não depende de rede real, arquivo, serviço, registro do Windows ou administrador.

Comando usado:

```powershell
rg -n "\b(fopen|remove|rename|system|CreateFile|RegOpen|socket|connect|bind|listen|accept|send|recv|ShellExecute)\s*\(" -g "*.c" -g "*.h"
```

Resultado esperado:

- Nenhuma ocorrência em código C/H.
- O programa é classificado como executável de terminal local, compatível com usuário comum.

Evidência:

- `docs/assets/evidencia-baixo-privilegio.txt`
- `docs/assets/evidencia-baixo-privilegio.png`

## Observações

- O teste não mede desempenho real de rede.
- A taxa de `100 KB/s` é fixa e didática.
- Os arquivos de imagem são evidência visual gerada a partir da saída textual local do executável.
- `scripts/validar-projeto.ps1` alimenta o modo interativo por entrada padrão e automatiza a bateria curta de regressão.
- Um modo dedicado `--demo` ou `--test` continua sendo melhoria opcional para simplificar novas capturas.
- O teste de baixo privilégio avalia o comportamento do código. Políticas externas da instituição ainda podem bloquear execução de binários baixados.
