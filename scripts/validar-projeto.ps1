param(
    [string] $Compiler = ""
)

$ErrorActionPreference = "Stop"
$root = Split-Path -Parent $PSScriptRoot
$buildScript = Join-Path $PSScriptRoot "build.ps1"
$executable = Join-Path $root "build\sistema-redes-validacao.exe"

function Assert-Contains {
    param(
        [string] $Output,
        [string] $Expected,
        [string] $Scenario
    )

    if (-not $Output.Contains($Expected)) {
        throw "Falha em '$Scenario': texto esperado nao encontrado: $Expected"
    }
}

function Invoke-Simulator {
    param([string[]] $Lines)

    $inputText = ($Lines -join "`n") + "`n"
    return ($inputText | & $executable 2>&1 | Out-String)
}

function Add-PacketInput {
    param(
        [System.Collections.Generic.List[string]] $Lines,
        [int] $Number,
        [int] $SizeKb
    )

    $Lines.Add("1")
    $Lines.Add($Number.ToString())
    $Lines.Add($SizeKb.ToString())
    $Lines.Add("")
    $Lines.Add("")
    $Lines.Add("")
}

& $buildScript -Compiler $Compiler -Output $executable -WarningsAsErrors

$demo = Invoke-Simulator @("1", "", "", "", "", "", "", "0")
Assert-Contains $demo "Resposta direta: o primeiro pacote retirado da fila e o Pacote 1" "demo da Questao 5"
Assert-Contains $demo "Pilha: pacote 1 registrado para retransmissao." "erro registrado na demo"
Assert-Contains $demo "Pacote 2 entregue e removido da lista ativa." "entrega registrada na demo"
Assert-Contains $demo "Pilha de erros (topo para base, ordem LIFO)" "pilha final da demo"

$entradaLonga = "origem-com-nome-maior-do-que-o-limite-permitido-na-interface"
$validacaoEntrada = Invoke-Simulator @(
    "2", "1", "abc", "7", "2147483647", $entradaLonga, "PC-01", "app.local", "",
    "0", "4", "2", "", "0", "0"
)
Assert-Contains $validacaoEntrada "Entrada invalida. Digite um numero inteiro." "validacao numerica"
Assert-Contains $validacaoEntrada "Texto muito longo. Digite no maximo 49 caracteres." "limite de texto"
Assert-Contains $validacaoEntrada "21474836470" "calculo sem overflow"

$rotaLocal = Invoke-Simulator @(
    "3", "2", "1", "PC-02", "192.168.10.11", "02:00:00:00:10:11", "-", "", "0",
    "2", "1", "8", "100", "PC-01", "PC-02", "", "2", "", "", "0", "0"
)
Assert-Contains $rotaLocal "ARP local: PC-01 resolve 192.168.10.11" "rota local"
Assert-Contains $rotaLocal "[PC-01]----[SWITCH-01]----[PC-02]" "animacao local"

$destinoInvalido = Invoke-Simulator @(
    "2", "1", "9", "100", "PC-01", "destino-inexistente", "", "2", "", "4", "", "0", "0"
)
Assert-Contains $destinoInvalido 'Destino `destino-inexistente` nao encontrado por nome, IP ou dominio.' "destino inexistente"
Assert-Contains $destinoInvalido "Pacote 9 retirado do topo da pilha para nova tentativa." "retransmissao invalida"

$sobrecarga = [System.Collections.Generic.List[string]]::new()
$sobrecarga.Add("2")
for ($number = 1; $number -le 20; $number++) {
    Add-PacketInput $sobrecarga $number 100
}
$sobrecarga.Add("1")
$sobrecarga.Add("")
$sobrecarga.Add("0")
$sobrecarga.Add("0")
$saidaSobrecarga = Invoke-Simulator $sobrecarga.ToArray()
Assert-Contains $saidaSobrecarga "Fila cheia. Transmita pacotes antes de cadastrar novos." "sobrecarga da fila"

Push-Location $root
try {
    & git diff --check
    if ($LASTEXITCODE -ne 0) {
        throw "git diff --check encontrou erro de whitespace."
    }

    $trackedExecutables = & git ls-files "*.exe"
    $presentTrackedExecutables = @($trackedExecutables | Where-Object {
        Test-Path -LiteralPath (Join-Path $root $_)
    })
    if ($presentTrackedExecutables) {
        throw "Executavel versionado presente na arvore: $presentTrackedExecutables"
    }

    $forbiddenPattern = "\b(fopen|rename|system|CreateFile|RegOpen|socket|connect|bind|listen|accept|send|recv|ShellExecute)\s*\("
    $forbiddenMatches = & rg -n $forbiddenPattern -g "*.c" -g "*.h"
    if ($LASTEXITCODE -eq 0) {
        throw "Chamada externa ou privilegiada encontrada:`n$forbiddenMatches"
    }
    if ($LASTEXITCODE -ne 1) {
        throw "Falha ao executar rg para varredura de baixo privilegio."
    }

    $trailingWhitespace = & rg -n "[ `t]+$" -g "*.c" -g "*.h" -g "*.md" -g "*.txt" -g "*.ps1" -g ".gitignore" .
    if ($LASTEXITCODE -eq 0) {
        throw "Whitespace no final de linha encontrado:`n$trailingWhitespace"
    }
    if ($LASTEXITCODE -ne 1) {
        throw "Falha ao executar rg para varredura de whitespace."
    }
} finally {
    Pop-Location
}

Write-Host "VALIDACAO_OK"
Write-Host "Cenarios: demo, entrada robusta, overflow, rota local, retransmissao invalida e sobrecarga."
Write-Host "Verificacoes estaticas: whitespace, executavel versionado e chamadas externas."
