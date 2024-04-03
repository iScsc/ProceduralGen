# --- Imports
from mapGenerator import *

# --- Code

SEED = mapGenerator.getRandSeed()
print("SEED =", SEED)

mapGenerator.setSeed(SEED)


GRID_SIZE = 5 #20
MAP_FACTOR = 1

WATER_LEVEL = -0.15

#Trying to generate two maps

# Generate First map

#main_map, main_water_map, main_color_map = mapGenerator.fullGen(GRID_SIZE, MAP_FACTOR, WATER_LEVEL, display_map=False)
mainGrid = np.array(mapGenerator.randomGradGrid2D((GRID_SIZE + 1, GRID_SIZE + 1)))
mainMap = np.array(mapGenerator.generateMap2D(mainGrid))


# Generate Second map
secondGrid = np.array(mapGenerator.randomGradGrid2D((GRID_SIZE + 1, GRID_SIZE + 1)))
secondGrid[0, :] = mainGrid[-1, :]

secondMap = np.array(mapGenerator.generateMap2D(secondGrid))
secondMap[0, :] = mainMap[-1, :]

fig, axs = plt.subplots(nrows=1, ncols=3, figsize=(20, 10))
ax1, ax2, ax3 = axs

ax1.imshow(mainMap, cmap="gray")
ax2.imshow(secondMap, cmap="gray")

completeMap = np.concatenate([mainMap, secondMap], axis=0)
ax3.imshow(completeMap, cmap="gray")

plt.show(block=False)

input("Press [Enter] to exit.")
