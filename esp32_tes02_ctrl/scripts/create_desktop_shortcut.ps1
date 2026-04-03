param(
  [string]$Url = "",
  [string]$Suffix = "",
  [string]$HostPrefix = "alb",
  [string]$Name = "LB-Control",
  [int]$TimeoutSec = 8
)

$ErrorActionPreference = "Stop"

$Suffix = ($Suffix -replace "[^0-9A-Fa-f]", "").ToLowerInvariant()
if ([string]::IsNullOrWhiteSpace($Url)) {
  if ([string]::IsNullOrWhiteSpace($Suffix)) {
    throw "Pass -Url http://alb-xxxxxx.local/ or -Suffix xxxxxx"
  }
  $Url = "http://$HostPrefix-$Suffix.local/"
}

if ($Url -notmatch "^https?://") {
  $Url = "http://$Url"
}
if (-not $Url.EndsWith("/")) {
  $Url += "/"
}

$scriptDir = Split-Path -Parent $MyInvocation.MyCommand.Path
$rootDir = Resolve-Path (Join-Path $scriptDir "..\..")
$localIcoPath = Join-Path $rootDir "ui\LB.ico"

$appDir = Join-Path $env:LOCALAPPDATA "LB-Control"
New-Item -ItemType Directory -Force -Path $appDir | Out-Null

$icoPath = Join-Path $appDir "LB.ico"
$desktop = [Environment]::GetFolderPath("Desktop")
$lnkPath = Join-Path $desktop "$Name.lnk"

try {
  Invoke-WebRequest -Uri ($Url + "LB.ico") -UseBasicParsing -OutFile $icoPath -TimeoutSec $TimeoutSec
} catch {
  if (Test-Path $localIcoPath) {
    Copy-Item -Force $localIcoPath $icoPath
  }
}

$ws = New-Object -ComObject WScript.Shell
$sc = $ws.CreateShortcut($lnkPath)
$sc.TargetPath = "$env:WINDIR\explorer.exe"
$sc.Arguments = $Url
if (Test-Path $icoPath) {
  $sc.IconLocation = "$icoPath,0"
}
$sc.WorkingDirectory = $desktop
$sc.Description = "LB Control"
$sc.Save()

Write-Host "Shortcut created: $lnkPath"
Write-Host "Target URL: $Url"
Write-Host "Icon: $icoPath"
