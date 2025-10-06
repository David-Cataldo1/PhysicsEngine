@echo off
setlocal EnableExtensions EnableDelayedExpansion

rem ============================================
rem build_vs2022.bat
rem - Generates VS2022 solution with vcpkg toolchain
rem - Builds (Debug/Release) and opens the solution
rem Usage:
rem   build_vs2022.bat            (defaults to Release)
rem   build_vs2022.bat Debug
rem   build_vs2022.bat Release
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
echo === PhysicsEngine: Configure for %GENERATOR% (%ARCH%), %CONFIG% ===

if not defined VCPKG_ROOT (
  echo [ERROR] VCPKG_ROOT is not set.
  echo         Install vcpkg and set:  setx VCPKG_ROOT C:\dev\vcpkg
  exit /b 1
)
if not exist "%VCPKG_ROOT%\scripts\buildsystems\vcpkg.cmake" (
  echo [ERROR] Toolchain not found: "%VCPKG_ROOT%\scripts\buildsystems\vcpkg.cmake"
  exit /b 1
)

echo.
echo [vcpkg] Ensuring dependencies are installed (triplet: %TRIPLET%)...
"%VCPKG_ROOT%\vcpkg.exe" install glfw3 glad glm entt spdlog --triplet=%TRIPLET%
if errorlevel 1 (
  echo [ERROR] vcpkg install failed.
  exit /b 1
)

if not exist "%ROOT%\%BUILD_DIR%" (
  mkdir "%ROOT%\%BUILD_DIR%" || ( echo [ERROR] Failed to create build dir & exit /b 1 )
)

echo.
echo [CMake] Configuring...
cmake -S "%ROOT%" -B "%ROOT%\%BUILD_DIR%" ^
  -G "%GENERATOR%" -A %ARCH% ^
  -DCMAKE_TOOLCHAIN_FILE="%VCPKG_ROOT%\scripts\buildsystems\vcpkg.cmake" ^
  -DVCPKG_TARGET_TRIPLET=%TRIPLET%
if errorlevel 1 ( echo [ERROR] CMake configuration failed. & exit /b 1 )

echo.
echo [CMake] Building (%CONFIG%)...
cmake --build "%ROOT%\%BUILD_DIR%" --config %CONFIG%
if errorlevel 1 ( echo [ERROR] Build failed. & exit /b 1 )

echo.
set "SLN_PATH="
if exist "%ROOT%\%BUILD_DIR%\PhysicsEngine.sln" (
  set "SLN_PATH=%ROOT%\%BUILD_DIR%\PhysicsEngine.sln"
) else (
  for %%F in ("%ROOT%\%BUILD_DIR%\*.sln") do ( set "SLN_PATH=%%~fF" & goto :found )
)
:found

if defined SLN_PATH (
  echo [OK] Solution: "%SLN_PATH%"
  echo Opening in Visual Studio...
  start "" "%SLN_PATH%"
) else (
  echo [WARN] No .sln found in "%ROOT%\%BUILD_DIR%".
)

echo Done.
exit /b 0
