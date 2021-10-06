import matplotlib.pyplot as plt
import math

# Max runtime
problems = {
    'basic': {
        'P=1': [0.0123,1.9959,181.4693],
        'P=4': [0.0035,0.6521,58.4116],
        'P=16': [0.0019,0.1770,15.3320],
        'P=64': [0.1548,0.0757,4.0984],
    },
    'bmmco': {
        'P=1, B=4': [0.0257,2.8115,176.3156],
        'P=1, B=16': [0.0144,0.8176,52.0498],
        'P=4, B=4': [0.0076,0.8192,52.1404],
        'P=4, B=16': [0.0041,0.2151,13.5306],
        'P=16, B=4': [0.0035,0.2025,13.2581],
        'P=16, B=16': [0.0020,0.0539,3.3899],
        'P=64, B=4': [0.1203,0.0824,3.1632],
        'P=64, B=16': [0.1480,0.0455,0.8788],
    },
    'size':[128, 512, 2048]
}

# def get_flops(times, size):
#     mflops = []
#     for i in range(len(times)):
#         mflop = 2 * size[i]**2 / (1024**3)
#         divide =  mflop / times[i]
#         mflops.append(divide)
#     return mflops

def get_speedup(times, baseline):
    speedups = []
    for i in range(len(times)):
        divide =  baseline[i] / times[i]
        speedups.append(divide)
    return speedups

# Figure 1
basic = problems['basic']
plt.figure(1)
for i in basic:
    if i == 'P=1':
        continue
    plt.plot(problems['size'],get_speedup(basic[i], basic['P=1']), marker='x', label=i)
    plt.xlabel('Problem size')
    plt.ylabel('Speedup')
    plt.legend()
plt.title("Basic MM with OpenMP")
plt.grid()

bmmco = problems['bmmco']
plt.figure(2)
for i in bmmco:
    if i == 'P=1, B=4' or i == 'P=1, B=16':
        continue
    if i == 'P=4, B=4' or i == 'P=16, B=4' or i == 'P=64, B=4':
        plt.plot(problems['size'],get_speedup(bmmco[i], bmmco['P=1, B=4']), marker='x', label=i)
    else:
        plt.plot(problems['size'],get_speedup(bmmco[i], bmmco['P=1, B=16']), marker='x', label=i)
    plt.xlabel('Problem size')
    plt.ylabel('Speedup')
    plt.legend()
plt.title("BMMCO with OpenMP")
plt.grid()

plt.figure(3)
for i in bmmco:
    if i == 'P=1, B=4' or i == 'P=1, B=16':
        continue
    if i == 'P=4, B=4' or i == 'P=16, B=4' or i == 'P=64, B=4':
        log = [math.log(y) for y in get_speedup(bmmco[i], bmmco['P=1, B=4'])]
        plt.plot(problems['size'],log, marker='x', label=i)
    else:
        log = [math.log(y) for y in get_speedup(bmmco[i], bmmco['P=1, B=16'])]
        plt.plot(problems['size'],log, marker='x', label=i)
    plt.xlabel('Problem size')
    plt.ylabel('Speedup(in ln)')
    plt.legend()
plt.title("BMMCO with OpenMP (Apply log)")
plt.grid()
plt.show()









