@echo off
setlocal EnableExtensions

rem ============================================
rem build_vs2022.bat (no block parentheses version)
rem ============================================

set "GENERATOR=Visual Studio 17 2022"
set "ARCH=x64"
set "BUILD_DIR=build-msvc"
set "DEFAULT_CONFIG=Release"
set "TRIPLET=x64-windows"

set "ROOT=%~dp0"
if "%ROOT:~-1%"=="\" set "ROOT=%ROOT:~0,-1%"

set "CONFIG=%~1"
if "%CONFIG%"=="" set "CONFIG=%DEFAULT_CONFIG%"

echo.
echo === PhysicsEngine: Configure for %GENERATOR% %ARCH%, %CONFIG% ===

if "%VCPKG_ROOT%"=="" goto ERR_NOVCPKG
if not exist "%VCPKG_ROOT%\scripts\buildsystems\vcpkg.cmake" goto ERR_TOOLCHAIN

echo.
set "MANIFEST_FILE=%ROOT%\vcpkg.json"
if exist "%MANIFEST_FILE%" goto MANIFEST
goto CLASSIC

:MANIFEST
echo [vcpkg] Manifest detected: "%MANIFEST_FILE%"
echo [vcpkg] Installing manifest dependencies (triplet: %TRIPLET%)...
"%VCPKG_ROOT%\vcpkg.exe" install --triplet=%TRIPLET%
if errorlevel 1 goto ERR_VCPKG
goto MKDIR

:CLASSIC
echo [vcpkg] No manifest found. Installing explicit deps (triplet: %TRIPLET%)...
"%VCPKG_ROOT%\vcpkg.exe" install glfw3 glad glm spdlog entt --triplet=%TRIPLET%
if errorlevel 1 goto ERR_VCPKG
goto MKDIR

:MKDIR
if not exist "%ROOT%\%BUILD_DIR%" mkdir "%ROOT%\%BUILD_DIR%" || goto ERR_MKDIR

echo.
echo [CMake] Configuring...
cmake -S "%ROOT%" -B "%ROOT%\%BUILD_DIR%" -G "%GENERATOR%" -A %ARCH% -DCMAKE_TOOLCHAIN_FILE="%VCPKG_ROOT%\scripts\buildsystems\vcpkg.cmake" -DVCPKG_TARGET_TRIPLET=%TRIPLET%
if errorlevel 1 goto ERR_CONFIG

echo.
echo [CMake] Building (%CONFIG%)...
cmake --build "%ROOT%\%BUILD_DIR%" --config %CONFIG%
if errorlevel 1 goto ERR_BUILD

echo.
set "SLN_PATH=%ROOT%\%BUILD_DIR%\PhysicsEngine.sln"
if exist "%SLN_PATH%" goto OPEN
for %%F in ("%ROOT%\%BUILD_DIR%\*.sln") do set "SLN_PATH=%%~fF" & goto OPEN
echo [WARN] No .sln found in "%ROOT%\%BUILD_DIR%".
echo Done.
exit /b 0

:OPEN
echo [OK] Solution: "%SLN_PATH%"
echo Opening in Visual Studio...
start "" "%SLN_PATH%"
echo Done.
exit /b 0

:ERR_NOVCPKG
echo [ERROR] VCPKG_ROOT is not set. Example:  setx VCPKG_ROOT C:\dev\vcpkg
exit /b 1

:ERR_TOOLCHAIN
echo [ERROR] Toolchain not found: "%VCPKG_ROOT%\scripts\buildsystems\vcpkg.cmake"
exit /b 1

:ERR_VCPKG
echo [ERROR] vcpkg install failed.
exit /b 1

:ERR_MKDIR
echo [ERROR] Failed to create build dir "%ROOT%\%BUILD_DIR%".
exit /b 1

:ERR_CONFIG
echo [ERROR] CMake configuration failed.
exit /b 1

:ERR_BUILD
echo [ERROR] Build failed.
exit /b 1
