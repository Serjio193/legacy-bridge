param(
  [string]$Port = "COM70",
  [string]$Env = "esp32c3"
)

$ErrorActionPreference = "Stop"

Push-Location (Split-Path -Parent $MyInvocation.MyCommand.Path)
try {
  $projRoot = Resolve-Path (Join-Path $PWD "..")
  Set-Location $projRoot
  $buildDir = Join-Path $projRoot (".pio\build\{0}" -f $Env)
  $recoveryBuildDir = Join-Path $projRoot ".pio\build\esp32c3_recovery"
  
  $bootloader = Join-Path $buildDir "bootloader.bin"
  $partitionTable = Join-Path $buildDir "partitions.bin"
  $firmware = Join-Path $buildDir "firmware.bin"
  $littlefs = Join-Path $buildDir "littlefs.bin"
  $recovery = Join-Path $recoveryBuildDir "firmware.bin"
  
  Write-Host "ESP32-C3 Full Flash Tool"
  Write-Host "======================"
  Write-Host ""
  Write-Host "IMPORTANT: Please put ESP32-C3 into bootloader mode FIRST:"
  Write-Host "  1. Hold BOOT button"
  Write-Host "  2. Click RESET button or reconnect USB"
  Write-Host "  3. Release BOOT button"
  Write-Host "  4. Press ENTER to continue..."
  Read-Host

  Write-Host ""
  Write-Host "Building main firmware ($Env)..."
  pio run -e $Env | Out-Host
  Write-Host ""
  Write-Host "Building LittleFS image ($Env)..."
  pio run -e $Env -t buildfs | Out-Host
  Write-Host ""
  Write-Host "Building recovery firmware (esp32c3_recovery)..."
  pio run -e esp32c3_recovery | Out-Host
  Write-Host ""
  Write-Host "Flashing to $Port..."
  Write-Host ""

  if (!(Test-Path $bootloader)) { throw "Missing: $bootloader" }
  if (!(Test-Path $partitionTable)) { throw "Missing: $partitionTable" }
  if (!(Test-Path $firmware)) { throw "Missing: $firmware" }
  if (!(Test-Path $littlefs)) { throw "Missing: $littlefs" }
  if (!(Test-Path $recovery)) { throw "Missing: $recovery" }

  $cmd = @(
    "python", "-m", "esptool",
    "--chip", "esp32c3",
    "--port", $Port,
    "--baud", "921600",
    "--before", "default_reset",
    "--after", "hard_reset",
    "write_flash",
    "-z",
    "--flash_mode", "dio",
    "--flash_freq", "80m",
    "0x0", $bootloader,
    "0x8000", $partitionTable,
    "0x10000", $firmware,
    "0x238000", $littlefs,
    "0x300000", $recovery
  )

  Write-Host "Command: $($cmd -join ' ')"
  Write-Host ""
  
  & {
    & python -m esptool `
      --chip esp32c3 `
      --port $Port `
      --baud 921600 `
      --before default_reset `
      --after hard_reset `
      write_flash -z `
      --flash_mode dio `
      --flash_freq 80m `
      0x0 $bootloader `
      0x8000 $partitionTable `
      0x10000 $firmware `
      0x238000 $littlefs `
      0x300000 $recovery
  }

  Write-Host ""
  Write-Host "Success! ESP32-C3 flashed with:"
  Write-Host "  * Bootloader"
  Write-Host "  * Partition table"
  Write-Host "  * Firmware (OTA_A)"
  Write-Host "  * LittleFS"
  Write-Host "  * Recovery image"
  Write-Host ""
  Write-Host "Device will reboot automatically."
}
finally {
  Pop-Location
}
