import matplotlib.pyplot as plt

problems = {
    'basic':[0.001069770, 0.004062506, 0.016500845, 0.064860580, 0.276027845],
    'cblas':[0.001018979, 0.003674522, 0.014140223, 0.059457105, 0.250049234],
    'openmp':[0.000405899, 0.000693438, 0.000878187, 0.001924074, 0.006809719],
    '1':[0.001119488, 0.003986310, 0.015893770, 0.062888863, 0.254458907],
    '2':[0.000556979, 0.002185885, 0.008074500, 0.031682841, 0.129369829],
    '4':[0.000299779, 0.001235237, 0.004151660, 0.015897751, 0.064183109],
    '8':[0.000172660, 0.000765328, 0.002232334, 0.008176440, 0.032183399],
    '16':[0.000110850, 0.000560449, 0.001385117, 0.004351289, 0.016313310],
    '32':[0.000087940, 0.000558178, 0.000936858, 0.002575633, 0.008766248],
    '64':[0.000096149, 0.000402789, 0.000813358, 0.002020425, 0.006952813],
    '1d':[0.001108207, 0.004015937, 0.015914489, 0.063743487, 0.269449735],
    '2d':[0.000561884, 0.002103734, 0.008110706, 0.032096580, 0.138737700],
    '4d':[0.000301112, 0.001161168, 0.004223979, 0.016440332, 0.069005222],
    '8d':[0.000173321, 0.000771175, 0.002349256, 0.008568439, 0.035296151],
    '16d':[0.000112351, 0.000568934, 0.001309599, 0.004672792, 0.018643117],
    '32d':[0.000087350, 0.000572454, 0.000925547, 0.002799809, 0.010676893],
    '64d':[0.000095881, 0.000504044, 0.000809215, 0.002101034, 0.007764244],
    'size':[1024, 2048, 4096, 8192, 16384]
}

def get_flops(times, size):
    mflops = []
    for i in range(len(times)):
        mflop = 2 * size[i]**2 / (1024**3)
        divide =  mflop / times[i]
        mflops.append(divide)
    return mflops

def get_speedup(times):
    speedups = []
    for i in range(len(times)):
        divide =  problems['1'][i] / times[i]
        speedups.append(divide)
    return speedups

# Figure 1
plt.figure(1)
plt.plot(problems['size'],get_flops(problems['cblas'], problems['size']), marker='x', label='CBLAS')
plt.plot(problems['size'],get_flops(problems['basic'], problems['size']), marker='x', label='Basic')
plt.title('VMM: Basic vs. CBLAS')
plt.xlabel('Problem size')
plt.ylabel('MFLOP/s')
plt.legend()

# Figure 2
plt.figure(2)
plt.plot(problems['size'],get_speedup(problems['1']), marker='x', label='1')
plt.plot(problems['size'],get_speedup(problems['2']), marker='x', label='2')
plt.plot(problems['size'],get_speedup(problems['4']), marker='x', label='4')
plt.plot(problems['size'],get_speedup(problems['8']), marker='x', label='8')
plt.plot(problems['size'],get_speedup(problems['16']), marker='x', label='16')
plt.plot(problems['size'],get_speedup(problems['32']), marker='x', label='32')
plt.plot(problems['size'],get_speedup(problems['64']), marker='x', label='64')
plt.title('VMM: Concurrency comparison (static)')
plt.xlabel('Problem size')
plt.ylabel('Speedup')
plt.legend()

# Figure 3
plt.figure(3)
plt.plot(problems['size'],get_speedup(problems['1d']), marker='x', label='1')
plt.plot(problems['size'],get_speedup(problems['2d']), marker='x', label='2')
plt.plot(problems['size'],get_speedup(problems['4d']), marker='x', label='4')
plt.plot(problems['size'],get_speedup(problems['8d']), marker='x', label='8')
plt.plot(problems['size'],get_speedup(problems['16d']), marker='x', label='16')
plt.plot(problems['size'],get_speedup(problems['32d']), marker='x', label='32')
plt.plot(problems['size'],get_speedup(problems['64d']), marker='x', label='64')
plt.title('VMM: Concurrency comparison (dynamic)')
plt.xlabel('Problem size')
plt.ylabel('Speedup')
plt.legend()

# Figure 4
plt.figure(4)
plt.plot(problems['size'],get_flops(problems['cblas'], problems['size']), marker='x', label='CBLAS')
plt.plot(problems['size'],get_flops(problems['64'], problems['size']), marker='x', label='OpenMP-64')
plt.title('VMM: OpenMP vs. CBLAS')
plt.xlabel('Problem size')
plt.ylabel('MFLOP/s')
plt.legend()

# Figure 5
plt.figure(5)
plt.plot(problems['size'],get_speedup(problems['cblas']), marker='x', label='cblas')
plt.plot(problems['size'],get_speedup(problems['1']), marker='x', label='1')
plt.title('VMM: OpenMP-1 vs. CBLAS')
plt.xlabel('Problem size')
plt.ylabel('Speedup')
plt.legend()


plt.show()









