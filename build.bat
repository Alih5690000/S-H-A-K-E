@echo off
:: Stop execution if any command fails
setlocal enabledelayedexpansion

:: --- Configuration ---
set "OUTPUT_DIR=dist"
set "OUTPUT_NAME=index.html"
set "SRC_FILES=main.cpp"
set "ASSETS_DIR=assets"

echo Building SDL2 project with Emscripten...

:: Create output directory if it doesn't exist
if not exist "%OUTPUT_DIR%" (
    mkdir "%OUTPUT_DIR%"
)

:: --- Compilation and Linking ---
:: -sUSE_SDL=2 tells Emscripten to download and link the SDL2 port.
:: -sUSE_SDL_IMAGE=2 tells Emscripten to download and link the SDL_image port.
:: Escape inner quotes for Windows cmd using \"
emcc %SRC_FILES% -o "%OUTPUT_DIR%\%OUTPUT_NAME%" -s USE_SDL=2 -s USE_SDL_IMAGE=2 -s SDL2_IMAGE_FORMATS="[\"png\",\"jpg\"]" -s MAX_WEBGL_VERSION=2 --preload-file "%ASSETS_DIR%@/assets" -O3


:: Check if compilation was successful
if %ERRORLEVEL% neq 0 (
    echo.
    echo Build failed with error code %ERRORLEVEL%
    exit /b %ERRORLEVEL%
)

echo.
echo Build complete! Files generated in '%OUTPUT_DIR%/'
endlocal
