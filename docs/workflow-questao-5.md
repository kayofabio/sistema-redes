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
6. Separar a navegação em `menu.c` quando o menu principal ficar grande.

## Etapa 5 - Testes locais

Status: concluída.

Testes mínimos:

- Cenário-base da questão.
- Demonstração de FIFO.
- Demonstração de LIFO.
- Busca na lista.
- Remoção de pacote entregue.
- Cadastro de ambiente.
- Sobrecarga da fila.
- Destino inexistente.
- Entrada inválida e duplicada.

## Etapa 6 - Documentação final

Status: concluída.

Entregáveis:

- README com build, execução, prints e respostas teóricas.
- Roteiro de testes.
- Evidências de terminal.
- Registro dos problemas encontrados e decisões.
- Raio-x de qualidade com arquitetura, decisões, testes, complexidade e lacunas.
- Documento de portabilidade e baixo privilégio.
- Glossário técnico.
- Guia básico para pessoa não técnica executar o projeto.

## Etapa 6.1 - Aprofundamento de rede

Status: concluída.

Itens adicionados:

- Cadastro de ambiente de rede.
- Resolução de destino por nome, IP ou domínio.
- Consulta DNS didática.
- Resolução ARP do próximo salto.
- Uso de roteador quando origem e destino estão em sub-redes diferentes.
- Telas limpas por ação.
- Animação ASCII da PDU no terminal.

## Etapa 6.2 - Raio-x e endurecimento local

Status: concluída.

Itens adicionados:

- Menu principal reorganizado em rotas e submenus.
- `main.c` reduzido a bootstrap.
- `menu.c` criado para navegação.
- Limpeza de tela sem `system("cls")`.
- Validação de pacote inválido e pacote duplicado.
- Validação de dispositivo duplicado por nome, IP ou domínio.
- Evidências focadas com imagens menores e legíveis.
- Documento `docs/raio-x-qualidade-projeto.md`.
- Documento `docs/portabilidade-baixo-privilegio.md`.
- Documento `docs/glossario-tecnico.md`.
- Documento `docs/guia-basico-execucao.md`.
- Prints de ferramentas e baixo privilégio em `docs/assets`.
- Pausas no cenário guiado para leitura em apresentação.

## Etapa 6.3 - Auditoria pré-PR

Status: concluída localmente.

Itens adicionados:

- `scripts/build.ps1` para padronizar compilação.
- `scripts/validar-projeto.ps1` para regressão curta automatizada.
- Correção de overflow no cálculo de tempo.
- Correção de descarte de entrada textual longa.
- Validação de IPv4, MAC e MAC duplicado.
- Proteção de estado quando fila ou pilha estão cheias.
- Animação distinta para rota local e rota via roteador.
- Retransmissão com nova resolução de rota.
- Remoção do executável antigo `main.exe`.
- Parecer técnico em `docs/auditoria-pre-pr.md`.

## Etapa 7 - PR

Status: pendente.

Condição de abertura:

- Código revisado.
- Fluxo executado localmente.
- Documentação coerente com o comportamento real do terminal.
- Reconciliar a branch com a `origin/main`, que avançou quatro commits.
- Preservar o PDF novo `simulacao-e-respostas.pdf`.
- Alinhar com o grupo a regra adicional de erro para pacotes com `500 KB` ou mais.
- Repetir `.\scripts\validar-projeto.ps1` depois da integração.
