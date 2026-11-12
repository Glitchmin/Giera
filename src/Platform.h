#pragma once
#include <string>

/**
 * @brief Small platform abstraction so the same sources build on Windows and Android.
 *
 * Asset paths are always relative ("assets/tx/..."):
 *  - on desktop the working directory is switched to the executable's folder
 *    (the build copies assets/ next to the executable),
 *  - on Android the APK contains the same "assets/..." tree and SDL_RWFromFile
 *    reads it straight from the APK.
 */
namespace Platform {

#if defined(__ANDROID__)
inline constexpr bool isAndroid = true;
#else
inline constexpr bool isAndroid = false;
#endif

/// Desktop: chdir to the executable's directory so relative asset paths work
/// no matter where the game was launched from. No-op on Android.
void initWorkingDirectory();

/// Folder where the game may write (logs, saves). Always ends with a separator.
/// Uses SDL_GetPrefPath; falls back to the working directory.
std::string writableDir();

/// Whether the on-screen touch controls (joysticks, buttons) should be shown.
/// Always true on Android; on desktop only when forced via setTouchControls().
bool touchControlsEnabled();
void setTouchControls(bool enabled);

}
