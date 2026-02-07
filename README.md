
# metabolus

A QTWidgets C++ app for a basic digital synthesizer.

This synthesizer isn't very good, but it's neat :3 

## Development plan: 
- [x] Build & project setup, get working hello-world program.
- [x] QTWidgets hello-world program: basic increment/reset counter
- [x] RtAudio hello-world: basic sine output
- [x] Connect UI control to sound output, add a slider for frequency control
- [x] Add note control via either Midi or a keyboard. Coordinate on-off events to
      start and stop tone generation
- [x] Create a widget for this smart-slider to clean up the ui code
- [x] Add envelope generation, attach to global volume for now. ADSR and such,
      responds to note-on/note-off events
- [x] Make midi/keyboard control cross-platform. Use case will mostly be
      Midi -> linux and Keyboard -> windows though
- [x] Create oscillator class where the actual tone generation occurs. Multiple
      oscillators increase the sound complexity considerably
- [x] Create a UI scope to visualize the synthesized composite waveform
- [x] Create wavetables for more complex tone generation. Needs to be selectable from ui
- [x] Wavetable file loading
- [x] Create digital filters, prob biquad. Controllable from ui obv (cutoff + resonance)
- [x] Add polyphony somewhere. Probably involves a voice class. If processing power
      allows it, tie a voice to each midi note
- [x] polyphony is lacking sustain pedal rn
- [ ] planning gets sparse this far out because its how far I got with the ESP32 synth
- [ ] Filter upgrades including some more complex solving techniques (State Variable Filter),
      better key tracking, more natural envelope curves, filter drive, etc.
- [ ] Reverb (quite a few ways to do this, needs more research)
- [x] Pitch tuning with the multiple oscillators (Octave > Semitone > Fine-tune)
- [ ] Frequency Modulation (huge)
- [x] Profile saving and loading, also includes loading configurations like keymaps, audio
      engine config, etc. from a yaml instead of being hardcoded
- [ ] Noise 
- [ ] LFO modulation

## Build Instructions

Prerequisites:
CMake: https://cmake.org/download/ \
QtWidgets: https://www.qt.io/development/download-qt-installer-oss

Windows: MSVC (The build scripts use Visual Studio 17 2022)
Linux: GCC

Clone repository
```PowerShell
git clone https://github.com/Blitblank/metabalus.git --recursive
```
or if you forgot to --recursive:
```PowerShell
git clone https://github.com/Blitblank/metabalus.git
git submodule update --init --recursive
```
\
Build. The script will build and install dependencies automatically

On Windows (MSVC):
```PowerShell
.\scripts\build.ps1 # builds in build/Debug/
```

On Linux (GCC):
```Bash
./scripts/build.sh
```

TODO: right now you need to run the executable from the executable's directory because of CWD paths and such. Needs to be fixed because annoying

Configure the CMake/build script if you have issues

To clean:
```
.\scripts\clean.ps1
./scripts/clean.sh
```
Note: dependencies are built into build/lib, so don't delete unless you need to rebuild the libraries
Use the install_dependencies script to manually install dependencies.

Build troubleshooting:
On windows, `bcdedit /set IncreaseUserVa 3072` solved cc1plus.exe: out of memory errors while building qt for me

## Configurations (NOT YET IMPLEMENTED)
Default config files are located in the config/ directory, and they are replicated into build/config/ if they dont already exist there. To edit the configurations, edit the config files in the build directory, not the defaults. Most config files are loaded/parsed at startup (TODO: investigate some reloading functions), so the program must be restarted, although not recompiled, for new configs to take effect. \
Voice profiles are saved into config files into a human-readable format (YAML) and can be edited manually or by saving within the app. \

## Wavetables (NOT YET IMPLEMENTED)
Wavetables are this synthesizer's starting point for audio synthesis. A wavetable (as defined for this synthesizer, not elsewhere) contains a single period of a particular wave-shape with a discrete number of samples. Wavetables are loaded at runtime and sampled by oscillator objects to define and mix different wave shapes. Further specifications, as well as instructions for generating your own wavetable (including an example python script << TODO), are located within config/wavetables/README.md

