
import math

WAVETABLE_FILE_NAME = "triangle"

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
    return 1

def sphere(phase):
    return 1

# process get called by generate_wavetable.py
# it calculates a single sample at a specified phase
# normalization is handled by generate_wavetable.py
def process(phase):
    return triangle(phase)
