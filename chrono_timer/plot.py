import matplotlib.pyplot as plt

problems = {
    # 'Problem 1':
    #     {
    #         'size': [100000, 500000, 1000000, 5000000, 10000000, 50000000, 100000000, 500000000],
    #         'o1': [3.0512e-05, 0.000156766, 0.00180037, 0.00912802, 0.00793515, 0.0254765, 0.0669249, 0.16704],
    #         'o3': [0.000273486, 0.0013877, 0.00337125, 0.0278091, 0.0297282, 0.140926, 0.288007, 2.62144]
    #     },
    # 'Problem 2':
    #     {
    #         'size': [100000, 500000, 1000000, 5000000, 10000000, 50000000, 100000000, 500000000],
    #         'o1': [5.6118e-05, 0.000279772, 0.000494816, 0.00235926, 0.00687546, 0.0349227, 0.0668372, 0.438688],
    #         'o3': [0.00104267, 0.0102426, 0.0203584, 0.100045, 0.21256, 0.569069, 0.980771, 5.48926]
    #     },
    # 'Problem 3':
    #     {
    #         'size': [100000, 500000, 1000000, 5000000,  10000000, 50000000 , 100000000],
    #         'o1': [0.000287802, 0.00751086, 0.0159018, 0.0314627, 0.4453, 2.48107, 5.28509],
    #         'o3': [0.000763452, 0.00533849, 0.0117927, 0.0483016, 0.461856, 3.91372, 6.99522]
    #     },
    'Cori: Problem 1':
        {
            'size': [100000, 500000, 1000000, 5000000, 10000000, 50000000, 100000000, 500000000],
            'o1': [0.000989457, 0.00497927, 0.00995327, 0.0498188, 0.0993865, 0.495979, 0.991269, 4.95582],
            'o3': [0.000100821, 0.000500653, 0.00100071, 0.0050299, 0.0100575, 0.0503056, 0.100473, 0.501558]
        },
    'Cori: Problem 2':
        {
            'size': [100000, 500000, 1000000, 5000000, 10000000, 50000000, 100000000, 500000000],
            'o1': [0.00527809, 0.0262128, 0.0521932, 0.261399, 0.522338, 2.60848, 5.21392, 26.0661],
            'o3': [0.000140831, 0.000779526, 0.00147201, 0.00747072, 0.0148543, 0.076494, 0.1531, 0.796726]
        },
    'Cori: Problem 3':
        {
            'size': [100000, 500000, 1000000, 5000000,  10000000, 50000000 , 100000000],
            'o1': [0.00217855, 0.0177107, 0.0406432, 0.129948, 0.794723, 8.18718, 27.1398],
            'o3': [0.000388593, 0.0116157, 0.0285182, 0.061867, 0.531396, 6.67182, 25.543]
        },
}

flops = {"Cori: Problem 1": {"o1":[], "o3":[]}, "Cori: Problem 2": {"o1":[], "o3":[]}, "Cori: Problem 3": {"o1":[], "o3":[]}}
bandwidth = {"Cori: Problem 1": {"o1":[], "o3":[]}, "Cori: Problem 2": {"o1":[], "o3":[]}, "Cori: Problem 3": {"o1":[], "o3":[]}}
access = {"Cori: Problem 1": {"o1":[], "o3":[]}, "Cori: Problem 2": {"o1":[], "o3":[]}, "Cori: Problem 3": {"o1":[], "o3":[]}}
latency = {"Cori: Problem 1": {"o1":[], "o3":[]}, "Cori: Problem 2": {"o1":[], "o3":[]}, "Cori: Problem 3": {"o1":[], "o3":[]}}
# for i, problem in enumerate(problems):
#     plt.figure(i+1)
#     data = problems[problem]
#     plt.plot(data['size'],data['o1'], marker='x', label='o1')
#     plt.plot(data['size'],data['o3'], marker='x', label='o3')
#     plt.title(problem)
#     plt.xlabel('Problem size')
#     plt.ylabel('Runtime(second)')
#     plt.legend()

# plt.show()

def get_avr(l):
    return sum(l) / len(l)

for i, problem in enumerate(problems):
    data = problems[problem]
    for j in range(len(data["size"])):
        flops[problem]["o1"].append(round(2 * data["size"][j] / (1024**3) / data["o1"][j], 2))
        flops[problem]["o3"].append(round(2 * data["size"][j] / (1024**3) / data["o3"][j], 2))
        bandwidth[problem]["o1"].append(round(8 * data["size"][j] / (1024**3) / data["o1"][j], 2))
        bandwidth[problem]["o3"].append(round(8 * data["size"][j] / (1024**3) / data["o3"][j], 2))
        access[problem]["o1"].append(round(data["size"][j] / data["o1"][j] / (1000**3), 2))
        access[problem]["o3"].append(round(8 * data["size"][j] / data["o3"][j] / (1000**3), 2))
        # latency[problem]["o1"].append(round(data["o1"][j] / data["size"][j], 2))
        # latency[problem]["o3"].append(round(data["o3"][j] / data["size"][j], 2))
        latency[problem]["o1"].append(data["o1"][j] / data["size"][j])
        latency[problem]["o3"].append(data["o3"][j] / data["size"][j])
    print("o1 ave", get_avr(flops[problem]["o1"]))
    print("o3 ave", get_avr(flops[problem]["o3"]))
print("flops:", flops)
print("========")
print("bandwidth:", bandwidth)
print("========")
print("access:", access)
print("========")
print("latency:", latency)









