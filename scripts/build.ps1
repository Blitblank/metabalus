
$PROJECT_ROOT = $PWD

# config

$BUILD_DIR = "$PWD/build"
$CONFIG = "Release"

# change these to your need
# or TODO: make qt_root configurable
$QT_ROOT = "C:\Qt\6.10.1\msvc2022_64"
$RTAUDIO_ROOT = "$BUILD_DIR\lib\rtaudio"
$RTMIDI_ROOT = "$BUILD_DIR\lib\rtmidi"
$YAMLCPP_ROOT = "$BUILD_DIR\lib\yaml-cpp"

$CONFIG_ROOT = "$PROJECT_ROOT\config"

# setup

& "$Env:Programfiles\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvars64.bat"

$PATH="$QT_ROOT\bin;$PATH"

if (-not (Test-Path -Path $BUILD_DIR)) {
    mkdir $BUILD_DIR
}

# detect dependencies

$libraries = @("rtaudio", "rtmidi", "yaml-cpp")
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
cmake -S . -B $BUILD_DIR -G "Visual Studio 17 2022" `
    -DQt6_ROOT="$QT_ROOT\lib\cmake\Qt6" `
    -DRtAudio_ROOT="$RTAUDIO_ROOT" `
    -DRtMidi_ROOT="$RTMIDI_ROOT" `
    -Dyaml-cpp_ROOT="$YAMLCPP_ROOT" `


# build
Write-Host "Building metabolus..."
cmake --build $BUILD_DIR --config $CONFIG

# TODO: install
cd $BUILD_DIR
cmake --install . --prefix ".\"

# link dlls
Write-Host "Deploying metabolus..."
cd $BUILD_DIR

& "$QT_ROOT\bin\windeployqt6.exe" .\$CONFIG\metabolus.exe

# copy dlls
Copy-Item -Path "$RTAUDIO_ROOT\bin\rtaudio.dll" -Destination .\$CONFIG
Copy-Item -Path "$RTMIDI_ROOT\bin\rtmidi.dll" -Destination .\$CONFIG
Copy-Item -Path "$YAMLCPP_ROOT\bin\yaml-cpp.dll" -Destination .\$CONFIG

# copy configs, but don't overwrite
Copy-Item -Path "$CONFIG_ROOT" -Destination ".\$CONFIG\" -Recurse -ErrorAction SilentlyContinue

cd $PROJECT_ROOT
