param(
  [string]$Env = "esp32c3",
  [string]$OutFile = "",
  [switch]$SkipBuild,
  [switch]$SkipUiSync
)

$ErrorActionPreference = "Stop"

Push-Location (Split-Path -Parent $MyInvocation.MyCommand.Path)
try {
  $scriptDir = Get-Location
  $projRoot = Resolve-Path (Join-Path $PWD "..")
  Set-Location $projRoot
  $buildDir = Join-Path $projRoot (".pio\\build\\{0}" -f $Env)
  $fwBin = Join-Path $buildDir "firmware.bin"
  $fwSig = Join-Path $buildDir "firmware.sig"
  $fsBin = Join-Path $buildDir "littlefs.bin"
  $fsSig = Join-Path $buildDir "littlefs.sig"
  $packTool = Join-Path $projRoot "tools\\make_update_pack.py"
  $pyExe = "py"
  $pyArgs = @("-3")

  if ([string]::IsNullOrWhiteSpace($OutFile)) {
    $OutFile = Join-Path $buildDir "update.lbpack"
  }

  if (-not $SkipUiSync) {
    $syncScript = Join-Path $scriptDir "sync_ui.ps1"
    if (Test-Path $syncScript) {
      Write-Host "Sync UI: ui -> data"
      powershell -ExecutionPolicy Bypass -File $syncScript | Out-Host
    }
  }

  if (-not $SkipBuild) {
    Write-Host "Build: firmware ($Env)"
    pio run -e $Env | Out-Host
    if ($LASTEXITCODE -ne 0) { throw "build firmware failed" }
    Write-Host "Build: littlefs ($Env)"
    pio run -e $Env -t buildfs | Out-Host
    if ($LASTEXITCODE -ne 0) { throw "build littlefs failed" }
  }

  powershell -ExecutionPolicy Bypass -File (Join-Path $scriptDir "sign_current_build.ps1") -Env $Env | Out-Host
  if ($LASTEXITCODE -ne 0) { throw "sign_current_build failed" }

  # Prefer py launcher on Windows; fallback to python when py is unavailable.
  & $pyExe @pyArgs -c "import sys" 2>$null
  if ($LASTEXITCODE -ne 0) {
    $pyExe = "python"
    $pyArgs = @()
  }

  & $pyExe @pyArgs $packTool `
    --firmware $fwBin `
    --firmware-sig $fwSig `
    --littlefs $fsBin `
    --littlefs-sig $fsSig `
    --out $OutFile | Out-Host
  if ($LASTEXITCODE -ne 0) { throw "make_update_pack.py failed" }

  Write-Host ""
  Write-Host "Done: $OutFile"
}
finally {
  Pop-Location
}
