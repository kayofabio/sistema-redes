# Auditoria pré-PR

Data: 2026-06-01.

Escopo: revisão local completa antes de abrir PR para `kayofabio/sistema-redes`.

## 1. Parecer

O simulador atende ao núcleo da Questão 5 e possui regressão local reproduzível. A branch foi reconciliada com a `origin/main`, o PDF do grupo foi preservado e os conflitos foram resolvidos mantendo a implementação modular auditada.

## 2. Fonte de verdade e divergência documental

A fonte principal continua sendo o enunciado original da Questão 5: fila, pilha, lista encadeada, quatro pacotes, duas transmissões, uma falha e uma entrega removida.

A `origin/main` adicionou `simulacao-e-respostas.pdf`. O arquivo inclui uma regra criada para teste: pacotes com `500 KB` ou mais são enviados para a pilha de erros.

Decisão aplicada:

- a demo guiada usa a regra por tamanho para reproduzir o teste do grupo;
- o modo manual mantém falha explícita;
- retransmissões manuais não ficam presas em um ciclo automático de erro.

## 3. Integração com a principal

Situação encontrada antes da integração:

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

O PDF `simulacao-e-respostas.pdf` foi preservado durante a integração. A principal ainda continha `main.exe`; a remoção do binário antigo foi mantida conscientemente após resolver os conflitos.

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

## 7. Fechamento

Concluído antes de abrir o PR:

1. `.\scripts\validar-projeto.ps1` executado após a integração.
2. Evidências regeneradas com `.\scripts\gerar-evidencias.ps1`.
3. Estado final conferido: Pacote 1 na pilha, Pacote 2 removido da lista e Pacotes 3 e 4 aguardando na fila.
