import random as rd
import numpy as np
import matplotlib.pyplot as plt

def randomGradGrid2D(size):
    
    I, J = size
    
    grid2D = [[(0, 0) for j in range(J)] for i in range(I)]
    
    for i in range(I):
        for j in range(J):
            x = -1 + 2 * rd.random()
            y = (-1) ** rd.randint(0, 1) * np.sqrt(1 - x ** 2)
            
            grid2D[i][j] = (x, y)
    
    return grid2D

def smoothstep(w):
    if w > 1:
        w = 1
    elif w < 0:
        w = 0
    return w * w * (3.0 - 2.0 * w)

def interpolate(a0, a1, w):
    return a0 + (a1 - a0) * smoothstep(w)
    # return a0 + (a1 - a0) * smoothstep(w)

def dotGridGradient(ix, iy, x, y, grid):    
    dx = x - ix
    dy = y - iy
    
    ix = int(ix)
    iy = int(iy)
    
    return dx * grid[ix][iy][0] + dy * grid[ix][iy][1]

def perlin(x, y, grid):
    
    x0 = np.floor(x)
    x1 = x0 + 1
    y0 = np.floor(y)
    y1 = y0 + 1
    
    sx = x - x0
    sy = y - y0
    
    n0 = dotGridGradient(x0, y0, x, y, grid)
    n1 = dotGridGradient(x1, y0, x, y, grid)
    ix0 = interpolate(n0, n1, sx)
    n0 = dotGridGradient(x0, y1, x, y, grid)
    n1 = dotGridGradient(x1, y1, x, y, grid)
    ix1 = interpolate(n0, n1, sx)
    value = interpolate(ix0, ix1, sy)

    return value

def generateMap2D(grid, sizeFactor = 10):
    size = ((len(grid) - 1) * sizeFactor, (len(grid[0]) - 1) * sizeFactor)
    I, J = size
    
    map = [[perlin(i/sizeFactor, j/sizeFactor, grid) for j in range(J)] for i in range(I)]
    
    return map

def colorize(value, seaLevel=0,min_value = -1, max_value = 1):
    i = 255-int((value - min_value) * 255/(max_value - min_value))
    s = int((value - min_value) * 200/(seaLevel - min_value))
    
    if value <= seaLevel:
        return (50, 50, s)
    else:
        return (i, i, i)

def setWaterLevel(map, seaLevel=0):
    size = ((len(map) - 1), (len(map[0]) - 1))
    I, J = size
    
    seaMap = [[max(map[i][j], seaLevel) for j in range(J)] for i in range(I)]
    
    min_value = min([ min(v) for v in map])
    max_value = max([ max(v) for v in map])
    
    colorMap = [[colorize(map[i][j],seaLevel, min_value,max_value) for j in range(J)] for i in range(I)]
    
    return seaMap, colorMap

def ppcm_of_list(l):
    ppcm = 1
    for i in l:
        ppcm = np.lcm(i,ppcm)
    return ppcm

SEED = rd.randint(0, 1000000000000)
print("SEED =", SEED)

rd.seed(SEED)


GRID_SIZE = np.array([5,8,10,15,20,36])
ppcm = ppcm_of_list(GRID_SIZE)
SIZE_FACTOR = np.array([int(ppcm/g) for g in GRID_SIZE])
N = len(GRID_SIZE)
GRIDS = [0] * N
MAPS = [0] * N

BASE_VALUE_FACTOR = np.array([.5,.8])
M = len(BASE_VALUE_FACTOR)
SECOND_VALUE_FACTOR = 1 - BASE_VALUE_FACTOR

SEA_LEVEL = -0.15

CMAPS = [[0] * N] * M
enhmap = [[0] * N] * M
colormap = [[0] * N] * M

fig, axs = plt.subplots(M, N, figsize =(20,10))
fig.suptitle("SEED = " + str(SEED) + " | BASE_GRID_SIZE x BASE_SIZE_FACTOR = " + str((GRID_SIZE[0], SIZE_FACTOR[0])) + " | SEA_LEVEL = " + str(SEA_LEVEL) )


for i in range(N):
    GRIDS[i] = randomGradGrid2D([GRID_SIZE[i]+1, GRID_SIZE[i]+1])
    MAPS[i] = generateMap2D(GRIDS[i], SIZE_FACTOR[i])
    for j in range(M):
        CMAPS[j][i] = BASE_VALUE_FACTOR[j] * np.array(MAPS[0]) + SECOND_VALUE_FACTOR[j] * np.array(MAPS[i])
        enhmap[j][i], colormap[j][i] = setWaterLevel(CMAPS[j][i], SEA_LEVEL) 
    
        axs[j,i].imshow(colormap[j][i])
        if i != 0:
            axs[j,i].set_title("GRID_SIZE x SIZE_FACTOR = \n" + str((GRID_SIZE[i], SIZE_FACTOR[i])) + "\nBASE_VALUE_FACTOR =" + str(BASE_VALUE_FACTOR[j]))
        else :
            axs[j,i].set_title("BASE_GRID")

plt.show()
