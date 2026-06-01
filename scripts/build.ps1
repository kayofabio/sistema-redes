param(
    [string] $Compiler = "",
    [string] $Output = "build\sistema-redes.exe",
    [switch] $WarningsAsErrors
)

$ErrorActionPreference = "Stop"
$root = Split-Path -Parent $PSScriptRoot
$sources = @(
    "main.c",
    "menu.c",
    "pacote.c",
    "ambiente.c",
    "interface.c",
    "simulador.c",
    "fila.c",
    "pilha.c",
    "lista-encadeada.c"
)

function Find-Compiler {
    if ($Compiler) {
        if (Test-Path -LiteralPath $Compiler) {
            return (Resolve-Path -LiteralPath $Compiler).Path
        }

        $explicitCommand = Get-Command $Compiler -ErrorAction SilentlyContinue
        if ($explicitCommand) {
            return $explicitCommand.Source
        }

        throw "Compilador informado nao foi encontrado: $Compiler"
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

    throw "Nenhum compilador encontrado. Instale GCC, Clang ou TinyCC, ou use -Compiler com o caminho completo."
}

$compilerPath = Find-Compiler
$outputPath = if ([System.IO.Path]::IsPathRooted($Output)) {
    $Output
} else {
    Join-Path $root $Output
}

$outputDirectory = Split-Path -Parent $outputPath
New-Item -ItemType Directory -Force -Path $outputDirectory | Out-Null

$arguments = @("-Wall")
if ($WarningsAsErrors) {
    $arguments += "-Werror"
}
$arguments += $sources
$arguments += @("-o", $outputPath)

Push-Location $root
try {
    & $compilerPath @arguments
    if ($LASTEXITCODE -ne 0) {
        throw "Falha de compilacao. Exit code: $LASTEXITCODE"
    }
} finally {
    Pop-Location
}

Write-Host "Build concluido: $outputPath"
Write-Host "Compilador: $compilerPath"
