# --- Imports
from mapGenerator import *

# --- Code

SEED = mapGenerator.getRandSeed()
print("SEED =", SEED)

mapGenerator.setSeed(SEED)


GRID_SIZES = [2, 3, 5]
MAP_FACTORS = [1, .3, .1]

# number of rows, number of columns
MAP_SIZE = (4, 7)

WATER_LEVEL = -0.15





global_grid, global_map = mapGenerator.get2DMap(GRID_SIZES, MAP_FACTORS, MAP_SIZE, False)

mainMap, secondMap = global_map[0][0], global_map[1][0]

fig = plt.figure(figsize=(20, 10))

ax1 = plt.subplot2grid((MAP_SIZE[0] + 2, MAP_SIZE[1] + 2), (0, 0), rowspan=1, colspan=1)
ax2 = plt.subplot2grid((MAP_SIZE[0] + 2, MAP_SIZE[1] + 2), (2, 0), rowspan=1, colspan=1)

ax3 = plt.subplot2grid((MAP_SIZE[0] + 2, MAP_SIZE[1] + 2), (0, 2), rowspan=MAP_SIZE[0], colspan=MAP_SIZE[1])

ax1.imshow(mainMap, cmap="gray")
ax2.imshow(secondMap, cmap="gray")

completeMap = np.concatenate([np.concatenate(global_map[i], axis=1) for i in range(len(global_map))], axis=0)
ax3.imshow(completeMap, cmap="gray")

plt.show(block=False)

input("Press [Enter] to exit.")
