param(
    [string]$ServerInstance = '.\SQLEXPRESS',
    [string]$DatabaseName = 'DB2020180021',
    [string]$OutputDirectory = $PSScriptRoot,
    [switch]$IncludeStressTestData
)

$ErrorActionPreference = 'Stop'

Import-Module SQLPS -DisableNameChecking

if (-not (Test-Path -LiteralPath $OutputDirectory)) {
    New-Item -ItemType Directory -Path $OutputDirectory | Out-Null
}

$server = New-Object Microsoft.SqlServer.Management.Smo.Server $ServerInstance
$server.ConnectionContext.LoginSecure = $true
$database = $server.Databases[$DatabaseName]

if ($null -eq $database) {
    throw "Database '$DatabaseName' was not found on '$ServerInstance'."
}

$schemaPath = Join-Path $OutputDirectory 'original_schema.sql'
$utf8WithoutBom = New-Object System.Text.UTF8Encoding $false

$scripter = New-Object Microsoft.SqlServer.Management.Smo.Scripter $server
$scripter.Options.AllowSystemObjects = $false
$scripter.Options.AnsiFile = $false
$scripter.Options.DriAll = $true
$scripter.Options.IncludeDatabaseContext = $false
$scripter.Options.IncludeHeaders = $true
$scripter.Options.Indexes = $true
$scripter.Options.SchemaQualify = $true
$scripter.Options.ScriptBatchTerminator = $true
$scripter.Options.ScriptData = $false
$scripter.Options.ScriptDrops = $false
$scripter.Options.ScriptSchema = $true
$scripter.Options.ToFileOnly = $true
$scripter.Options.WithDependencies = $true
$scripter.Options.FileName = $schemaPath
$scripter.Options.Encoding = $utf8WithoutBom

$urns = New-Object 'System.Collections.Generic.List[Microsoft.SqlServer.Management.Sdk.Sfc.Urn]'

foreach ($table in $database.Tables) {
    if (-not $table.IsSystemObject) {
        $urns.Add($table.Urn)
    }
}

foreach ($procedure in $database.StoredProcedures) {
    if (-not $procedure.IsSystemObject) {
        $urns.Add($procedure.Urn)
    }
}

if ($urns.Count -eq 0) {
    throw "No user tables or stored procedures were found in '$DatabaseName'."
}

if (Test-Path -LiteralPath $schemaPath) {
    Remove-Item -LiteralPath $schemaPath
}

$scripter.Script($urns.ToArray())
Write-Output "Schema exported to: $schemaPath"

if ($IncludeStressTestData) {
    $seedPath = Join-Path $OutputDirectory 'stress_test_seed.sql'
    $writer = New-Object System.IO.StreamWriter($seedPath, $false, $utf8WithoutBom)

    try {
        $writer.WriteLine('SET NOCOUNT ON;')
        $writer.WriteLine('SET XACT_ABORT ON;')
        $writer.WriteLine('GO')
        $writer.WriteLine('SET IDENTITY_INSERT [dbo].[Player] ON;')
        $writer.WriteLine('GO')

        $connectionString = 'Server={0};Database={1};Integrated Security=True;TrustServerCertificate=True;' -f $ServerInstance, $DatabaseName
        $connection = New-Object System.Data.SqlClient.SqlConnection $connectionString
        $connection.Open()

        try {
            $command = $connection.CreateCommand()
            $command.CommandText = @'
SELECT UserID, RTRIM(PlayerID), posX, posY, MaxHP, HP, ClassType, UserLevel, UserExp
FROM dbo.Player
WHERE RTRIM(PlayerID) LIKE N'P%'
  AND TRY_CONVERT(INT, SUBSTRING(RTRIM(PlayerID), 2, 20)) IS NOT NULL
ORDER BY UserID;
'@
            $reader = $command.ExecuteReader()

            try {
                while ($reader.Read()) {
                    $escapedName = $reader.GetString(1).Replace("'", "''")
                    $line = 'INSERT [dbo].[Player] ([UserID], [PlayerID], [posX], [posY], [MaxHP], [HP], [ClassType], [UserLevel], [UserExp]) VALUES ({0}, N''{1}'', {2}, {3}, {4}, {5}, {6}, {7}, {8});' -f `
                        $reader.GetInt64(0), $escapedName, $reader.GetInt16(2), $reader.GetInt16(3), `
                        $reader.GetInt32(4), $reader.GetInt32(5), $reader.GetByte(6), `
                        $reader.GetInt32(7), $reader.GetInt64(8)
                    $writer.WriteLine($line)
                }
            }
            finally {
                $reader.Close()
            }
        }
        finally {
            $connection.Close()
        }

        $writer.WriteLine('GO')
        $writer.WriteLine('SET IDENTITY_INSERT [dbo].[Player] OFF;')
        $writer.WriteLine('GO')
    }
    finally {
        $writer.Dispose()
    }

    Write-Output "Sanitized stress-test data exported to: $seedPath"
}
