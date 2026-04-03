param(
  [string]$Port = "",
  [int]$Baud = 921600
)

$ErrorActionPreference = "Stop"

Push-Location (Split-Path -Parent $MyInvocation.MyCommand.Path)
try {
  $proj = Resolve-Path (Join-Path $PWD "..")
  Set-Location $proj

  # Build recovery firmware.
  pio run -e esp32c3_recovery | Out-Host

  $fw = Join-Path $proj ".pio\\build\\esp32c3_recovery\\firmware.bin"
  if (!(Test-Path $fw)) { throw "Recovery firmware not found: $fw" }

  $tool = Join-Path $env:USERPROFILE ".platformio\\packages\\tool-esptoolpy\\esptool.py"
  if (!(Test-Path $tool)) { throw "esptool.py not found: $tool" }

  $pioPy = Join-Path $env:USERPROFILE ".platformio\\penv\\Scripts\\python.exe"
  $py = if (Test-Path $pioPy) { $pioPy } else { "python" }

  if ([string]::IsNullOrWhiteSpace($Port)) {
    # Try to read upload_port from platformio.ini env:esp32c3.
    $ini = Get-Content (Join-Path $proj "platformio.ini") -Raw
    $m = [regex]::Match($ini, "(?m)^upload_port\\s*=\\s*(.+)$")
    if ($m.Success) { $Port = $m.Groups[1].Value.Trim() }
  }
  if ([string]::IsNullOrWhiteSpace($Port)) { throw "Specify -Port COMxx" }

  # Factory(recovery) partition offset from partitions_4mb_ota_fs_recovery.csv is 0x320000.
  $addr = "0x320000"

  Write-Host "Flashing recovery firmware to factory partition @ $addr on $Port ..."
  & $py $tool --chip esp32c3 --port $Port --baud $Baud write_flash -z $addr $fw | Out-Host
}
finally {
  Pop-Location
}
