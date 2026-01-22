
$PROJECT_ROOT = $PWD

# config

$BUILD_DIR = "$PWD/build"
$CONFIG = "Release"

# change these to the build libs
$QT_ROOT = "$BUILD_DIR\lib\qt\install"
$RTAUDIO_ROOT = "$BUILD_DIR\lib\rtaudio"
$RTMIDI_ROOT = "$BUILD_DIR\lib\rtmidi"

# setup

& "$Env:Programfiles\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvars64.bat"

$PATH="$QT_ROOT\bin;$PATH"

if (-not (Test-Path -Path $BUILD_DIR)) {
    mkdir $BUILD_DIR
}

# detect dependencies

$libraries = @("qt", "rtaudio", "rtmidi")
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
    Write-Host "All dependencies detected, skipping depency install step..."
}

# configure
Write-Host "Configuring metabolus..."
cmake -S . -B $BUILD_DIR -G Ninja -DCMAKE_BUILD_TYPE=$CONFIG -DQt6_ROOT="$QT_ROOT\lib\cmake\Qt6" -DRTAUDIO_ROOT=$RTAUDIO_ROOT -DRTMIDI_ROOT=$RTMIDI_ROOT 

# build
Write-Host "Building metabolus..."
cmake --build $BUILD_DIR

# link dlls
Write-Host "Deploying metabolus..."
cd $BUILD_DIR

& "$QT_ROOT\bin\windeployqt.exe" metabolus.exe

Copy-Item -Path "$RTAUDIO_ROOT\bin\rtaudio.dll" -Destination .
Copy-Item -Path "$RTMIDI_ROOT\bin\rtmidi.dll" -Destination .


# TODO: allow input of an external qt install because this one is huge 
# TODO: remove unnecessary qt modules bc why is this install like 80 gb
