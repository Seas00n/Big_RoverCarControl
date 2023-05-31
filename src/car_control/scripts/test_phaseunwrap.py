import numpy as np
import matplotlib.pyplot as plt
PI = np.pi
def unwrap(original_phase, phase):
    diff = abs(phase-original_phase)
    if(diff<PI):
        return phase
    else:
        multi = diff/(2*PI)
        remain = diff%(2*PI)
        move = np.floor(multi)
        if remain>PI:
            move += 1
        if phase>original_phase:
            return phase-move*2*PI
        else:
            return phase+move*2*PI


t = np.arange(0,10000)
phase = np.zeros_like(t).astype('float32')
phase_sec = np.linspace(-PI,PI,1000).astype('float32')

for i in range(10):
    for j in range(np.size(phase_sec)):
        phase[i*np.size(phase_sec,0)+j] = phase_sec[j]


phase_unwrap = np.zeros_like(t).astype('float32')
original_phase = phase[0]
for i in range(np.size(phase,0)):
    phase_unwrap[i] = unwrap(original_phase,phase[i])
    original_phase = phase_unwrap[i]
    if i==999:
        print(i)

fig = plt.figure()
plt.plot(t,phase)
plt.plot(t,phase_unwrap)
plt.show()

