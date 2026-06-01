# Guia básico para rodar o projeto

Este guia é para uma pessoa que não trabalha com programação e precisa executar a demonstração do projeto no computador.

## 1. O que este projeto faz

O projeto abre uma aplicação no terminal. Ela simula uma rede pequena com:

- um computador de origem;
- um switch;
- um roteador;
- um DNS;
- um servidor;
- pacotes entrando em fila, falhando, indo para pilha e sendo acompanhados por lista encadeada.

O programa não usa internet real. Tudo acontece dentro do próprio executável.

## 2. O que precisa ter no computador

Para rodar o projeto, existem dois caminhos.

### Caminho A - Já existe um executável

Se alguém já gerou o arquivo executável, a pessoa só precisa:

- ter acesso à pasta do projeto;
- abrir um terminal;
- executar o arquivo compatível com o sistema.

No Windows, o executável costuma terminar com `.exe`.

### Caminho B - Precisa compilar

Compilar significa transformar os arquivos `.c` em um programa executável.

Para compilar, precisa ter uma destas ferramentas:

- TinyCC;
- GCC;
- MinGW;
- Clang;
- MSVC `cl`.

Na validação local deste projeto foi usado TinyCC portátil, porque ele não exigiu instalação administrativa.

## 3. Como abrir o terminal no Windows

1. Abra a pasta do projeto.
2. Clique na barra de endereço da pasta.
3. Digite `powershell`.
4. Pressione Enter.

O PowerShell deve abrir já dentro da pasta do projeto.

Para confirmar se está na pasta certa, rode:

```powershell
dir
```

Resultado esperado: aparecerem arquivos como `main.c`, `menu.c`, `rede.h` e a pasta `docs`.

## 4. Como compilar no Windows

### Forma recomendada com PowerShell

O projeto possui um script que procura `gcc`, `clang` ou `tcc` disponível e gera o executável dentro de `build`:

```powershell
.\scripts\build.ps1
```

### Com TinyCC

Use este comando:

```powershell
tcc -Wall main.c menu.c pacote.c ambiente.c interface.c simulador.c fila.c pilha.c lista-encadeada.c -o sistema-redes.exe
```

O que cada parte faz:

| Parte | Explicação |
| --- | --- |
| `tcc` | Chama o compilador TinyCC. |
| `-Wall` | Pede avisos úteis durante a compilação. |
| `main.c menu.c ...` | Lista os arquivos de código que formam o programa. |
| `-o sistema-redes.exe` | Define o nome do executável gerado. |

Se não aparecer erro, o arquivo `sistema-redes.exe` foi gerado.

### Com GCC ou MinGW

Use este comando:

```powershell
gcc main.c menu.c pacote.c ambiente.c interface.c simulador.c fila.c pilha.c lista-encadeada.c -o sistema-redes.exe
```

Se o terminal responder que `gcc` não foi encontrado, significa que o GCC não está instalado ou não está no PATH.

## 5. Como compilar no Linux ou macOS

Abra o terminal na pasta do projeto e rode:

```bash
gcc main.c menu.c pacote.c ambiente.c interface.c simulador.c fila.c pilha.c lista-encadeada.c -o sistema-redes
```

Depois rode:

```bash
./sistema-redes
```

No macOS, se usar Clang:

```bash
clang main.c menu.c pacote.c ambiente.c interface.c simulador.c fila.c pilha.c lista-encadeada.c -o sistema-redes
```

## 6. Como executar no Windows

Depois de compilar, rode:

```powershell
.\sistema-redes.exe
```

O `.\` significa: execute o arquivo que está nesta pasta.

Se o executável estiver dentro da pasta `build`, use:

```powershell
.\build\sistema-redes.exe
```

## 7. Como usar a demo pronta

Quando o menu abrir, escolha:

```text
1 - Rodar demo pronta da Questao 5
```

Digite:

```text
1
```

Depois pressione Enter.

A demo agora para em pontos importantes da apresentação. Quando aparecer:

```text
Pressione Enter para continuar...
```

leia a tela, explique o que aconteceu e só então pressione Enter.

## 8. O que observar na demo

Na demo, observe esta sequência:

1. Os quatro pacotes entram na fila.
2. O Pacote 1 sai primeiro, porque a fila é FIFO.
3. O destino `app.local` é resolvido pelo DNS.
4. O roteador aparece porque origem e destino estão em sub-redes diferentes.
5. O ARP mostra o MAC do próximo salto.
6. A animação mostra a PDU passando pela topologia.
7. O Pacote 1 é entregue e removido da lista ativa.
8. O Pacote 2 é marcado como erro.
9. A pilha mostra o Pacote 2 aguardando retransmissão.
10. A lista mostra os pacotes que ainda estão ativos.

## 9. Como cadastrar um dispositivo

No menu principal:

```text
3 - Ambiente de rede
```

Depois:

```text
2 - Cadastrar dispositivo
```

Exemplo de cadastro:

| Campo | Valor de exemplo |
| --- | --- |
| Tipo | `1` |
| Nome | `PC-02` |
| IP | `192.168.10.11` |
| MAC | `02:00:00:00:10:11` |
| Domínio | `-` |

O tipo `1` significa PC.

Depois, use:

```text
1 - Mostrar ambiente cadastrado
```

para confirmar que o dispositivo apareceu.

## 10. Como adicionar um pacote manualmente

No menu principal:

```text
2 - Operar pacotes
```

Depois:

```text
1 - Adicionar pacote manualmente
```

Exemplo:

| Campo | Valor |
| --- | --- |
| Número do pacote | `10` |
| Tamanho | `500` |
| Origem | `PC-01` |
| Destino | `app.local` |

Depois escolha:

```text
2 - Transmitir proximo pacote com animacao
```

## 11. Capacidade e sobrecarga

O projeto tem limites fixos para facilitar apresentação e teste.

| Área | Limite atual | O que acontece ao passar do limite |
| --- | ---: | --- |
| Fila de transmissão | 20 pacotes | O pacote seguinte é recusado. |
| Pilha de erros | 20 pacotes | O próximo erro não é registrado na pilha. |
| Dispositivos no ambiente | 12 dispositivos | O próximo cadastro é recusado. |

O ambiente padrão já começa com 5 dispositivos:

- `PC-01`;
- `SWITCH-01`;
- `ROTEADOR-01`;
- `DNS-01`;
- `SERVIDOR-01`.

Por isso, o usuário consegue cadastrar mais 7 dispositivos antes de atingir `MAX_DISPOSITIVOS = 12`.

Importante: esse limite não é velocidade de rede. É a capacidade didática das estruturas dentro do programa.

## 12. Ferramentas usadas no projeto

| Ferramenta | Uso no projeto |
| --- | --- |
| C | Linguagem de programação principal. |
| TinyCC 0.9.27 | Compilador usado na validação local. |
| GCC | Compilador documentado para Linux. |
| MinGW/GCC | Ambiente documentado para gerar executável no Windows. |
| Clang | Compilador documentado para macOS e como alternativa em Linux. |
| PowerShell | Terminal usado nos testes locais do Windows. |
| Git | Controle de versão e verificação com `git diff --check`. |
| ripgrep (`rg`) | Busca rápida em arquivos do projeto. |
| System.Drawing via PowerShell | Geração das imagens de evidência a partir de transcripts. |
| Cppcheck | Ferramenta recomendada para análise estática futura. |
| Valgrind/Memcheck | Ferramenta recomendada para análise de memória futura em ambiente Linux. |
| `scripts/build.ps1` | Script que padroniza o build local no PowerShell. |
| `scripts/validar-projeto.ps1` | Script que compila e percorre cenários críticos automaticamente. |

Evidência das ferramentas:

- `docs/assets/evidencia-ferramentas-validacao.png`
- `docs/assets/evidencia-ferramentas-validacao.txt`

## 13. Erros comuns

| Erro | O que significa | Como resolver |
| --- | --- | --- |
| `gcc não é reconhecido` | GCC não está instalado ou não está no PATH. | Usar TinyCC, instalar MinGW/GCC ou compilar em outra máquina. |
| `tcc não é reconhecido` | TinyCC não está no PATH. | Rodar o comando usando o caminho completo do `tcc.exe`. |
| `Permission denied` | A pasta ou executável está bloqueado. | Compilar em uma pasta do usuário ou fechar o executável aberto. |
| PowerShell tenta importar `build` | Faltou `.\` antes do caminho. | Usar `.\build\sistema-redes.exe`. |
| A tela limpa e muda | É o comportamento do app de terminal. | Ler a etapa e pressionar Enter quando solicitado. |
| Destino não encontrado | Nome, IP ou domínio não existe no ambiente. | Verificar o ambiente cadastrado ou usar `app.local`. |

## 14. Ordem recomendada para apresentação

1. Abrir o terminal.
2. Rodar o executável.
3. Escolher a opção `1`.
4. Explicar a entrada dos quatro pacotes.
5. Explicar DNS, ARP e roteador antes da animação.
6. Explicar FIFO após o Pacote 1 sair primeiro.
7. Explicar erro e pilha com o Pacote 2.
8. Mostrar o estado final.
9. Abrir o cadastro de ambiente.
10. Mostrar os limites de sobrecarga.

## 15. Fechamento

Para uma pessoa não técnica, o caminho mais seguro é:

1. abrir a pasta do projeto;
2. abrir o terminal;
3. executar o binário pronto;
4. escolher a opção `1`;
5. pressionar Enter somente depois de ler cada tela.

Se não houver binário pronto, a pessoa precisa primeiro rodar `.\scripts\build.ps1` ou compilar manualmente com TinyCC, GCC, MinGW ou Clang.
