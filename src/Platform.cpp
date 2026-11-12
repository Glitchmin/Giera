#include "Platform.h"
#include <SDL.h>
#include <filesystem>

namespace {
bool touchControls = Platform::isAndroid;
}

void Platform::initWorkingDirectory()
{
    if constexpr (!isAndroid) {
        char* base = SDL_GetBasePath();
        if (base) {
            std::error_code ec;
            std::filesystem::current_path(std::filesystem::u8path(base), ec);
            SDL_free(base);
        }
    }
}

std::string Platform::writableDir()
{
    static std::string dir = [] {
        char* pref = SDL_GetPrefPath("Glitchmin", "Giera");
        if (!pref) {
            return std::string("./");
        }
        std::string result(pref);
        SDL_free(pref);
        return result;
    }();
    return dir;
}

bool Platform::touchControlsEnabled()
{
    return touchControls;
}

void Platform::setTouchControls(bool enabled)
{
    touchControls = enabled;
}
