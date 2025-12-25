
# metabolus

A QTWidgets C++ app for a basic digital synthesizer.

This synthesizer isn't very good, but it's neat :3 

## Development plan: 
- [x] Build & project setup, get working hello-world program.
- [x] QTWidgets hello-world program: basic increment/reset counter
- [x] RtAudio hello-world: basic sine output
- [x] Connect UI control to sound output, add a slider for frequency control
- [+] Add note control via either Midi or a keyboard. Coordinate on-off events to
      start and stop tone generation
- [ ] Create a widget for this smart-slider to clean up the ui code
- [ ] Add envelope generation, attach to global volume for now. ADSR and such,
      responds to note-on/note-off events
- [ ] Make midi/keyboard control cross-platform. Use case will mostly be
      Midi -> linux and Keyboard -> windows though
- [ ] Create oscillator class where the actual tone generation occurs. Multiple
      oscillators increase the sound complexity considerably
- [ ] Create a UI scope to visualize the synthesized composite waveform
- [ ] Create wavetables for more complex tone generation. Needs to be selectable from ui
- [ ] Create digital filters, prob biquad. Controllable from ui obv (cutoff + resonance)
- [ ] Add polyphony somewhere. Probably involves a voice class. If processing power
      allows it, tie a voice to each midi note
- [ ] Add noise ? 
- [ ] planning gets sparse this far out because its how far I got with the ESP32 synth

x = complete, + = implemented, in progress, o = working on it

## setup
TODO: instructions on build setup
