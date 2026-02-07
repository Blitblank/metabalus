
# this is a script for generating a wavetable file 
# a wavetable file consists of a one-dimensional array of samples representing one period of a waveform
# metadata includes: 
#    - file version (for program compatibility)
#    - binary format (float, double, int32, etc.)
#    - domain (normal is a phase from x=0 to x=2pi)
#    - range (depending on datatypes, e.g. float=[-1,1], int32=[-2^15, 2^15-1])
#    - waveform RMS (for loudness normalization)
# the synth program uses the filename, not any metadata

# this script uses the function defined in example_wavetable.py to calculate samples
# if you want a custom wavetable, copy/edit/modify the example function (desmos is great for brainstorming)

import example_wavetable

def createFile():
    print("creating file")
    return 1

def writeMetadata(file):
    print("im writing metadata")

def generateWavetable(file):
    print("im generating the wavetable")
    example_wavetable.process()

def closeFile(file):
    print("finishing up")

def main():
    print("Hello main")
    file = createFile()
    writeMetadata(file)
    generateWavetable(file)
    closeFile(file)
    print("done")
    

if __name__ == "__main__":
    main()
