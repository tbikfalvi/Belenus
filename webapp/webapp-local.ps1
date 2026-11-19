[CmdletBinding()]
param(
    [ValidateSet('Deploy', 'Run', 'CheckQmysql')]
    [string]$Action = 'Run',
    [string]$QmysqlPlugin,
    [string]$QmysqlClientDll
)

$ErrorActionPreference = 'Stop'
$buildDir = 'C:\DevBuild\WebApp'
$runtimeDir = 'C:\KiwiSun\WebApp'
$qtRoot = 'C:\DevEnv\Qt6\6.12.0\mingw_64'
$compilerBin = 'C:\DevEnv\Qt6\Tools\mingw1510_64\bin'
$qmakeBuildDir = Join-Path $buildDir 'qmake'
$projectFile = Join-Path $PSScriptRoot 'webapp.pro'
$buildExe = Join-Path $buildDir 'webapp.exe'
$runtimeExe = Join-Path $runtimeDir 'webapp.exe'
$originalPath = $env:Path

try {
    if ($QmysqlPlugin -or $QmysqlClientDll) {
        if ($Action -ne 'Deploy' -or !$QmysqlPlugin -or !$QmysqlClientDll) {
            throw 'Supply both -QmysqlPlugin and -QmysqlClientDll with Deploy.'
        }
        $plugin = Get-Item -LiteralPath $QmysqlPlugin
        $client = Get-Item -LiteralPath $QmysqlClientDll
        if ($plugin.PSIsContainer -or $plugin.Name -ne 'qsqlmysql.dll' -or
            $client.PSIsContainer -or $client.Extension -ne '.dll' -or $client.Name -eq $plugin.Name) {
            throw 'Specify qsqlmysql.dll and its matching client DLL as separate files.'
        }
    }

    $env:Path = "$qtRoot\bin;$compilerBin;$originalPath"
    New-Item -ItemType Directory -Path $qmakeBuildDir -Force | Out-Null
    Push-Location -LiteralPath $qmakeBuildDir
    try {
        & "$qtRoot\bin\qmake.exe" $projectFile -spec win32-g++ "DESTDIR=$($buildDir.Replace('\', '/'))"
        if ($LASTEXITCODE -ne 0) {
            throw "qmake Release configuration failed (exit $LASTEXITCODE)."
        }
        $jobs = [Math]::Min([Environment]::ProcessorCount, 8)
        & "$compilerBin\mingw32-make.exe" "-j$jobs"
        if ($LASTEXITCODE -ne 0) {
            throw "qmake Release build failed (exit $LASTEXITCODE)."
        }
    } finally {
        Pop-Location
    }

    if ($Action -eq 'Deploy') {
        New-Item -ItemType Directory -Path $runtimeDir -Force | Out-Null
    } elseif (!(Test-Path -LiteralPath (Join-Path $runtimeDir 'platforms\qwindows.dll') -PathType Leaf)) {
        throw "Qt runtime not found in $runtimeDir. Run with -Action Deploy first."
    }

    Copy-Item -LiteralPath $buildExe -Destination $runtimeExe -Force
    if ((Get-FileHash -LiteralPath $buildExe).Hash -ne (Get-FileHash -LiteralPath $runtimeExe).Hash) {
        throw "The deployed EXE differs from the Release build: $runtimeExe"
    }

    if ($Action -eq 'Deploy') {
        & "$qtRoot\bin\windeployqt.exe" --release --compiler-runtime --no-translations $runtimeExe
        if ($LASTEXITCODE -ne 0) {
            throw "windeployqt failed (exit $LASTEXITCODE)."
        }
        if ($QmysqlPlugin) {
            $sqlDrivers = Join-Path $runtimeDir 'sqldrivers'
            New-Item -ItemType Directory -Path $sqlDrivers -Force | Out-Null
            Copy-Item -LiteralPath $plugin.FullName -Destination $sqlDrivers -Force
            Copy-Item -LiteralPath $client.FullName -Destination $runtimeDir -Force
        }
        Write-Output "Deployed $runtimeExe"
        return
    }

    # Load the deployed runtime with the development kit removed from PATH.
    $env:Path = "$env:SystemRoot\System32;$env:SystemRoot"
    if ($Action -eq 'Run') {
        Start-Process -FilePath $runtimeExe -WorkingDirectory $runtimeDir
        return
    }

    $argument = '--check-qmysql'
    $info = [System.Diagnostics.ProcessStartInfo]::new($runtimeExe, $argument)
    $info.WorkingDirectory = $runtimeDir
    $info.UseShellExecute = $false
    $info.RedirectStandardOutput = $true
    $info.RedirectStandardError = $true
    $process = [System.Diagnostics.Process]::Start($info)
    try {
        $output = $process.StandardOutput.ReadToEndAsync()
        $errors = $process.StandardError.ReadToEndAsync()
        $process.WaitForExit()
        if ($output.Result) { Write-Output $output.Result.TrimEnd() }
        if ($errors.Result) { Write-Output $errors.Result.TrimEnd() }
        if ($process.ExitCode -ne 0) {
            throw "$argument failed (exit $($process.ExitCode)) in $runtimeDir."
        }
    } finally {
        $process.Dispose()
    }
} catch {
    throw "WebApp ${Action}: $($_.Exception.Message)"
} finally {
    $env:Path = $originalPath
}
