from math import sin, cos, pi

a = 30
f = 1
fs = 100
for i in range(1 * fs):
    print(f'{{{int(a * cos(i*f*2*pi/fs))}, {int(a * sin(i*f*2*pi/fs))}}},')
