
$PROJECT_ROOT = $PWD

# config

$BUILD_DIR = "$PWD/build"
$CONFIG = "Release"

# change these to your need
$QT_ROOT = "C:\Qt\6.10.1\msvc2022_64"
$RTAUDIO_ROOT = "$BUILD_DIR\lib\rtaudio"
$RTMIDI_ROOT = "$BUILD_DIR\lib\rtmidi"

# setup

& "$Env:Programfiles\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvars64.bat"

$PATH="$QT_ROOT\bin;$PATH"

if (-not (Test-Path -Path $BUILD_DIR)) {
    mkdir $BUILD_DIR
}

# detect dependencies

$libraries = @("rtaudio", "rtmidi")
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
} else {
    Write-Host "All dependencies detected, skipping dependency install step..."
}

# configure
Write-Host "Configuring metabolus..."
cmake -S . -B $BUILD_DIR -G "Visual Studio 17 2022" -DCMAKE_BUILD_TYPE=Release -DQt6_ROOT="$QT_ROOT\lib\cmake\Qt6" -DRtAudio_ROOT="$RTAUDIO_ROOT" -DRtMidi_ROOT="$RTMIDI_ROOT"

# build
Write-Host "Building metabolus..."
cmake --build $BUILD_DIR

# link dlls
Write-Host "Deploying metabolus..."
cd $BUILD_DIR

& "$QT_ROOT\bin\windeployqt6.exe" .\Debug\metabolus.exe

Copy-Item -Path "$RTAUDIO_ROOT\bin\rtaudio.dll" -Destination .\Debug
Copy-Item -Path "$RTMIDI_ROOT\bin\rtmidi.dll" -Destination .\Debug


# TODO: allow input of an external qt install because this one is huge 
# TODO: remove unnecessary qt modules bc why is this install like 80 gb

cd $PROJECT_ROOT
