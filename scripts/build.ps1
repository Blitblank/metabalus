
# config

$BUILD_DIR = "build"
$CONFIG = "Release"

# change these to the build libs
$QT_ROOT = "C:\Qt\6.10.1\msvc2022_64"
$RTAUDIO_ROOT = "C:\rtaudio"
$RTMIDI_ROOT = "C:\rtmidi"

# setup

& "$Env:Programfiles\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvars64.bat"

$PATH="$QT_ROOT\bin;$PATH"

if (-not (Test-Path -Path $BUILD_DIR)) {
    mkdir $BUILD_DIR
}

# detect dependencies

$libraries = @("qt", "rtaudio", "rtmidi", "yaml-cpp")
$dependencies_found = 0
foreach ($lib in $libraries) {
    if (Test-Path -Path ".\build\lib\$lib") {
        Write-Host "found $lib"
        $dependencies_found++
    } else {
        Write-Host "did not find $lib"
    }
}

if (-not ($dependencies_found -eq $libraries.Count)) {
    & "scripts\install_dependencies.ps1"
}

# configure

<#
cmake -S . -B $BUILD_DIR ^
    -G Ninja ^
    -DCMAKE_BUILD_TYPE=$CONFIG ^
    -DRTAUDIO_ROOT=$RTAUDIO_ROOT ^
    -DRTMIDI_ROOT=$RTMIDI_ROOT 


if errorlevel 1 goto error

# build
cmake --build $BUILD_DIR

if errorlevel 1 goto error

# link dlls

cd %BUILD_DIR%

windeployqt metabolus.exe

copy "%RTAUDIO_ROOT%\bin\rtaudio.dll" .
copy "%RTMIDI_ROOT%\bin\rtmidi.dll" .

echo.
echo Build successful
goto end

:error
echo.
echo Build failed
exit /b 1

:end
endlocal
pause
#>