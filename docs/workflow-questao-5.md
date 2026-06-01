# Workflow - Questão 5

Este documento acompanha o andamento da implementação para evitar perda de escopo entre análise, código, teste e documentação.

## Etapa 1 - Raio-x do repositório

Status: concluída.

Resultado:

- `sistema-redes` é o repositório alinhado à Questão 5.
- `sistema-hospital` cobre outro cenário da atividade e não deve receber esta implementação.
- O `sistema-redes` já possui fila, pilha e lista, mas ainda não possui README, cenário guiado, tempo estimado de transmissão, busca/remoção no menu e evidências.

## Etapa 2 - Verificação do PDF

Status: concluída.

Requisitos confirmados:

- Pacotes têm número, tamanho e tempo estimado.
- Fila representa pacotes aguardando transmissão.
- Pilha representa pacotes com erro aguardando retransmissão.
- Lista encadeada representa pacotes ativos na rede.
- A simulação deve usar os pacotes 1, 2, 3 e 4 com tamanhos 500 KB, 300 KB, 700 KB e 200 KB.

## Etapa 3 - PRD técnico

Status: concluída.

Documento principal:

- `docs/prd-questao-5-simulacao-redes.md`

## Etapa 4 - Implementação

Status: concluída.

Plano:

1. Ajustar `rede.h` com status textual, tempo estimado e novos protótipos.
2. Revisar `fila.c` para FIFO com remoção correta.
3. Revisar `pilha.c` para LIFO de retransmissão.
4. Revisar `lista-encadeada.c` para busca, status e remoção de entregues.
5. Reescrever o menu em `main.c` com cenário guiado.

## Etapa 5 - Testes locais

Status: concluída.

Testes mínimos:

- Cenário-base da questão.
- Demonstração de FIFO.
- Demonstração de LIFO.
- Busca na lista.
- Remoção de pacote entregue.

## Etapa 6 - Documentação final

Status: concluída.

Entregáveis:

- README com build, execução, prints e respostas teóricas.
- Roteiro de testes.
- Evidências de terminal.
- Registro dos problemas encontrados e decisões.

## Etapa 7 - PR

Status: pendente.

Condição de abertura:

- Código revisado.
- Fluxo executado localmente.
- Documentação coerente com o comportamento real do terminal.
