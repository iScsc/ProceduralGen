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

def colorize(value, seaLevel=0):
    i = int((value + 1) * 255/2)
    
    if value <= seaLevel:
        return (50, 50, i)
    else:
        return (i, i, i)

def setWaterLevel(map, seaLevel=0):
    size = ((len(map) - 1), (len(map[0]) - 1))
    I, J = size
    
    seaMap = [[max(map[i][j], seaLevel) for j in range(J)] for i in range(I)]
    
    colorMap = [[colorize(map[i][j], seaLevel) for j in range(J)] for i in range(I)]
    
    return seaMap, colorMap

SEED = rd.randint(0, 1000000000000)
print("SEED =", SEED)

rd.seed(SEED)

BASE_GRID_SIZE = 1 + 6
BASE_SIZE_FACTOR = 60
BASE_GRID = randomGradGrid2D([BASE_GRID_SIZE, BASE_GRID_SIZE])
baseMap = generateMap2D(BASE_GRID, BASE_SIZE_FACTOR)

SECOND_GRID_SIZE = 1 + 20
SECOND_SIZE_FACTOR = 18
SECOND_GRID = randomGradGrid2D((SECOND_GRID_SIZE, SECOND_GRID_SIZE))
secondMap = generateMap2D(SECOND_GRID, SECOND_SIZE_FACTOR)

SECOND_GRID_SIZEa = 1 + 15
SECOND_SIZE_FACTORa = 24
SECOND_GRIDa = randomGradGrid2D((SECOND_GRID_SIZEa, SECOND_GRID_SIZEa))
secondMapa = generateMap2D(SECOND_GRIDa, SECOND_SIZE_FACTORa)

SECOND_GRID_SIZEb = 1 + 10
SECOND_SIZE_FACTORb = 36
SECOND_GRIDb = randomGradGrid2D((SECOND_GRID_SIZEb, SECOND_GRID_SIZEb))
secondMapb = generateMap2D(SECOND_GRIDb, SECOND_SIZE_FACTORb)

BASE_VALUE_FACTOR = .8
SECOND_VALUE_FACTOR = .2

SEA_LEVEL = -0.15

map1 = BASE_VALUE_FACTOR * np.array(baseMap) + SECOND_VALUE_FACTOR * np.array(secondMap)

map1a = BASE_VALUE_FACTOR * np.array(baseMap) + SECOND_VALUE_FACTOR * np.array(secondMapa)

map1b = BASE_VALUE_FACTOR * np.array(baseMap) + SECOND_VALUE_FACTOR * np.array(secondMapb)

enhmap1, colormap1 = setWaterLevel(map1, SEA_LEVEL)

enhmap1a, colormap1a = setWaterLevel(map1a, SEA_LEVEL)

enhmap1b, colormap1b = setWaterLevel(map1b, SEA_LEVEL)


fig, axs = plt.subplots(1, 3)
fig.suptitle("SEED = " + str(SEED) + " | BASE_GRID_SIZE x BASE_SIZE_FACTOR = " + str((BASE_GRID_SIZE, BASE_SIZE_FACTOR)) + " | SEA_LEVEL = " + str(SEA_LEVEL) + " | BASE_VALUE_FACTOR = " + str(BASE_VALUE_FACTOR) + " | SECOND_VALUE_FACTOR = " + str(SECOND_VALUE_FACTOR))

axs[0].imshow(colormap1)
axs[0].set_title("SECOND_GRID_SIZE x SECOND_SIZE_FACTOR = " + str((SECOND_GRID_SIZE, SECOND_SIZE_FACTOR)))

axs[1].imshow(colormap1a)
axs[1].set_title("SECOND_GRID_SIZE x SECOND_SIZE_FACTOR = " + str((SECOND_GRID_SIZEa, SECOND_SIZE_FACTORa)))

axs[2].imshow(colormap1b)
axs[2].set_title("SECOND_GRID_SIZE x SECOND_SIZE_FACTOR = " + str((SECOND_GRID_SIZEb, SECOND_SIZE_FACTORb)))
plt.show()
