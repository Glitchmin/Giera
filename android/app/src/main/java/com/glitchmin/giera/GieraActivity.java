package com.glitchmin.giera;

import org.libsdl.app.SDLActivity;

/**
 * Android entry point. SDLActivity loads the native libraries and calls SDL_main()
 * (our main() in src/main.cpp) inside libmain.so.
 */
public class GieraActivity extends SDLActivity {
    @Override
    protected String[] getLibraries() {
        // SDL2_image and SDL2_ttf are linked statically into libmain.so.
        // libc++_shared is listed explicitly for old Android versions whose
        // linker does not resolve app-private dependencies on its own.
        return new String[] {
            "c++_shared",
            "SDL2",
            "main"
        };
    }
}
