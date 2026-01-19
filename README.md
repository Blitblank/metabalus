
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

## setup
TODO: instructions on build setup
TODO: add dependencies into lib
TODO: autobuild libs in build scripts
Package Dependencies: Qt 6, RtAudio, RtMidi

$ ./scripts/build.sh
PS ./scripts/build.bat
