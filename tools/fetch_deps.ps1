# Downloads third-party dependencies (none of them are committed).
#
#   powershell -ExecutionPolicy Bypass -File tools/fetch_deps.ps1            # CMake / VS Code / Android
#   powershell -ExecutionPolicy Bypass -File tools/fetch_deps.ps1 -Premake   # also the Visual Studio (premake) build
#
# Default: SDL sources into external/ (about 170 MB), built from source by CMake for Windows and Android.
# -Premake: additionally premake5.exe into premake/ and the prebuilt SDL "VC" packages into SDL2/.
param(
    [switch] $Premake
)
$ErrorActionPreference = 'Stop'
$ProgressPreference = 'SilentlyContinue'   # Invoke-WebRequest is very slow with the progress bar
$root = Split-Path -Parent $PSScriptRoot

$sdlVersion      = '2.30.9'
$sdlImageVersion = '2.8.2'
$sdlTtfVersion   = '2.22.0'
$premakeVersion  = '5.0.0-beta2'

# ---- SDL sources for CMake (external/) ----
$external = Join-Path $root 'external'
New-Item -ItemType Directory -Force $external | Out-Null

$deps = @(
    @{ Name = 'SDL';       Url = 'https://github.com/libsdl-org/SDL.git';       Tag = "release-$sdlVersion" },
    @{ Name = 'SDL_image'; Url = 'https://github.com/libsdl-org/SDL_image.git'; Tag = "release-$sdlImageVersion" },
    @{ Name = 'SDL_ttf';   Url = 'https://github.com/libsdl-org/SDL_ttf.git';   Tag = "release-$sdlTtfVersion" }
)

foreach ($dep in $deps) {
    $dir = Join-Path $external $dep.Name
    if (Test-Path (Join-Path $dir 'CMakeLists.txt')) {
        Write-Host "$($dep.Name): already present, skipping"
        continue
    }
    Write-Host "$($dep.Name): cloning $($dep.Tag)"
    git clone --depth 1 --branch $dep.Tag $dep.Url $dir
    if ($LASTEXITCODE -ne 0) { throw "git clone failed for $($dep.Name)" }
}

# SDL_ttf builds its bundled freetype (external/freetype) - fetch that submodule.
$ttf = Join-Path $external 'SDL_ttf'
if (-not (Test-Path (Join-Path $ttf 'external/freetype/CMakeLists.txt'))) {
    Write-Host 'SDL_ttf: fetching vendored freetype'
    git -C $ttf submodule update --init --depth 1 external/freetype
    if ($LASTEXITCODE -ne 0) { throw 'freetype submodule update failed' }
}

# ---- premake + prebuilt SDL for the Visual Studio solution ----
function Get-Zip([string] $url, [string] $destination) {
    $zip = Join-Path ([System.IO.Path]::GetTempPath()) ([System.IO.Path]::GetFileName($url))
    Write-Host "downloading $url"
    Invoke-WebRequest -Uri $url -OutFile $zip -UseBasicParsing
    Expand-Archive -Path $zip -DestinationPath $destination -Force
    Remove-Item $zip
}

if ($Premake) {
    $premakeDir = Join-Path $root 'premake'
    if (Test-Path (Join-Path $premakeDir 'premake5.exe')) {
        Write-Host 'premake: already present, skipping'
    } else {
        Get-Zip "https://github.com/premake/premake-core/releases/download/v$premakeVersion/premake-$premakeVersion-windows.zip" $premakeDir
    }

    $sdl2 = Join-Path $root 'SDL2'
    New-Item -ItemType Directory -Force $sdl2 | Out-Null
    $packages = @(
        @{ Dir = "SDL2-$sdlVersion";            Url = "https://github.com/libsdl-org/SDL/releases/download/release-$sdlVersion/SDL2-devel-$sdlVersion-VC.zip" },
        @{ Dir = "SDL2_image-$sdlImageVersion"; Url = "https://github.com/libsdl-org/SDL_image/releases/download/release-$sdlImageVersion/SDL2_image-devel-$sdlImageVersion-VC.zip" },
        @{ Dir = "SDL2_ttf-$sdlTtfVersion";     Url = "https://github.com/libsdl-org/SDL_ttf/releases/download/release-$sdlTtfVersion/SDL2_ttf-devel-$sdlTtfVersion-VC.zip" }
    )
    foreach ($pkg in $packages) {
        if (Test-Path (Join-Path $sdl2 "$($pkg.Dir)/lib/x64")) {
            Write-Host "$($pkg.Dir): already present, skipping"
            continue
        }
        Get-Zip $pkg.Url $sdl2
    }
    Write-Host 'Premake build: run generateProjects.bat (or tools/build_premake.ps1) and open Giera.sln'
}

Write-Host 'Done. CMake build: cmake --preset windows-debug'
