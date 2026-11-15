# Premake / Visual Studio build from the command line (or the VS Code "Premake: ..." tasks).
#
#   powershell -ExecutionPolicy Bypass -File tools/build_premake.ps1                  # Debug build
#   powershell -ExecutionPolicy Bypass -File tools/build_premake.ps1 -Config Release
#   powershell -ExecutionPolicy Bypass -File tools/build_premake.ps1 -Test            # build + unit tests + smoke test
#
# Needs premake/premake5.exe and SDL2/ (tools/fetch_deps.ps1 -Premake downloads both).
param(
    [ValidateSet('Debug', 'Release')] [string] $Config = 'Debug',
    [switch] $Test
)
$ErrorActionPreference = 'Stop'
$root = Split-Path -Parent $PSScriptRoot
Push-Location $root
try {
    $premake = Join-Path $root 'premake/premake5.exe'
    if (-not (Test-Path $premake)) {
        throw 'premake/premake5.exe missing - run: powershell -ExecutionPolicy Bypass -File tools/fetch_deps.ps1 -Premake'
    }
    & $premake vs2022
    if ($LASTEXITCODE -ne 0) { throw 'premake failed' }

    $vswhere = Join-Path ${env:ProgramFiles(x86)} 'Microsoft Visual Studio/Installer/vswhere.exe'
    if (-not (Test-Path $vswhere)) { throw 'Visual Studio 2022 not found (vswhere.exe missing)' }
    $msbuild = & $vswhere -latest -products * -requires Microsoft.Component.MSBuild -find 'MSBuild\**\Bin\MSBuild.exe' | Select-Object -First 1
    if (-not $msbuild) { throw 'MSBuild not found' }

    & $msbuild Giera.sln -m -nologo -verbosity:minimal "-p:Configuration=$Config" -p:Platform=x64
    if ($LASTEXITCODE -ne 0) { throw 'build failed' }

    $bin = Join-Path $root "bin/$Config-windows-x86_64"
    Write-Host "Built into $bin"

    if ($Test) {
        & (Join-Path $bin 'GieraTests.exe')
        if ($LASTEXITCODE -ne 0) { throw 'unit tests failed' }

        # Headless end-to-end run: 15 s of game time with the player fighting by itself.
        $env:SDL_VIDEODRIVER = 'dummy'
        $out = & (Join-Path $bin 'Giera.exe') --smoke-test 15 --autoplay 2>&1 | Out-String
        Remove-Item Env:SDL_VIDEODRIVER
        Write-Host $out.Trim()
        if ($LASTEXITCODE -ne 0 -or $out -notmatch 'killed [1-9]') { throw 'game smoke test failed' }
        Write-Host 'All tests passed.'
    }
}
finally {
    Pop-Location
}
