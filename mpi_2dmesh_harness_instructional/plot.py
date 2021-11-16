import matplotlib.pyplot as plt
import math


problems = {
    'row-slab': {
        'scatter': [1110.9762, 2193.4005, 3919.8821, 6136.4912, 8938.4833, 12292.2743, 16047.6137, 20343.7645],
        'sobel': [333.4846, 149.0440, 85.3406, 58.2849, 45.4689, 27.6344, 27.4191, 27.4292],
        'gather': [194.9209, 94.1331, 81.3224, 75.0791, 73.6568, 72.4878, 72.7697, 83.0275],
    },
    'col-slab': {
        'scatter': [1106.5842, 2216.4470, 3943.9331, 6212.5032, 8995.2463, 12281.2571, 16095.5288, 20267.0632],
        'sobel': [333.9629, 149.7169, 85.7183, 56.1850, 41.3514, 29.0511, 22.9580, 29.0092],
        'gather': [200.0052, 107.8461, 100.7804, 114.2853, 101.2190, 106.5805, 99.9656, 119.8329],
    },
    'tiled': {
        'scatter': [1109.1019, 2234.6479, 3963.8883, 6158.3882, 8962.4243, 12189.0303, 15704.4758, 20392.6916],
        'sobel': [333.5515, 148.0318, 83.4960, 53.5646, 37.4044, 27.4674, 21.1751, 17.0766],
        'gather': [197.1831, 87.7970, 79.1348, 71.3553, 71.6887, 84.7492, 76.9786, 82.8292],
    },
    'size':[4,9,16,25,36,49,64,81]
}


i = 1
for key in problems:
    plt.figure(i)
    i += 1
    if key == 'size':
        continue
    for step in problems[key]:
        plt.plot(problems['size'],[math.log(num) for num in problems[key][step]], marker='x', label=step)
    plt.xlabel('Concurrency')
    plt.ylabel('Runtime(ms) in log')
    plt.legend()
    plt.title(key)
    plt.grid()
    plt.show()


P=[4, 9, 16, 25, 36, 49, 64, 81]
width = 7112
height = 5146
def row():
    res = []
    for p in P:
        scatter = 4 * ((height/p+2)*(width+2))
        gather = 4 * ((height/p)*width)
        res.append((p-1)*(scatter+gather)/100000)
    print("row", res)

def col():
    res = []
    for p in P:
        scatter = 4 * ((height+2)*(width/p+2))
        gather = 4 * (height*width/p)
        res.append((p-1)*(scatter+gather)/100000)
    print("col", res)

def tile():
    res = []
    for p in P:
        r = math.sqrt(p)
        scatter = 4 * ((height/r+2)*(width/r+2))
        gather = 4 * ((height/r)*(width/r))
        res.append((p-1)*(scatter+gather)/100000)
    print("tile", res)

row()
col()
tile()







