@echo off
setlocal

:: ─────────────────────────────────────────────────────────────────────────────
:: MWOn12SDK — build the sample plugins
::
:: The SDK itself is already built; this compiles the samples against it.
:: Needs Visual Studio 2022 with the C++ workload. Nothing else, no network.
:: ─────────────────────────────────────────────────────────────────────────────

set BUILD_DIR=build_win32

cmake -S . -B %BUILD_DIR% -G "Visual Studio 17 2022" -A Win32
if errorlevel 1 ( echo. & echo CMake configure failed. & pause & exit /b 1 )

cmake --build %BUILD_DIR% --config Release
if errorlevel 1 ( echo. & echo Build failed. & pause & exit /b 1 )

echo.
echo -- Build complete --
echo   Plugins: %BUILD_DIR%\bin\Plugins\
echo.
echo Copy the plugin DLLs into:
echo   ^<game^>\MWOn12\Plugins\
echo and set VerboseLog=1 in MWOn12.ini to see their log output.
echo.
pause
