@echo off
setlocal

REM ================================
REM Configuration
REM ================================

set BUILD_DIR=build
set CONFIG=Release

set QT_ROOT=C:\Qt\6.10.1\msvc2022_64
set RTAUDIO_ROOT=C:\rtaudio
set RTMIDI_ROOT=C:\rtmidi

REM ================================
REM Environment setup
REM ================================

call "%ProgramFiles%\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvars64.bat"

set PATH=%QT_ROOT%\bin;%PATH%

REM ================================
REM Configure
REM ================================

if not exist %BUILD_DIR% (
    mkdir %BUILD_DIR%
)

cmake -S . -B %BUILD_DIR% ^
    -G Ninja ^
    -DCMAKE_BUILD_TYPE=%CONFIG% ^
    -DRTAUDIO_ROOT=%RTAUDIO_ROOT% ^
    -DRTMIDI_ROOT=%RTMIDI_ROOT% 

if errorlevel 1 goto error

REM ================================
REM Build
REM ================================

cmake --build %BUILD_DIR%

if errorlevel 1 goto error

REM ================================
REM Deploy Qt + RtAudio
REM ================================

cd %BUILD_DIR%

windeployqt metabolus.exe

copy "%RTAUDIO_ROOT%\bin\rtaudio.dll" .
copy "%RTMIDI_ROOT%\bin\rtmidi.dll" .

echo.
echo Build successful.
goto end

:error
echo.
echo Build FAILED.
exit /b 1

:end
endlocal
pause