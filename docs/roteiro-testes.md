# Roteiro de testes locais

Este roteiro registra os testes mínimos para apresentar a implementação da Questão 5.

## Ambiente usado na validação

- Sistema: Windows com execução em terminal PowerShell.
- Compilador usado no teste: TinyCC portátil 0.9.27.
- Comando de compilação:

```powershell
tcc main.c pacote.c ambiente.c interface.c simulador.c fila.c pilha.c lista-encadeada.c -o build\sistema-redes.exe
```

O projeto também deve compilar com GCC/MinGW:

```bash
gcc main.c pacote.c ambiente.c interface.c simulador.c fila.c pilha.c lista-encadeada.c -o sistema-redes
```

## Teste 1 - Cenário guiado da Questão 5

Entrada:

```text
12

0
```

Validações esperadas:

- Pacotes 1, 2, 3 e 4 entram na fila.
- O Pacote 1 é transmitido primeiro.
- Dois pacotes são removidos da fila por transmissão.
- Um pacote entregue é removido da lista.
- Um pacote com erro aparece na pilha.
- A fila final mantém Pacotes 3 e 4 aguardando.
- A saída mostra resolução DNS, ARP do próximo salto e passagem pelo roteador.

Evidência:

- `docs/assets/execucao-cenario-questao-5.txt`
- `docs/assets/cenario-questao-5.png`
- `docs/assets/execucao-animacao-pdu.txt`
- `docs/assets/animacao-pdu.png`

## Teste 2 - Pilha LIFO

Entrada:

```text
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
10
6
11

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
- `docs/assets/pilha-lifo.png`

## Teste 3 - Busca na lista

Fluxo manual:

1. Adicionar um pacote.
2. Selecionar a opção `5 - Buscar pacote ativo`.
3. Informar o número do pacote.

Resultado esperado:

- O programa imprime ID, número, tamanho, tempo estimado, origem, destino e status.

## Teste 4 - Remoção de entregue

Fluxo manual:

1. Adicionar um pacote.
2. Transmitir o próximo pacote.
3. Selecionar a opção `6 - Marcar entregue e remover da lista`.
4. Informar o número do pacote transmitido.
5. Listar a lista de ativos.

Resultado esperado:

- O pacote marcado como entregue não aparece mais na lista de ativos.

## Teste 5 - Cadastro de ambiente

Fluxo manual:

1. Selecionar `1 - Cadastrar dispositivo no ambiente`.
2. Informar tipo `1`, nome `PC-02`, IP `192.168.10.11`, MAC `02:00:00:00:10:11` e domínio `-`.
3. Selecionar `2 - Mostrar ambiente de rede`.

Resultado esperado:

- O novo dispositivo aparece na tabela do ambiente.

## Teste 6 - Transmissão animada com DNS e ARP

Fluxo manual:

1. Selecionar `3 - Adicionar pacote a fila`.
2. Informar pacote `1`, tamanho `500`, origem `PC-01` e destino `app.local`.
3. Selecionar `4 - Transmitir proximo pacote com animacao`.

Resultado esperado:

- O programa mostra a consulta DNS para `app.local`.
- O programa mostra ARP para o próximo salto.
- O terminal alterna telas da animação ASCII até o destino.

## Observações

- O teste não mede desempenho real de rede.
- A taxa de `100 KB/s` é fixa e didática.
- Os arquivos de imagem são evidência visual gerada a partir da saída textual real do executável.
