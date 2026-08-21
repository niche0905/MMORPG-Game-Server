param(
    [Parameter(Mandatory = $true)]
    [ValidatePattern('^[0-9]+\.[0-9]+\.[0-9]+$')]
    [string]$Version
)

$ErrorActionPreference = 'Stop'

$distributionDirectory = $PSScriptRoot
$repositoryRoot = Split-Path -Parent $distributionDirectory
$binaryDirectory = Join-Path $repositoryRoot 'Binary\Release'
$artifactDirectory = Join-Path $repositoryRoot 'Artifacts'
$packageName = "MMORPG-Game-Server-v$Version-win64"
$packageDirectory = Join-Path $artifactDirectory $packageName
$zipPath = Join-Path $artifactDirectory "$packageName.zip"

$requiredFiles = @(
    (Join-Path $binaryDirectory 'Server.exe'),
    (Join-Path $binaryDirectory 'Client.exe'),
    (Join-Path $binaryDirectory 'StressTest.exe'),
    (Join-Path $repositoryRoot 'Resource\map.bin'),
    (Join-Path $repositoryRoot 'Database\original_schema.sql'),
    (Join-Path $repositoryRoot 'Database\stress_test_seed.sql'),
    (Join-Path $repositoryRoot 'Database\README.md'),
    (Join-Path $repositoryRoot 'Client\Resource\Font\neodgm.ttf'),
    (Join-Path $repositoryRoot 'Client\Resource\Texture\TextureSet.json')
)

foreach ($requiredFile in $requiredFiles) {
    if (-not (Test-Path -LiteralPath $requiredFile -PathType Leaf)) {
        throw "Required release file was not found: $requiredFile"
    }
}

if (-not (Test-Path -LiteralPath $artifactDirectory)) {
    New-Item -ItemType Directory -Path $artifactDirectory | Out-Null
}

if (Test-Path -LiteralPath $packageDirectory) {
    $resolvedPackage = [System.IO.Path]::GetFullPath($packageDirectory)
    $resolvedArtifacts = [System.IO.Path]::GetFullPath($artifactDirectory) + [System.IO.Path]::DirectorySeparatorChar
    if (-not $resolvedPackage.StartsWith($resolvedArtifacts, [System.StringComparison]::OrdinalIgnoreCase)) {
        throw "Refusing to replace a directory outside Artifacts: $resolvedPackage"
    }
    Remove-Item -LiteralPath $packageDirectory -Recurse -Force
}

if (Test-Path -LiteralPath $zipPath) {
    Remove-Item -LiteralPath $zipPath -Force
}

$resourceDestination = Join-Path $packageDirectory 'Resource'
$databaseDestination = Join-Path $packageDirectory 'Database'
New-Item -ItemType Directory -Path $resourceDestination -Force | Out-Null
New-Item -ItemType Directory -Path $databaseDestination -Force | Out-Null

Copy-Item -LiteralPath (Join-Path $binaryDirectory 'Server.exe') -Destination $packageDirectory
Copy-Item -LiteralPath (Join-Path $binaryDirectory 'Client.exe') -Destination $packageDirectory
Copy-Item -LiteralPath (Join-Path $binaryDirectory 'StressTest.exe') -Destination $packageDirectory
Copy-Item -LiteralPath (Join-Path $distributionDirectory 'server.ini') -Destination $packageDirectory
Copy-Item -LiteralPath (Join-Path $distributionDirectory 'client.ini') -Destination $packageDirectory
Copy-Item -LiteralPath (Join-Path $distributionDirectory 'stresstest.ini') -Destination $packageDirectory
Copy-Item -LiteralPath (Join-Path $repositoryRoot 'README.md') -Destination $packageDirectory

Copy-Item -Path (Join-Path $repositoryRoot 'Client\Resource\*') -Destination $resourceDestination -Recurse
Copy-Item -LiteralPath (Join-Path $repositoryRoot 'Resource\map.bin') -Destination $resourceDestination

Copy-Item -LiteralPath (Join-Path $repositoryRoot 'Database\original_schema.sql') -Destination $databaseDestination
Copy-Item -LiteralPath (Join-Path $repositoryRoot 'Database\stress_test_seed.sql') -Destination $databaseDestination
Copy-Item -LiteralPath (Join-Path $repositoryRoot 'Database\README.md') -Destination $databaseDestination

Compress-Archive -LiteralPath $packageDirectory -DestinationPath $zipPath -CompressionLevel Optimal
$hash = Get-FileHash -LiteralPath $zipPath -Algorithm SHA256
$checksumPath = "$zipPath.sha256"
Set-Content -LiteralPath $checksumPath -Encoding ascii -Value "$($hash.Hash.ToLowerInvariant())  $([System.IO.Path]::GetFileName($zipPath))"

Write-Output "Release package: $zipPath"
Write-Output "SHA-256: $($hash.Hash.ToLowerInvariant())"

