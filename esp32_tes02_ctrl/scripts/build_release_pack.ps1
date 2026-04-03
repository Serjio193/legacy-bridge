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

  Write-Host "Build: System (esp32c3)"
  pio run -e esp32c3 | Out-Host
  Write-Host "Build: LittleFS (esp32c3)"
  pio run -e esp32c3 -t buildfs | Out-Host
  Write-Host "Build: Recovery (esp32c3_recovery)"
  pio run -e esp32c3_recovery | Out-Host

  Write-Host "Sign: firmware/littlefs"
  powershell -ExecutionPolicy Bypass -File (Join-Path $proj "scripts\sign_current_build.ps1") -Env esp32c3 | Out-Host

  Write-Host "Pack: update.lbpack"
  powershell -ExecutionPolicy Bypass -File (Join-Path $proj "scripts\make_update_pack.ps1") -Env esp32c3 | Out-Host

  $mainSrc = Get-Content (Join-Path $proj "src\main.cpp") -Raw
  $recSrc = Get-Content (Join-Path $proj "src\recovery_main.cpp") -Raw
  $fwVer = "unknown"
  $recVer = "unknown"
  $mFwDef = [regex]::Match($mainSrc, 'LB_FW_VERSION\s*"([^"]+)"')
  if ($mFwDef.Success) { $fwVer = $mFwDef.Groups[1].Value.Trim() }
  $mRec = [regex]::Match($recSrc, 'LB_RECOVERY_VERSION\s*"([^"]+)"')
  if ($mRec.Success) { $recVer = $mRec.Groups[1].Value.Trim() }

  $stamp = Get-Date -Format "yyyyMMdd-HHmmss"
  $safeFw = ($fwVer -replace '[^a-zA-Z0-9._-]', '_')
  $safeRec = ($recVer -replace '[^a-zA-Z0-9._-]', '_')
  $outDir = Join-Path $proj ("release\bundle-{0}-fw-{1}-rec-{2}" -f $stamp, $safeFw, $safeRec)
  New-Item -ItemType Directory -Force -Path $outDir | Out-Null

  $sysBuild = Join-Path $proj ".pio\build\esp32c3"
  $recBuild = Join-Path $proj ".pio\build\esp32c3_recovery"

  $files = @(
    @{ src = (Join-Path $sysBuild "firmware.bin"); dst = "firmware.bin" },
    @{ src = (Join-Path $sysBuild "firmware.sig"); dst = "firmware.sig" },
    @{ src = (Join-Path $sysBuild "littlefs.bin"); dst = "littlefs.bin" },
    @{ src = (Join-Path $sysBuild "littlefs.sig"); dst = "littlefs.sig" },
    @{ src = (Join-Path $sysBuild "update.lbpack"); dst = "update.lbpack" },
    @{ src = (Join-Path $recBuild "firmware.bin"); dst = "recovery.bin" }
  )

  foreach ($f in $files) {
    if (!(Test-Path $f.src)) { throw "Missing artifact: $($f.src)" }
    Copy-Item -Force $f.src (Join-Path $outDir $f.dst)
  }

  $manifest = [ordered]@{
    generated_at = (Get-Date).ToString("s")
    fw_version = $fwVer
    recovery_version = $recVer
    pack_format = "LBPACK v1"
    artifacts = @()
  }
  foreach ($name in @("firmware.bin","firmware.sig","littlefs.bin","littlefs.sig","update.lbpack","recovery.bin")) {
    $p = Join-Path $outDir $name
    $h = (Get-FileHash -Algorithm SHA256 -Path $p).Hash.ToLower()
    $size = (Get-Item $p).Length
    $manifest.artifacts += [ordered]@{ name = $name; size = $size; sha256 = $h }
  }
  ($manifest | ConvertTo-Json -Depth 5) | Set-Content -Encoding UTF8 (Join-Path $outDir "manifest.json")

  Write-Host ""
  Write-Host "Release bundle ready:"
  Write-Host "  $outDir"
  Write-Host ""
  Write-Host "Use in Recovery: update.lbpack"
  Write-Host "Use via cable for Recovery app: recovery.bin"
}
finally {
  Pop-Location
}
