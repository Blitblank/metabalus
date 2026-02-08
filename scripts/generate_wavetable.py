
# this is a script for generating a wavetable file 
# a wavetable file consists of a one-dimensional array of samples representing one period of a waveform
# metadata includes: 
#    - file version (for program compatibility)
#    - binary format (float, double, int32, etc.) (RIGHT NOW I ONLY USE FLOAT)
#    - domain (normal is a phase from x=0 to x=2pi)
#    - range (depending on datatypes, e.g. float=[-1,1], int32=[-2^15, 2^15-1])
#    - waveform RMS (for loudness normalization)
#    - sample count 
# the synth program uses the filename, not any metadata

# this script uses the function defined in example_wavetable.py to calculate samples
# if you want a custom wavetable, copy/edit/modify the example function (desmos is great for brainstorming)
# import this script and call generateWavetable(processFunction) to generate a custom wavetable

from array import array
import math

wavetableLength = 2048

def createFile(name):
    filename = name + ".wt"
    print("creating file " + filename)
    file = open(filename, "wb")
    return file

def writeMetadata(file):
    print(">> im writing metadata")

def writeData(file, processFunction):
    print(">> im generating the wavetable")

    # init variables
    data_list = [None] * wavetableLength
    phaseInc = 2*math.pi / wavetableLength
    x = 0
    accumulator = 0

    # generate each discrete sample
    for i in range(wavetableLength): 
        sample = processFunction(x)
        accumulator += sample * sample
        x += phaseInc
        data_list[i] = sample

    # normalize by rms
    rms = math.sqrt(accumulator/wavetableLength)
    print(">> wavetable RMS: ", rms)
    for i in range(wavetableLength): 
        data_list[i] /= rms

    # write to file
    binary_data = array("f", data_list)
    file.write(binary_data)

def closeFile(file): 
    print(">> finishing up")
    file.close()

def generateWavetable(name, processFunction):
    print("Hello main")
    file = createFile(name)
    writeMetadata(file)
    writeData(file, processFunction)
    closeFile(file)
    print("done")
    