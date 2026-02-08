
import math

from generate_wavetable import generateWavetable

WAVETABLE_FILE_NAME = "sigmoid"

# process expects a waveform from x=[0, 2pi) centered around f(x)=0
# normalization is handled by the wavetableGenerator

def sine(phase):
    return math.sin(phase)

def square(phase):
    sample = 1.0
    if(phase <= math.pi):
        sample = -1
    return sample

def saw(phase):
    return (phase / math.pi) - 1.0

def triangle(phase):
    sample = 0.0
    if(phase <= math.pi/2.0):
        sample = phase * 2.0/math.pi
    elif(phase <= 3.0*math.pi/2.0):
        sample = phase * (-2.0/math.pi) + 2.0
    else:
        sample = phase * 2.0/math.pi - 4.0
    return sample

def sharkFin(phase):
    k = 10.0
    if(phase <= math.pi):
        sample = 2 * (phase/math.pi) ** k - 1.0
    else:
        sample = -2 * (phase/math.pi - 1.0) ** k + 1.0
    return sample

def sigmoid(phase):
    k = 4
    a = -k * (phase - math.pi)
    sample = 2 / (1 + math.exp(a)) - 1
    return sample

# pass in the name of your wavtable file and the process function
def main():
    generateWavetable(WAVETABLE_FILE_NAME, sigmoid)

if __name__ == "__main__":
    main()
