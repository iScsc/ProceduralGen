import random as rd
import numpy as np
import matplotlib.pyplot as plt

from old_mapGenerator import mapGenerator

def ppcm_of_list(l):
    ppcm = 1
    for i in l:
        ppcm = np.lcm(i,ppcm)
    return ppcm

SEED = rd.randint(0, 1000000000000)
# Interesting seeds :
# 909729265464
#
#
#
#
print("SEED =", SEED)

mapGenerator.setSeed(SEED)

GRID_SIZE = np.array([2, 15]) #np.array([5,8,10,15,20,36])
ppcm = ppcm_of_list(GRID_SIZE)
SIZE_FACTOR = np.array([int(ppcm/g) for g in GRID_SIZE])
N = len(GRID_SIZE)
GRIDS = [0] * N
MAPS = [0] * N

BASE_VALUE_FACTOR = np.array([.999, .95, .9]) #np.array([.9, .7, .6])
M = len(BASE_VALUE_FACTOR)
SECOND_VALUE_FACTOR = 1 - BASE_VALUE_FACTOR

SEA_LEVEL = -0.15

CMAPS = [[0] * N] * M
enhmap = [[0] * N] * M
colormap = [[0] * N] * M

fig, axs = plt.subplots(M, N, figsize =(20,10))

fig.suptitle("SEED = " + str(SEED) + " | BASE_GRID_SIZE x BASE_SIZE_FACTOR = " + str((GRID_SIZE[0], SIZE_FACTOR[0])) + " | SEA_LEVEL = " + str(SEA_LEVEL) )


for i in range(N):
    GRIDS[i] = mapGenerator.randomGradGrid2D([GRID_SIZE[i]+1, GRID_SIZE[i]+1])
    MAPS[i] = mapGenerator.generateMap2D(GRIDS[i], SIZE_FACTOR[i])
    for j in range(M):
        CMAPS[j][i] = BASE_VALUE_FACTOR[j] * np.array(MAPS[0]) + SECOND_VALUE_FACTOR[j] * np.array(MAPS[i])
        enhmap[j][i], colormap[j][i] = mapGenerator.setWaterLevel(CMAPS[j][i], SEA_LEVEL)
    
        if M==1:
            axs[i].imshow(colormap[j][i])
            if i != 0:
                axs[i].set_title("GRID_SIZE x SIZE_FACTOR = \n" + str((GRID_SIZE[i], SIZE_FACTOR[i])) + "\nBASE_VALUE_FACTOR =" + str(BASE_VALUE_FACTOR[j]))
            else :
                axs[i].set_title("BASE_GRID")
        else:
            axs[j,i].imshow(colormap[j][i])
            if i != 0:
                axs[j,i].set_title("GRID_SIZE x SIZE_FACTOR = \n" + str((GRID_SIZE[i], SIZE_FACTOR[i])) + "\nBASE_VALUE_FACTOR =" + str(BASE_VALUE_FACTOR[j]))
            else :
                axs[j,i].set_title("BASE_GRID")

fig3D, ax3D = plt.subplots(subplot_kw=dict(projection='3d'), figsize =(20,10))

x = np.linspace(0, 1, ppcm)
y = np.linspace(0, 1, ppcm)

x, y = np.meshgrid(x, y)

seamap, cmapfloat = mapGenerator.setWaterLevel(CMAPS[0][1], SEA_LEVEL, True)
surf = ax3D.plot_surface(x, y, np.array(seamap), facecolors=np.array(cmapfloat))
ax3D.set_zlim((-1, 1))

plt.show()
