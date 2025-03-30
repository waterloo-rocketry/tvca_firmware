from math import sin, cos, pi, sqrt

a = 30
d = round(a / sqrt(2))
f = 1
fs = 100
phi = -pi/4

print(f'{{{d}, {d}, {fs/2}}},')
print(f'{{{-d}, {-d}, {fs/2}}},')
print(f'{{0, 0, {fs/2}}},')
print(f'{{{-d}, {d}, {fs/2}}},')
print(f'{{{d}, {-d}, {fs/2}}},')

for i in range(1 * fs):
    print(f'{{{round(a * cos(i*f*2*pi/fs+phi))}, {round(a * sin(i*f*2*pi/fs+phi))}, 1}},')

print(f'{{{d}, {-d}, {fs/2}}},')
print(f'{{0, 0, 0}},')
