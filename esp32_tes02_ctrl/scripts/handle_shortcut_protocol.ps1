param(
  [string]$Uri = ""
)

$ErrorActionPreference = "Stop"

if ([string]::IsNullOrWhiteSpace($Uri)) {
  throw "URI is empty"
}

$raw = $Uri.Trim()
if ($raw.StartsWith('"') -and $raw.EndsWith('"') -and $raw.Length -ge 2) {
  $raw = $raw.Substring(1, $raw.Length - 2)
}

if (-not $raw.ToLowerInvariant().StartsWith("lb-shortcut://")) {
  throw "Unsupported URI scheme"
}

$qIndex = $raw.IndexOf("?")
$query = ""
if ($qIndex -ge 0 -and $qIndex + 1 -lt $raw.Length) {
  $query = $raw.Substring($qIndex + 1)
}

$params = @{}
if (-not [string]::IsNullOrWhiteSpace($query)) {
  foreach ($pair in $query.Split("&")) {
    if ([string]::IsNullOrWhiteSpace($pair)) { continue }
    $parts = $pair.Split("=", 2)
    $k = [System.Uri]::UnescapeDataString($parts[0]).ToLowerInvariant()
    $v = ""
    if ($parts.Length -gt 1) {
      $v = [System.Uri]::UnescapeDataString($parts[1].Replace("+", "%20"))
    }
    $params[$k] = $v
  }
}

$url = ""
if ($params.ContainsKey("url")) {
  $url = [string]$params["url"]
}
$name = "LB-Control"
if ($params.ContainsKey("name")) {
  $cand = [string]$params["name"]
  if (-not [string]::IsNullOrWhiteSpace($cand)) {
    $name = $cand
  }
}

if ([string]::IsNullOrWhiteSpace($url)) {
  throw "Missing url parameter"
}

# Basic Windows filename sanitization for shortcut name.
$name = ($name -replace '[\\/:*?"<>|]', '').Trim()
if ([string]::IsNullOrWhiteSpace($name)) {
  $name = "LB-Control"
}

$scriptDir = Split-Path -Parent $MyInvocation.MyCommand.Path
$creator = Join-Path $scriptDir "create_desktop_shortcut.ps1"
if (-not (Test-Path $creator)) {
  throw "Missing script: $creator"
}

& powershell -NoProfile -ExecutionPolicy Bypass -File $creator -Url $url -Name $name
