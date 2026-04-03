param(
  [string]$Src = "..\\ui",
  [string]$Dst = "data",
  [string[]]$ImmutableGzipFiles = @("three.min.js")
)

$ErrorActionPreference = "Stop"

$here = Split-Path -Parent $MyInvocation.MyCommand.Path
$proj = Resolve-Path (Join-Path $here "..")
$srcPath = Resolve-Path (Join-Path $proj $Src)
$dstPath = Join-Path $proj $Dst

New-Item -ItemType Directory -Force -Path $dstPath | Out-Null
# Clean destination first to avoid stale .gz leftovers from previous builds.
Get-ChildItem -Path $dstPath -Recurse -Force | Remove-Item -Recurse -Force
Copy-Item (Join-Path $srcPath "*") $dstPath -Recurse -Force

function Compress-GzipFile {
  param(
    [Parameter(Mandatory = $true)][string]$InputFile,
    [Parameter(Mandatory = $true)][string]$OutputFile
  )
  $bytes = [System.IO.File]::ReadAllBytes($InputFile)
  $outFs = [System.IO.File]::Create($OutputFile)
  try {
    $gz = New-Object System.IO.Compression.GzipStream($outFs, [System.IO.Compression.CompressionLevel]::Optimal)
    try {
      $gz.Write($bytes, 0, $bytes.Length)
    } finally {
      $gz.Dispose()
    }
  } finally {
    $outFs.Dispose()
  }
}

$gzCount = 0
foreach ($name in $ImmutableGzipFiles) {
  if ([string]::IsNullOrWhiteSpace($name)) { continue }
  $plain = Join-Path $dstPath $name
  if (-not (Test-Path $plain -PathType Leaf)) { continue }
  $gz = "$plain.gz"
  Compress-GzipFile -InputFile $plain -OutputFile $gz
  Remove-Item -Force $plain
  $gzCount++
}

Write-Host "Synced UI from $srcPath to $dstPath (gzip immutable files: $gzCount)"
