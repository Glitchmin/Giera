cmake_minimum_required(VERSION 3.22)

# CMake 4.x refuses sub-projects that declare cmake_minimum_required < 3.5
# (SDL_ttf's vendored freetype does). This keeps them building on new CMake.
if(NOT DEFINED CMAKE_POLICY_VERSION_MINIMUM)
    set(CMAKE_POLICY_VERSION_MINIMUM 3.5 CACHE STRING "" FORCE)
endif()

project(Giera VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

option(GIERA_BUILD_TESTS "Build the unit tests" ON)

# Everything that runs (game, tests, SDL2.dll) lands in one folder so the
# DLL and the copied assets are found without any PATH tricks.
if(NOT ANDROID)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
endif()

# ---------------------------------------------------------------------------
# Third-party: SDL2 (shared - Android's SDLActivity loads libSDL2.so),
# SDL2_image + SDL2_ttf (static, linked into the game).
# Sources live in external/ (see tools/fetch_deps.ps1).
# ---------------------------------------------------------------------------
foreach(dep SDL SDL_image SDL_ttf)
    if(NOT EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/external/${dep}/CMakeLists.txt)
        message(FATAL_ERROR "external/${dep} is missing. Run tools/fetch_deps.ps1 first.")
    endif()
endforeach()

set(SDL_SHARED ON  CACHE BOOL "" FORCE)
set(SDL_STATIC OFF CACHE BOOL "" FORCE)
set(SDL_TEST   OFF CACHE BOOL "" FORCE)
set(SDL_TESTS  OFF CACHE BOOL "" FORCE)
add_subdirectory(external/SDL EXCLUDE_FROM_ALL)
# SDL_image/SDL_ttf built as static libs look for "SDL2::SDL2-static";
# point that name at the shared SDL2 so everything uses one SDL instance.
if(NOT TARGET SDL2::SDL2-static)
    add_library(SDL2::SDL2-static ALIAS SDL2)
endif()

set(BUILD_SHARED_LIBS OFF)
set(SDL2IMAGE_INSTALL OFF CACHE BOOL "" FORCE)
set(SDL2IMAGE_SAMPLES OFF CACHE BOOL "" FORCE)
set(SDL2IMAGE_VENDORED OFF CACHE BOOL "" FORCE)     # PNG/JPG come from built-in stb_image
set(SDL2IMAGE_BACKEND_STB ON CACHE BOOL "" FORCE)
foreach(fmt AVIF JXL TIF WEBP)
    set(SDL2IMAGE_${fmt} OFF CACHE BOOL "" FORCE)
endforeach()
add_subdirectory(external/SDL_image EXCLUDE_FROM_ALL)

set(SDL2TTF_INSTALL OFF CACHE BOOL "" FORCE)
set(SDL2TTF_SAMPLES OFF CACHE BOOL "" FORCE)
set(SDL2TTF_VENDORED ON CACHE BOOL "" FORCE)        # bundled freetype
set(SDL2TTF_HARFBUZZ OFF CACHE BOOL "" FORCE)
add_subdirectory(external/SDL_ttf EXCLUDE_FROM_ALL)

# ---------------------------------------------------------------------------
# Game code: everything except main.cpp goes into a static library that both
# the game and the unit tests link against.
# ---------------------------------------------------------------------------
file(GLOB GIERA_SOURCES CONFIGURE_DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp ${CMAKE_CURRENT_SOURCE_DIR}/src/*.h)
list(REMOVE_ITEM GIERA_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp)

add_library(giera_core STATIC ${GIERA_SOURCES})
target_include_directories(giera_core PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)
target_link_libraries(giera_core PUBLIC SDL2::SDL2 SDL2_image::SDL2_image-static SDL2_ttf::SDL2_ttf-static)
if(MSVC)
    target_compile_options(giera_core PUBLIC /utf-8 /MP /permissive- /Zc:__cplusplus)
    target_compile_definitions(giera_core PUBLIC _CRT_SECURE_NO_WARNINGS NOMINMAX)
endif()
if(ANDROID)
    target_link_libraries(giera_core PUBLIC log android)
endif()

set(GIERA_ASSETS_DIR ${CMAKE_CURRENT_SOURCE_DIR}/assets)

if(ANDROID)
    # SDLActivity loads "libmain.so" and calls SDL_main() in it.
    add_library(main SHARED src/main.cpp)
    target_link_libraries(main PRIVATE giera_core)
else()
    add_executable(Giera src/main.cpp)
    target_link_libraries(Giera PRIVATE giera_core SDL2::SDL2main)
    set_target_properties(Giera PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY $<TARGET_FILE_DIR:Giera>)

    # Copy assets next to the executable (the game chdirs to its own folder).
    add_custom_target(giera_assets
        COMMAND ${CMAKE_COMMAND} -E copy_directory ${GIERA_ASSETS_DIR} ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/assets
        COMMENT "Copying assets")
    add_dependencies(Giera giera_assets)

    if(GIERA_BUILD_TESTS)
        enable_testing()
        add_subdirectory(tests)
    endif()
endif()
