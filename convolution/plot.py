import numpy as np
import matplotlib
import matplotlib.pyplot as plt

filters = ['1', '10', '100', '1000'] # x axis
channel_dimension = ['1000', '100', '10', '1'] # y axis

# # RUNTIME efficiency
# runtime = np.array([
#     [545.65,130.84,34.964,9.5897,4.3885,4.3805,4.3782],
#     [292.90,74.779,20.754,8.5260,4.2963,4.2640,4.2795],
#     [208.77,54.375,15.611,6.2675,5.4777,5.4899,5.4928],
#     [178.27,46.853,17.829,10.421,9.0184,9.1179,8.9764],
#     [178.64,52.719,17.697,20.575,17.682,17.668,17.607],
#     [194.27,76.528,38.469,44.717,38.425,38.430,38.285]])

# Efficiency
raw = [[0.106683,1.06465,10.65,106.262],[0.00113958,0.0107015,0.105587,1.05693],[1.948e-05,0.000113681,0.00118425,0.0109533],[5.98e-06,1.0171e-05,2.4121e-05,0.000166331]]
im2col = [[0.433335,1.72476,14.6043,143.039],[0.00434069,0.0171262,0.144317,1.41024],[5.8841e-05,0.000176301,0.00143628,0.0133835],[3.26e-06,4.89e-06,2.561e-05,0.000133081]]
im2col_opt = [[1.29748,2.61392,15.5723,145.168],[0.0126538,0.0253811,0.152222,1.42054],[0.000123421,0.000248611,0.00147733,0.0133805],[3.28e-06,3.89e-06,1.541e-05,0.000132571]]
read_omp = [[0.292475,0.302801,0.41996,1.59643],[0.00663951,0.00655099,0.00790006,0.0540453],[0.00330731,0.0032719,0.00335315,0.00364444],[0.00310556,0.00322729,0.00327068,0.00327958]]
write_omp = [[1.15221,1.15985,1.30826,2.45453],[0.0145,0.0149719,0.0159366,0.0256153],[0.00314772,0.00335992,0.00340383,0.00355286],[0.00188331,0.00321881,0.00297655,0.00305075]]



im_s = []
im_op_s = []
read_omp_s = []
write_omp_s = []

for i in range(4):
    temp1 = []
    temp2 = []
    temp3 = []
    temp4 = []
    for j in range(4):
        temp1.append(float("{:.2f}".format(raw[i][j] / im2col[i][j])))
        temp2.append(float("{:.2f}".format(raw[i][j] / im2col_opt[i][j])))
        temp3.append(float("{:.2f}".format(raw[i][j] / write_omp[i][j])))
        temp4.append(float("{:.2f}".format(raw[i][j] / read_omp[i][j])))
    im_s.append(temp1)
    im_op_s.append(temp2)
    write_omp_s.append(temp3)
    read_omp_s.append(temp4)

plt.figure(1)
fig, ax = plt.subplots()
im = ax.imshow(np.array(im_s), cmap="coolwarm")

# We want to show all ticks...
ax.set_xticks(np.arange(len(filters)))
ax.set_yticks(np.arange(len(channel_dimension)))
# ... and label them with the respective list entries
ax.set_xticklabels(filters)
ax.set_yticklabels(channel_dimension)

# Rotate the tick labels and set their alignment.
plt.setp(ax.get_xticklabels(), rotation=45, ha="right",
         rotation_mode="anchor")

# Loop over data dimensions and create text annotations.
for i in range(len(channel_dimension)): # y axis
    for j in range(len(filters)): # x axis
        text = ax.text(j, i, np.array(im_s)[i, j], ha="center", va="center", color="k")

ax.set_title("Speedup: im2col")
ax.set_ylabel('Number of filters')
ax.set_xlabel('Channel dimension')
fig.colorbar(im, ax=ax)
fig.tight_layout()


plt.figure(2)
fig, ax = plt.subplots()
im = ax.imshow(np.array(im_op_s), cmap="coolwarm")

# We want to show all ticks...
ax.set_xticks(np.arange(len(filters)))
ax.set_yticks(np.arange(len(channel_dimension)))
# ... and label them with the respective list entries
ax.set_xticklabels(filters)
ax.set_yticklabels(channel_dimension)

# Rotate the tick labels and set their alignment.
plt.setp(ax.get_xticklabels(), rotation=45, ha="right",
         rotation_mode="anchor")

# Loop over data dimensions and create text annotations.
for i in range(len(channel_dimension)): # y axis
    for j in range(len(filters)): # x axis
        text = ax.text(j, i, np.array(im_op_s)[i, j], ha="center", va="center", color="k")

ax.set_title("Speedup: im2col(optimized for write)")
ax.set_ylabel('Number of filters')
ax.set_xlabel('Channel dimension')
fig.colorbar(im, ax=ax)
fig.tight_layout()


plt.figure(3)
fig, ax = plt.subplots()
im = ax.imshow(np.array(write_omp_s), cmap="coolwarm")

# We want to show all ticks...
ax.set_xticks(np.arange(len(filters)))
ax.set_yticks(np.arange(len(channel_dimension)))
# ... and label them with the respective list entries
ax.set_xticklabels(filters)
ax.set_yticklabels(channel_dimension)

# Rotate the tick labels and set their alignment.
plt.setp(ax.get_xticklabels(), rotation=45, ha="right",
         rotation_mode="anchor")

# Loop over data dimensions and create text annotations.
for i in range(len(channel_dimension)): # y axis
    for j in range(len(filters)): # x axis
        text = ax.text(j, i, np.array(write_omp_s)[i, j], ha="center", va="center", color="k")

ax.set_title("Speedup: im2col(optimize for write) + GEMM(OMP)")
ax.set_ylabel('Number of filters')
ax.set_xlabel('Channel dimension')
fig.colorbar(im, ax=ax)
fig.tight_layout()


plt.figure(4)
fig, ax = plt.subplots()
im = ax.imshow(np.array(read_omp_s), cmap="coolwarm")

# We want to show all ticks...
ax.set_xticks(np.arange(len(filters)))
ax.set_yticks(np.arange(len(channel_dimension)))
# ... and label them with the respective list entries
ax.set_xticklabels(filters)
ax.set_yticklabels(channel_dimension)

# Rotate the tick labels and set their alignment.
plt.setp(ax.get_xticklabels(), rotation=45, ha="right",
         rotation_mode="anchor")

# Loop over data dimensions and create text annotations.
for i in range(len(channel_dimension)): # y axis
    for j in range(len(filters)): # x axis
        text = ax.text(j, i, np.array(read_omp_s)[i, j], ha="center", va="center", color="k")

ax.set_title("Speedup: im2col(optimize for read) + GEMM(OMP)")
ax.set_ylabel('Number of filters')
ax.set_xlabel('Channel dimension')
fig.colorbar(im, ax=ax)
fig.tight_layout()


plt.show()
