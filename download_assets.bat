@echo off
echo "Creating directories"
mkdir SDL2
echo "Downloading SDL files and unzipping files"
powershell -Command "Invoke-WebRequest https://github.com/libsdl-org/SDL/releases/download/release-2.30.1/SDL2-devel-2.30.1-VC.zip -Outfile SDL2/sdl.zip"
powershell -Command "Expand-Archive -Path SDL2\sdl.zip -DestinationPath SDL2"
del SDL2\sdl.zip
echo "Done"

echo "Downloading SDL_ttf files and unzipping files"
powershell -Command "Invoke-WebRequest https://github.com/libsdl-org/SDL_ttf/releases/download/release-2.22.0/SDL2_ttf-devel-2.22.0-VC.zip -Outfile SDL2/sdl_ttf.zip"
powershell -Command "Expand-Archive -Path SDL2\sdl_ttf.zip -DestinationPath SDL2"
del SDL2\sdl_ttf.zip
echo "Done"

echo "Downloading SDL_image files and unzipping files"
powershell -Command "Invoke-WebRequest https://github.com/libsdl-org/SDL_image/releases/download/release-2.8.2/SDL2_image-devel-2.8.2-VC.zip -Outfile SDL2/sdl_image.zip"
powershell -Command "Expand-Archive -Path SDL2\sdl_image.zip -DestinationPath SDL2"
del SDL2\sdl_image.zip
echo "Done"

echo "Downloading premake"
powershell -Command "Invoke-WebRequest https://github.com/premake/premake-core/releases/download/v5.0.0-beta2/premake-5.0.0-beta2-windows.zip -Outfile premake/premake.zip"
powershell -Command "Expand-Archive -Path premake\premake.zip -DestinationPath premake"
del premake\premake.zip
echo "Done"

Pause