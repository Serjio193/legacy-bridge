param(
  [string]$Env = "esp32c3"
)

$ErrorActionPreference = "Stop"

Push-Location (Split-Path -Parent $MyInvocation.MyCommand.Path)
try {
  $projRoot = Resolve-Path (Join-Path $PWD "..")
  $workspaceRoot = Resolve-Path (Join-Path $projRoot "..")

  $privKey = Join-Path $workspaceRoot "security_private.pem"
  $pubKey = Join-Path $workspaceRoot "security_public.pem"
  $buildDir = Join-Path $projRoot (".pio\\build\\{0}" -f $Env)
  $firmwareBin = Join-Path $buildDir "firmware.bin"
  $littlefsBin = Join-Path $buildDir "littlefs.bin"
  $firmwareSig = Join-Path $buildDir "firmware.sig"
  $littlefsSig = Join-Path $buildDir "littlefs.sig"
  $signTool = Join-Path $projRoot "tools\\sign_bin.py"

  if (!(Test-Path $privKey)) { throw "Missing key: $privKey" }
  if (!(Test-Path $pubKey)) { throw "Missing key: $pubKey" }
  if (!(Test-Path $firmwareBin)) { throw "Missing firmware: $firmwareBin (run build first)" }
  if (!(Test-Path $littlefsBin)) { throw "Missing littlefs: $littlefsBin (run buildfs first)" }

  Write-Host "Signing with root keys:"
  Write-Host "  private: $privKey"
  Write-Host "  public : $pubKey"
  Write-Host "  env    : $Env"

  python $signTool sign --bin $littlefsBin --private $privKey --sig $littlefsSig | Out-Host
  if ($LASTEXITCODE -ne 0) { throw "sign littlefs failed" }
  python $signTool sign --bin $firmwareBin --private $privKey --sig $firmwareSig | Out-Host
  if ($LASTEXITCODE -ne 0) { throw "sign firmware failed" }

  python $signTool verify --bin $littlefsBin --public $pubKey --sig $littlefsSig | Out-Host
  if ($LASTEXITCODE -ne 0) { throw "verify littlefs failed" }
  python $signTool verify --bin $firmwareBin --public $pubKey --sig $firmwareSig | Out-Host
  if ($LASTEXITCODE -ne 0) { throw "verify firmware failed" }

  Write-Host ""
  Write-Host "Done:"
  Write-Host "  $littlefsSig"
  Write-Host "  $firmwareSig"
}
finally {
  Pop-Location
}
