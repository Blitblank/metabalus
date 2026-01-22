
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
- [ ] Create oscillator class where the actual tone generation occurs. Multiple
      oscillators increase the sound complexity considerably
- [x] Create a UI scope to visualize the synthesized composite waveform
- [ ] Create wavetables for more complex tone generation. Needs to be selectable from ui
- [ ] Wavetable file loading
- [x] Create digital filters, prob biquad. Controllable from ui obv (cutoff + resonance)
- [x] Add polyphony somewhere. Probably involves a voice class. If processing power
      allows it, tie a voice to each midi note
- [x] polyphony is lacking sustain pedal rn
- [ ] planning gets sparse this far out because its how far I got with the ESP32 synth
- [ ] Filter upgrades including some more complex solving techniques (State Variable Filter),
      better key tracking, more natural envelope curves, filter drive, etc.
- [ ] Reverb (quite a few ways to do this, needs more research)
- [ ] Pitch tuning with the multiple oscillators (Octave > Semitone > Fine-tune)
- [ ] Frequency Modulation (huge)
- [ ] Profile saving and loading, also includes loading configurations like keymaps, audio
      engine config, etc. from a yaml instead of being hardcoded
- [ ] Noise 
- [ ] LFO modulation

## Build Instructions

Prerequisites:
CMake: https://cmake.org/download/
Ninja: https://github.com/ninja-build/ninja/releases

Windows: MSVC
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
Build. The script will build and install dependencies automatically

On Windows (MSVC):
```PowerShell
.\scripts\build.ps1
```

On Linux (GCC):
```Bash
./scripts/build.sh
```

Configure the CMake/build script if you have issues

To clean:
```
.\scripts\clean.ps1
./scripts/clean.sh
```
Note: dependencies are built into build/lib, so don't delete that unless you want to rebuild qt which takes forever :)
Use the install_dependencies script to manually install dependencies.

Build troubleshooting:
On windows, `bcdedit /set IncreaseUserVa 3072` solved cc1plus.exe: out of memory errors while building qt for me
