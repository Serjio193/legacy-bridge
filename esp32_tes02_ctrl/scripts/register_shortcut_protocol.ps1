param(
  [string]$Scheme = "lb-shortcut"
)

$ErrorActionPreference = "Stop"

$scriptDir = Split-Path -Parent $MyInvocation.MyCommand.Path
$handler = Join-Path $scriptDir "handle_shortcut_protocol.ps1"
if (-not (Test-Path $handler)) {
  throw "Missing handler script: $handler"
}

$root = "Registry::HKEY_CURRENT_USER\Software\Classes\$Scheme"
New-Item -Path $root -Force | Out-Null
Set-ItemProperty -Path $root -Name "(default)" -Value "URL:LB Shortcut Protocol"
Set-ItemProperty -Path $root -Name "URL Protocol" -Value ""

$iconKey = "$root\DefaultIcon"
New-Item -Path $iconKey -Force | Out-Null
Set-ItemProperty -Path $iconKey -Name "(default)" -Value "$env:SystemRoot\System32\WindowsPowerShell\v1.0\powershell.exe,0"

$cmdKey = "$root\shell\open\command"
New-Item -Path $cmdKey -Force | Out-Null
$psExe = "$env:SystemRoot\System32\WindowsPowerShell\v1.0\powershell.exe"
$cmd = "`"$psExe`" -NoProfile -ExecutionPolicy Bypass -File `"$handler`" `"%1`""
Set-ItemProperty -Path $cmdKey -Name "(default)" -Value $cmd

Write-Host "Registered protocol: ${Scheme}://"
Write-Host "Handler: $handler"
Write-Host "Run test in browser:"
Write-Host "${Scheme}://install?url=http://alb-cbb9e4.local/&name=LB-Control"
