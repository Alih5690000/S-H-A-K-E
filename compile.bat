@echo off
:: Stop execution if any command fails
setlocal enabledelayedexpansion

:: --- Configuration ---
set "OUTPUT_DIR=dist"
set "OUTPUT_NAME=index.html"
set "SRC_FILES=main.cpp"
set "ASSETS_DIR=assets"

echo Building SDL2 project with ALL ports (Image, Mixer, TTF) using Emscripten...

:: Create output directory if it doesn't exist
if not exist "%OUTPUT_DIR%" (
    mkdir "%OUTPUT_DIR%"
)

:: --- Compilation and Linking (All Major Ports Combined) ---
:: -s USE_SDL=2             -> Compiles & links core SDL2.
:: -s USE_SDL_IMAGE=2       -> Links SDL_image with PNG and JPG format decoders.
:: -s USE_SDL_MIXER=2       -> Links SDL_mixer with MP3 and OGG Vorbis audio support.
:: -s USE_SDL_TTF=2         -> Links SDL_ttf for TrueType Font rendering (FreeType port).
:: -s MAX_WEBGL_VERSION=2   -> Forces WebGL 2.0 context compatibility.

emcc %SRC_FILES% -o "%OUTPUT_DIR%\%OUTPUT_NAME%" -s USE_SDL=2 -s USE_SDL_IMAGE=2 -s SDL2_IMAGE_FORMATS="[\"png\",\"jpg\"]" -s USE_SDL_MIXER=2 -s SDL2_MIXER_FORMATS="[\"mp3\",\"ogg\"]" -s USE_SDL_TTF=2 -s MAX_WEBGL_VERSION=2 --preload-file "%ASSETS_DIR%@/assets" -O3

:: Check if compilation was successful
if %ERRORLEVEL% neq 0 (
    echo.
    echo Build failed with error code %ERRORLEVEL%
    exit /b %ERRORLEVEL%
)

echo.
echo Build complete! Files generated in '%OUTPUT_DIR%/'
endlocal
