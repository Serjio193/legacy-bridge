param(
  [switch]$Clean
)

$ErrorActionPreference = "Stop"

Push-Location (Split-Path -Parent $MyInvocation.MyCommand.Path)
try {
  $proj = Resolve-Path (Join-Path $PWD "..")
  Set-Location $proj

  if ($Clean) {
    Write-Host "Cleaning build artifacts..."
    pio run -t clean | Out-Host
  }

  Write-Host "Building Standard channel (env: esp32c3)..."
  pio run -e esp32c3 | Out-Host

  Write-Host "Building Hardened channel (env: esp32c3_hardened)..."
  pio run -e esp32c3_hardened | Out-Host

  $stdBin = Join-Path $proj ".pio\\build\\esp32c3\\firmware.bin"
  $hardBin = Join-Path $proj ".pio\\build\\esp32c3_hardened\\firmware.bin"
  if (!(Test-Path $stdBin)) { throw "Standard firmware not found: $stdBin" }
  if (!(Test-Path $hardBin)) { throw "Hardened firmware not found: $hardBin" }

  $srcMain = Get-Content (Join-Path $proj "src\\main.cpp") -Raw
  $ver = "unknown"
  $m = [regex]::Match($srcMain, 'LB_FW_VERSION\s*"([^"]+)"')
  if ($m.Success) { $ver = $m.Groups[1].Value.Trim() }
  $verSafe = ($ver -replace "[^a-zA-Z0-9._-]", "_")
  $stamp = Get-Date -Format "yyyyMMdd-HHmmss"

  $relDir = Join-Path $proj "release"
  New-Item -ItemType Directory -Force -Path $relDir | Out-Null

  $outStd = Join-Path $relDir ("lb-legacy-bridge-{0}-standard-{1}.bin" -f $verSafe, $stamp)
  $outHard = Join-Path $relDir ("lb-legacy-bridge-{0}-hardened-{1}.bin" -f $verSafe, $stamp)
  Copy-Item -Force $stdBin $outStd
  Copy-Item -Force $hardBin $outHard

  Write-Host ""
  Write-Host "Release binaries ready:"
  Write-Host "  Standard : $outStd"
  Write-Host "  Hardened : $outHard"
  Write-Host ""
  Write-Host "Note: Hardened profile is a release-channel marker."
  Write-Host "      eFuse burn policy should be executed only via explicit production workflow."
}
finally {
  Pop-Location
}
