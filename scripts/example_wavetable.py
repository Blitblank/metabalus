
import math

WAVETABLE_FILE_NAME = "sharkFin"

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

def sphere(phase):
    return 1

# process get called by generate_wavetable.py
# it calculates a single sample at a specified phase
# normalization is handled by generate_wavetable.py
def process(phase):
    return sharkFin(phase)
