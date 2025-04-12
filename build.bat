@echo off
setlocal enabledelayedexpansion

set CXX=cl
set CXXFLAGS=/Zi /std:c++17 /W4 /Od /MDd
set LDFLAGS=opengl32.lib user32.lib gdi32.lib shell32.lib assimp-vc143-mt.lib zlib.lib /DEBUG
set BUILD_DIR=build
set EXECUTABLE=game.exe
set VENDOR_DIR=../vendor

:: Create build directory if it doesn't exist
if not exist "%BUILD_DIR%" mkdir "%BUILD_DIR%"

:: Include paths
set INCLUDE_FLAGS=/I"%VENDOR_DIR%" /I"%VENDOR_DIR%\imgui" /I"../src"

:: GLFW library (assuming pre-built Windows version)
set GLFW_LIB=%VENDOR_DIR%\GLFW\lib-vc2022\glfw3.lib

:: Source files
set SOURCES=
for /r src %%f in (*.cpp) do set SOURCES=!SOURCES! "%%f"

:: ImGui sources
set IMGUI_SOURCES=
for %%f in (
    "%VENDOR_DIR%\imgui\imgui.cpp"
    "%VENDOR_DIR%\imgui\imgui_draw.cpp"
    "%VENDOR_DIR%\imgui\imgui_tables.cpp"
    "%VENDOR_DIR%\imgui\imgui_widgets.cpp"
    "%VENDOR_DIR%\imgui\backends\imgui_impl_glfw.cpp"
    "%VENDOR_DIR%\imgui\backends\imgui_impl_opengl3.cpp"
    "%VENDOR_DIR%\imgui\misc\cpp\imgui_stdlib.cpp"
) do set IMGUI_SOURCES=!IMGUI_SOURCES! "%%f"

:: GLAD source
set GLAD_SRC=%VENDOR_DIR%\glad\src\glad.c

set LIB_PATH=/LIBPATH:"%VENDOR_DIR%\lib"

echo Compiling...
pushd %BUILD_DIR%
echo "%CXX% %CXXFLAGS% %INCLUDE_FLAGS% %SOURCES% %IMGUI_SOURCES% %GLAD_SRC% %GLFW_LIB% /Fe:../%BUILD_DIR%\%EXECUTABLE% /link %LDFLAGS%"

%CXX% %CXXFLAGS% %INCLUDE_FLAGS% %SOURCES% %IMGUI_SOURCES% %GLAD_SRC% %GLFW_LIB% /Fe:../%BUILD_DIR%\%EXECUTABLE% /link %LIB_PATH% %LDFLAGS%
popd

if %errorlevel% equ 0 (
    echo Build successful! Run with %BUILD_DIR%\%EXECUTABLE%
) else (
    echo Build failed.
)

endlocal