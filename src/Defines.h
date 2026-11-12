#pragma once
// ANDROID_BUILD is derived from the compiler (the NDK always defines __ANDROID__),
// so the same sources build for Windows and Android without hand-editing.
#if defined(__ANDROID__)
#define ANDROID_BUILD 1
#else
#define ANDROID_BUILD 0
#endif

// Relative to the working directory on desktop (see Platform::initWorkingDirectory)
// and to the APK root on Android (Gradle packs the repo's assets/ folder as "assets/").
#define SAVE_FILES_PATH "assets"
