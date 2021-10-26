import numpy as np
import matplotlib
import matplotlib.pyplot as plt

threads_per_block = ['32', '64', '128', '256', '512', '1024'] # y axis, 6 of them
thread_blocks = ["1", "4", "16", "64", "256", "1024", "4096"] # x axis, 7 of them

# # RUNTIME efficiency
# runtime = np.array([
#     [545.65,130.84,34.964,9.5897,4.3885,4.3805,4.3782],
#     [292.90,74.779,20.754,8.5260,4.2963,4.2640,4.2795],
#     [208.77,54.375,15.611,6.2675,5.4777,5.4899,5.4928],
#     [178.27,46.853,17.829,10.421,9.0184,9.1179,8.9764],
#     [178.64,52.719,17.697,20.575,17.682,17.668,17.607],
#     [194.27,76.528,38.469,44.717,38.425,38.430,38.285]])

# Efficiency
runtime = np.array([
    [1.25,4.92,18.45,67.47,87.73,87.88,88.07],
    [1.25,4.87,17.47,49.51,87.36,87.26,86.77],
    [1.25,4.80,16.73,49.32,49.63,49.38,49.63],
    [1.25,4.76,12.65,24.97,25.13,24.87,24.85],
    [1.25,4.22,12.68,12.67,12.68,12.68,12.70],
    [1.25,3.17,6.33,6.33,6.34,6.34,6.35]])


fig, ax = plt.subplots()
im = ax.imshow(runtime, cmap="coolwarm")

# We want to show all ticks...
ax.set_xticks(np.arange(len(thread_blocks)))
ax.set_yticks(np.arange(len(threads_per_block)))
# ... and label them with the respective list entries
ax.set_xticklabels(thread_blocks)
ax.set_yticklabels(threads_per_block)

# Rotate the tick labels and set their alignment.
plt.setp(ax.get_xticklabels(), rotation=45, ha="right",
         rotation_mode="anchor")

# Loop over data dimensions and create text annotations.
for i in range(len(threads_per_block)): # y axis
    for j in range(len(thread_blocks)): # x axis
        text = ax.text(j, i, runtime[i, j],
                       ha="center", va="center", color="k")

ax.set_title("SM_efficiency on Cori at Varying Block Size and Number of Blocks")
ax.set_ylabel('Threads per block')
ax.set_xlabel('Block Sizes')
fig.colorbar(im, ax=ax)
fig.tight_layout()
plt.show()
