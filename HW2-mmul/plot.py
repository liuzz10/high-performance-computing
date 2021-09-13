import matplotlib.pyplot as plt

problems = {
    'Mmul: Basic vs. CBLAS':
        {
            'size': [64, 128, 256, 512, 1024],
            'Basic': [0.000406751, 0.003713615, 0.038682237, 0.384241911, 7.465015875],
            'CBLAS': [0.000132071, 0.000197915, 0.001070416, 0.006578540, 0.052980467]
        },
    'Mmul: BMMCO vs. CBLAS':
        {
            'size': [64, 128, 256, 512, 1024],
            'Basic': [0.000406751, 0.003713615, 0.038682237, 0.384241911, 7.465015875],
            'CBLAS': [0.000132071, 0.000197915, 0.001070416, 0.006578540, 0.052980467],
            'size2': [0.000755056, 0.005812432, 0.057160967, 0.440759875, 4.473087937],
            'size16': [0.000253976, 0.001842965, 0.016248778, 0.129539558, 1.015333969],
            'size32': [0.000341540, 0.002682007, 0.021854335, 0.164995170, 1.304600111],
            'size64': [0.000435553, 0.002833187, 0.024814909, 0.184663909, 1.496730498],
        },
}


plt.figure(1)
data = problems['Mmul: Basic vs. CBLAS']
flop_basic = []
flop_cblas = []
for i in range(len(data['size'])):
    total_flop = 2 * data['size'][i]**3 / (1024**3)
    basic =  total_flop / data['Basic'][i]
    flop_basic.append(basic)
    cblas = total_flop / data['CBLAS'][i]
    flop_cblas.append(cblas)
plt.plot(data['size'],flop_cblas, marker='x', label='CBLAS')
plt.plot(data['size'],flop_basic, marker='x', label='Basic')
plt.title('Mmul: Basic vs. CBLAS')
plt.xlabel('Problem size')
plt.ylabel('MFLOP/s')
plt.legend()





plt.figure(2)
data = problems['Mmul: BMMCO vs. CBLAS']
flop_basic = []
flop_cblas = []
flop_2 = []
flop_16 = []
flop_32 = []
flop_64 = []
for i in range(len(data['size'])):
    total_flop = 2 * data['size'][i]**3 / (1024**3)
    basic =  total_flop / data['Basic'][i]
    flop_basic.append(basic)
    flop_cblas.append(total_flop / data['CBLAS'][i])
    flop_2.append(total_flop / data['size2'][i])
    flop_16.append(total_flop / data['size16'][i])
    flop_32.append(total_flop / data['size32'][i])
    flop_64.append(total_flop / data['size64'][i])

# plt.plot(data['size'],flop_cblas, marker='x', label='CBLAS')
plt.plot(data['size'],flop_basic, marker='x', label='Basic')
plt.plot(data['size'],flop_2, marker='x', label='block size 2')
plt.plot(data['size'],flop_16, marker='x', label='block size 16')
plt.plot(data['size'],flop_32, marker='x', label='block size 32')
plt.plot(data['size'],flop_64, marker='x', label='block size 64')

plt.title('Mmul: Basic vs. CBLAS')
plt.xlabel('Problem size')
plt.ylabel('MFLOP/s')
plt.legend()



plt.show()









