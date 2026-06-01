# Auditoria pré-PR

Data: 2026-06-01.

Escopo: revisão local completa antes de abrir PR para `kayofabio/sistema-redes`.

## 1. Parecer

O simulador atende ao núcleo da Questão 5 e possui regressão local reproduzível. O PR ainda não deve ser aberto porque a `origin/main` avançou quatro commits durante o desenvolvimento e trouxe alterações conflitantes nos módulos centrais.

Antes da publicação, a branch precisa ser reconciliada com a principal e validada novamente.

## 2. Fonte de verdade e divergência documental

A fonte principal continua sendo o enunciado original da Questão 5: fila, pilha, lista encadeada, quatro pacotes, duas transmissões, uma falha e uma entrega removida.

A `origin/main` adicionou `simulacao-e-respostas.pdf`. O arquivo inclui uma regra criada para teste: pacotes com `500 KB` ou mais são enviados para a pilha de erros. Essa regra não aparece no enunciado original.

Pendência para o grupo:

- manter falha explícita como comportamento principal;
- transformar a regra por tamanho em modo opcional;
- ou assumir a regra por tamanho como decisão do grupo e atualizar código, demo e documentação.

## 3. Bloqueio de integração

Situação medida em 2026-06-01:

```text
branch local: 3 commits à frente
origin/main: 4 commits à frente
```

Arquivos com conflito previsto pela análise de merge:

- `README.md`;
- `fila.c`;
- `lista-encadeada.c`;
- `main.c`;
- `pilha.c`;
- `rede.h`.

O PDF `simulacao-e-respostas.pdf` existente na `origin/main` deve ser preservado durante a integração. A principal ainda contém `main.exe`; a remoção do binário antigo deve ser mantida conscientemente após resolver os conflitos.

## 4. Falhas encontradas e corrigidas

| Falha | Risco | Correção |
| --- | --- | --- |
| `main.exe` antigo versionado | Executar comportamento diferente do código-fonte | Removido da árvore versionada. |
| Multiplicação inteira no tempo estimado | Overflow para tamanho alto | Cálculo alterado para `long long`. |
| Texto maior que o buffer | Resíduo consumido pelo prompt seguinte | Linha excedente descartada e entrada solicitada novamente. |
| IPv4 malformado aceito | Ambiente incoerente | Validação de quatro octetos entre `0` e `255`. |
| MAC malformado ou duplicado aceito | Identidade ambígua | Validação hexadecimal e rejeição de duplicidade. |
| Pilha cheia durante registro de erro | Perda ou divergência de estado | Pacote permanece no estado anterior quando a pilha recusa inserção. |
| Falha de rota com pilha cheia | Pacote poderia sair da fila sem registro | Remoção da fila ocorre somente após empilhamento bem-sucedido. |
| Rota local com animação de roteador | Explicação visual incorreta | Animação separada para rota local e rota via roteador. |
| Retransmissão sem nova resolução | Status podia mudar sem tentativa válida | Retransmissão refaz resolução e retorna à pilha quando necessário. |
| Tabelas com texto e inteiros longos | Saída desalinhada | Resumo textual e colunas numéricas ampliadas. |
| Limpeza de tela dependente de shell | Fragilidade e efeito externo desnecessário | API de console no Windows e ANSI em POSIX. |

## 5. Validação executada

Comando:

```powershell
.\scripts\validar-projeto.ps1 -Compiler .\build\tools\tcc-0.9.27\tcc\tcc.exe
```

Resultado:

```text
VALIDACAO_OK
```

A bateria automatizada cobre:

- build com `-Wall -Werror`;
- demo da Questão 5;
- leitura numérica inválida;
- texto longo;
- cálculo com `2147483647 KB`;
- rota local sem roteador;
- destino inexistente e retransmissão inválida;
- fila cheia;
- `git diff --check`;
- ausência de executável versionado presente na árvore;
- ausência de chamadas externas ou privilegiadas no código C.

Também foram executados manualmente:

- cadastro de PC adicional;
- limite de 12 dispositivos;
- recusa de IP inválido;
- recusa de MAC inválido;
- recusa de MAC duplicado;
- pilha cheia com 20 erros;
- remoção de pacote entregue;
- busca após remoção;
- retransmissão válida com DNS e ARP.

## 6. Validações externas pendentes

Não estavam disponíveis no ambiente local:

- GCC;
- Clang;
- MSVC `cl`;
- Cppcheck;
- Valgrind/Memcheck;
- Make.

Antes da entrega final, é recomendável compilar em pelo menos mais um ambiente e rodar Cppcheck. Valgrind/Memcheck depende de ambiente compatível, normalmente Linux.

## 7. Próxima sequência

1. Confirmar com o grupo a regra adicional do PDF novo.
2. Reconciliar a branch com a `origin/main`.
3. Preservar `simulacao-e-respostas.pdf`.
4. Resolver conflitos sem perder as validações adicionadas.
5. Rodar `.\scripts\validar-projeto.ps1`.
6. Regenerar evidências afetadas pela integração.
7. Revisar o diff final antes de abrir PR.
