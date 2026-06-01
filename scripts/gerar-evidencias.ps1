param(
    [string] $Compiler = ""
)

$ErrorActionPreference = "Stop"
$root = Split-Path -Parent $PSScriptRoot
$assets = Join-Path $root "docs\assets"
$buildScript = Join-Path $PSScriptRoot "build.ps1"
$executable = Join-Path $root "build\sistema-redes-evidencias.exe"

function Invoke-Simulator {
    param([string[]] $Lines)

    $inputText = ($Lines -join "`n") + "`n"
    return ($inputText | & $executable 2>&1 | Out-String)
}

function Normalize-Text {
    param([string] $Text)

    $normalized = $Text -replace "`r", ""
    $normalized = $normalized -replace "`e\[[0-9;]*[A-Za-z]", ""
    return $normalized.Trim()
}

function Select-Block {
    param(
        [string] $Text,
        [string] $Pattern,
        [string] $Name
    )

    $match = [regex]::Match((Normalize-Text $Text), $Pattern)
    if (-not $match.Success) {
        throw "Nao foi possivel extrair a evidencia: $Name"
    }

    return $match.Value.Trim()
}

function Render-TextImage {
    param(
        [string] $Text,
        [string] $Path
    )

    Add-Type -AssemblyName System.Drawing

    $lines = (Normalize-Text $Text) -split "`n"
    $font = [System.Drawing.Font]::new("Consolas", 20, [System.Drawing.FontStyle]::Regular, [System.Drawing.GraphicsUnit]::Pixel)
    $probe = [System.Drawing.Bitmap]::new(1, 1)
    $probeGraphics = [System.Drawing.Graphics]::FromImage($probe)

    try {
        $lineHeight = [int] [Math]::Ceiling($font.GetHeight($probeGraphics)) + 4
        $contentWidth = 0
        foreach ($line in $lines) {
            $width = [int] [Math]::Ceiling($probeGraphics.MeasureString($line, $font).Width)
            if ($width -gt $contentWidth) {
                $contentWidth = $width
            }
        }
    } finally {
        $probeGraphics.Dispose()
        $probe.Dispose()
    }

    $padding = 28
    $bitmap = [System.Drawing.Bitmap]::new($contentWidth + (2 * $padding), ($lines.Count * $lineHeight) + (2 * $padding))
    $bitmap.SetResolution(144, 144)
    $graphics = [System.Drawing.Graphics]::FromImage($bitmap)

    try {
        $graphics.Clear([System.Drawing.Color]::FromArgb(20, 24, 28))
        $graphics.TextRenderingHint = [System.Drawing.Text.TextRenderingHint]::ClearTypeGridFit
        $brush = [System.Drawing.SolidBrush]::new([System.Drawing.Color]::FromArgb(235, 239, 244))

        try {
            for ($index = 0; $index -lt $lines.Count; $index++) {
                $graphics.DrawString($lines[$index], $font, $brush, $padding, $padding + ($index * $lineHeight))
            }
        } finally {
            $brush.Dispose()
        }

        $bitmap.Save($Path, [System.Drawing.Imaging.ImageFormat]::Png)
    } finally {
        $graphics.Dispose()
        $bitmap.Dispose()
        $font.Dispose()
    }
}

function Save-Evidence {
    param(
        [string] $Name,
        [string] $Text
    )

    $normalized = Normalize-Text $Text
    Set-Content -LiteralPath (Join-Path $assets "$Name.txt") -Value $normalized -Encoding utf8
    Render-TextImage $normalized (Join-Path $assets "$Name.png")
}

function Save-Transcript {
    param(
        [string] $Name,
        [string] $Text
    )

    Set-Content -LiteralPath (Join-Path $assets $Name) -Value (Normalize-Text $Text) -Encoding utf8
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

function Resolve-CompilerPath {
    if ($Compiler) {
        return (Resolve-Path -LiteralPath $Compiler).Path
    }

    foreach ($name in @("gcc", "clang", "tcc")) {
        $command = Get-Command $name -ErrorAction SilentlyContinue
        if ($command) {
            return $command.Source
        }
    }

    $localTinyCc = Join-Path $root "build\tools\tcc-0.9.27\tcc\tcc.exe"
    if (Test-Path -LiteralPath $localTinyCc) {
        return $localTinyCc
    }

    throw "Nenhum compilador encontrado para gerar evidencias."
}

New-Item -ItemType Directory -Force -Path $assets | Out-Null
$compilerPath = Resolve-CompilerPath
& $buildScript -Compiler $compilerPath -Output $executable -WarningsAsErrors

$menu = Invoke-Simulator @("0")
Save-Evidence "evidencia-menu-principal" (Select-Block $menu "(?s)===== Micro Maquina de Pacotes =====.*?Escolha:" "menu principal")

$cadastro = Invoke-Simulator @("3", "2", "1", "PC-02", "192.168.10.11", "02:00:00:00:10:11", "-", "", "0", "0")
Save-Evidence "evidencia-cadastro-ambiente" (Select-Block $cadastro "(?s)===== Cadastro de ambiente =====.*?\| PC-02\s+\| PC\s+\| 192\.168\.10\.11\s+\| 02:00:00:00:10:11 \| -\s+\|\n\+[-+]+\+" "cadastro de ambiente")

$demo = Invoke-Simulator @("1", "", "", "", "", "", "", "", "0")
Save-Transcript "execucao-cenario-questao-5.txt" $demo
Save-Evidence "evidencia-demo-estado-final" (Select-Block $demo "(?s)5\. Estado final da micro maquina\..*?Resposta direta:.*?FIFO\." "estado final da demo")
Save-Evidence "evidencia-demo-pausada" (Select-Block $demo "(?s)===== Processamento da rede =====.*?Pressione Enter para continuar\.\.\." "demo pausada")

$animacao = Invoke-Simulator @("2", "1", "10", "500", "", "", "", "2", "", "", "0", "0")
Save-Transcript "execucao-animacao-pdu.txt" $animacao

$lifo = Invoke-Simulator @(
    "2",
    "1", "2", "300", "", "", "",
    "1", "4", "200", "", "", "",
    "3", "2", "",
    "3", "4", "",
    "4", "", "",
    "0", "0"
)
Save-Transcript "execucao-pilha-lifo.txt" $lifo

$erroDestino = Invoke-Simulator @(
    "2", "1", "9", "100", "PC-01", "destino-inexistente", "", "2", "",
    "0", "4", "3", "", "0", "0"
)
$erroProcessamento = Select-Block $erroDestino '(?s)===== Processamento da rede =====.*?Pilha: pacote 9 registrado para retransmissao\.' "erro de destino"
$erroPilha = Select-Block $erroDestino '(?s)===== Pilha de erros =====.*?\|\s+1\s+\|\s+9\s+\|\s+100\s+\|\s+1000\s+\| erro\s+\|\n\+[-+]+\+' "pilha apos erro de destino"
Save-Evidence "evidencia-erro-destino" "$erroProcessamento`n`n$erroPilha"

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
$sobrecargaLinhas = (Normalize-Text $saidaSobrecarga) -split "`n" | Where-Object {
    $_ -match "Fila: pacote (18|19|20) aguardando|Fila cheia"
}
Save-Evidence "evidencia-sobrecarga-fila" (@(
    "===== Sobrecarga da fila =====",
    "",
    $sobrecargaLinhas,
    "",
    "Limite validado: MAX_FILA = 20."
) -join "`n")

$entradaLonga = "origem-com-nome-maior-do-que-o-limite-permitido-na-interface"
$validacaoPacote = Invoke-Simulator @(
    "2", "1", "abc", "7", "2147483647", $entradaLonga, "PC-01", "app.local", "",
    "0", "4", "2", "", "0", "0"
)
$validacaoDispositivo = Invoke-Simulator @(
    "3", "2", "1", "PC-IP-INVALIDO", "999.1.1.1", "02:00:00:00:10:12", "-", "", "0", "0"
)
$validacaoLinhas = @(
    "===== Validacao de entrada =====",
    "",
    ((Normalize-Text $validacaoPacote) -split "`n" | Where-Object {
        $_ -match "Entrada invalida|Texto muito longo|21474836470"
    }),
    "",
    ((Normalize-Text $validacaoDispositivo) -split "`n" | Where-Object {
        $_ -match "Cadastro recusado"
    })
)
Save-Evidence "evidencia-validacao-entrada" ($validacaoLinhas -join "`n")

$toolLines = [System.Collections.Generic.List[string]]::new()
$toolLines.Add("===== Ferramentas de validacao =====")
$toolLines.Add("")
$toolLines.Add("Compilador usado:")
$toolLines.Add($compilerPath)
$toolLines.Add((& $compilerPath -v 2>&1 | Out-String).Trim())
$toolLines.Add("")
$toolLines.Add("Ferramentas no PATH:")
foreach ($name in @("gcc", "clang", "cl", "cppcheck", "valgrind", "make")) {
    $command = Get-Command $name -ErrorAction SilentlyContinue
    if ($command) {
        $toolLines.Add("$name`: $($command.Source)")
    } else {
        $toolLines.Add("$name`: indisponivel no PATH")
    }
}
$toolLines.Add("")
$toolLines.Add("Build: -Wall -Werror")
$toolLines.Add("Regressao curta: scripts/validar-projeto.ps1")
Save-Evidence "evidencia-ferramentas-validacao" ($toolLines -join "`n")

Push-Location $root
try {
    $forbiddenPattern = "\b(fopen|remove|rename|system|CreateFile|RegOpen|socket|connect|bind|listen|accept|send|recv|ShellExecute)\s*\("
    $matches = & rg -n $forbiddenPattern -g "*.c" -g "*.h"
    if ($LASTEXITCODE -ne 1) {
        throw "A varredura de baixo privilegio encontrou ocorrencia ou falhou:`n$matches"
    }
} finally {
    Pop-Location
}

Save-Evidence "evidencia-baixo-privilegio" @"
===== Baixo privilegio e portabilidade =====

Varredura no codigo C/H:
rg -n "\b(fopen|remove|rename|system|CreateFile|RegOpen|socket|connect|bind|listen|accept|send|recv|ShellExecute)\s*\(" -g "*.c" -g "*.h"
Resultado: sem ocorrencias.

O executavel:
- usa terminal e memoria do processo;
- usa Sleep/usleep para animacao;
- nao abre arquivo nem socket;
- nao altera registro, servico ou permissao;
- nao exige administrador para executar.

Tela limpa:
- API de console no Windows;
- sequencia ANSI em sistemas POSIX.
"@

Write-Host "EVIDENCIAS_OK"
