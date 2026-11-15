# Giera

2.5D RPG game project, developed with C++20 and SDL2. Runs on **Windows** and **Android** from one source tree.

Gameplay will be similar to that from previous project:
https://user-images.githubusercontent.com/73713848/164265547-8efe5550-ccea-4964-afb9-02d0eaa053fd.mp4

This project is an opportunity to learn Modern C++ and develop a game with interesting mechanics.

## Layout

```
CMakeLists.txt        one build for Windows and Android
CMakePresets.json     windows-debug / windows-release
premake5.lua          Visual Studio 2022 solution (Windows), same sources and tests
generateProjects.bat  runs premake -> Giera.sln
.vscode/              build, debug, test, premake and Android tasks for VS Code
src/                  game code (giera_core library + main.cpp)
assets/               textures, CSV data, fonts (packed into the APK as "assets/...")
tests/                unit tests + headless game-loop tests (CTest)
android/              Gradle project; builds libmain.so from the root CMakeLists.txt
external/             SDL2 2.30.9, SDL2_image 2.8.2, SDL2_ttf 2.22.0 sources (not committed)
tools/fetch_deps.ps1  downloads external/
art/                  source art (.psd), not shipped
docs/                 Doxyfile, class diagram
```

## Requirements (Windows)

- Visual Studio 2022 with the "Desktop development with C++" workload (MSVC + bundled CMake/Ninja)
- VS Code with the **C/C++** and **CMake Tools** extensions
- For Android: Android Studio (SDK, NDK 28.0.12433566, CMake 3.30.5 from the SDK Manager)

## First time

```powershell
powershell -ExecutionPolicy Bypass -File tools/fetch_deps.ps1   # SDL sources into external/
```

## Windows - VS Code

1. Open the folder in VS Code, pick the **Windows Debug** preset in the CMake Tools status bar.
2. **F5** with "Giera (Windows)" builds and starts the game under the debugger.
   - "Giera (Windows, Android touch controls)" shows the on-screen joysticks/buttons.
   - "Giera (Windows, autoplay demo)" lets the player fight by itself.
3. Tests: Testing panel, or `Ctrl+Shift+P` → *CMake: Run Tests*.

Command line equivalent (from a "Developer PowerShell for VS 2022"):

```powershell
cmake --preset windows-debug
cmake --build --preset windows-debug
ctest --preset windows-debug
build/windows-debug/bin/Giera.exe
```

Controls: `WASD` move, left click melee attack towards the cursor (bow: hold to draw, release to shoot),
`Q` switch melee/bow, `Left Ctrl` block (hold to raise the shield; without a shield press just before a hit to parry),
`Space` dash, `1`-`4` eat food from the satchel, `I` inventory, `Esc` close, hold right mouse to look around.

Fight system rules and how to tune them: [docs/FightSystem.md](docs/FightSystem.md)
(numbers in `assets/rules/Combat.txt` and `assets/baseItems/*.csv`).

Command line options: `--touch-ui`, `--autoplay`, `--verbose`, `--smoke-test <seconds>`.

## Windows - Visual Studio (premake)

The premake build uses the same `src/`, `tests/` and `assets/`, but links the prebuilt SDL2 "VC"
packages instead of building SDL from source.

```powershell
powershell -ExecutionPolicy Bypass -File tools/fetch_deps.ps1 -Premake   # premake5.exe + SDL2/
generateProjects.bat                                                     # -> Giera.sln
```

Open `Giera.sln`. Projects:

- **GieraCore**: static library with all game code except `main.cpp`
- **Giera**: the game (start project)
- **GieraTests**: unit tests + headless game-loop tests (`GieraTests.exe --list`, `--file TestCombat`, a name filter)

Output goes to `bin/<Config>-windows-x86_64/` (SDL DLLs and `assets/` are copied there after each build).
Without opening Visual Studio: `tools/build_premake.ps1 [-Config Release] [-Test]`, also available as the
VS Code tasks *Premake: build Debug and run tests* and *Premake: build Release*.

## Android - VS Code tasks

`Ctrl+Shift+P` → *Tasks: Run Task*:

- **Android: build debug APK** → `android/app/build/outputs/apk/debug/app-debug.apk`
- **Android: install and run** (device with USB debugging, or a running emulator)
- **Android: logcat (game log)**

The tasks use the JDK bundled with Android Studio (Gradle 8.9 does not run on Java 23).
For native debugging on Android open the `android/` folder in Android Studio.

## UML diagrams

https://www.edrawmax.com/online/share.html?code=92e62840bceb11eca95c0a54be41f961
https://www.edrawmax.com/online/share.html?code=e9791434ecc411ecbb9f0a54be41f961
https://www.edrawmax.com/online/share.html?code=2198bffeecc511ecb3310a951ba8b83d
https://www.edrawmax.com/online/share.html?code=2e99ebe8546611ed99230a951ba8b83d
https://www.edrawmax.com/online/share.html?code=3de2538c546711ed855d0a951ba8b83d
