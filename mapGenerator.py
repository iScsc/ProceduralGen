import random as rd
import numpy as np
import matplotlib.pyplot as plt


#--------------------------------# useful functions and constants #--------------------------------#

GREEN_COLOR = "\033[92m"
DEFAULT_COLOR = "\033[0m"

FULL_BLOCK_CHAR = u"\u2588"
LOWER_HALF_BLOCK_CHAR = u"\u2584"
SHORT_CENTERED_BAR_CHAR = u"\u2501"
HORIZONTAL_RECTANGLE_CHAR = u"\u25ac"

# https://en.wikipedia.org/wiki/Block_Elements
# https://en.wikipedia.org/wiki/Box_Drawing
# https://en.wikipedia.org/wiki/Geometric_Shapes_(Unicode_block)
# (general) https://en.wikipedia.org/wiki/Unicode_block

LIST_TYPES = [list, tuple, np.ndarray]

def ppcm_of_list(l):
    if type(l) not in LIST_TYPES:
        return l
    # try:
    #     len(l)
    # except:
    #     return l
    
    ppcm = 1
    for i in l:
        ppcm = np.lcm(i,ppcm)
    return ppcm



def loading_bar(value, max_value, number_of_segments, pre_text = ""):
    begin_char = "["
    end_char = "]"
    loading_char = HORIZONTAL_RECTANGLE_CHAR
    
    
    
    loading_str = begin_char
        
    value_per_segment = max_value/number_of_segments
    number_filled = value//value_per_segment
    
    if value == max_value:
        number_filled = number_of_segments
    
    for i in range(number_of_segments):
        if i < number_filled:
            loading_str += loading_char
        else:
            loading_str += " "
    
    percent = value/max_value * 100
    
    loading_str += end_char + "    {0:.2f}% completed".format(percent)
    
    return pre_text + loading_str


#--------------------------------# Map Generator Class #--------------------------------#

class mapGenerator:
    
    MAX_SEED = 1000000000000
    CURRENT_SEED = "a simple seed"
    
    EPSILON = 5e-2
    
    NUMBER_OF_SEGMENTS = 21
    
    #--------------------------------# class global methods #--------------------------------#
    
    def isSeedValid(seed):
        if seed == None or type(seed) in [int, float, str, bytes, bytearray]:
            return True
        
        return False
    
    def getRandSeed():
        return rd.randint(0, mapGenerator.MAX_SEED)
    
    def setSeed(seed = "a simple seed"):
        if mapGenerator.isSeedValid(seed):
            mapGenerator.CURRENT_SEED = seed
            rd.seed(seed)
    
    def getSeed():
        return mapGenerator.CURRENT_SEED
    
    def smoothstep(w):
        if w > 1:
            w = 1
        elif w < 0:
            w = 0
        return w * w * (3.0 - 2.0 * w)
    
    def interpolate(a0, a1, w):
        return a0 + (a1 - a0) * mapGenerator.smoothstep(w)
        # return a0 + (a1 - a0) * smoothstep(w)
    
    
    
    
    
    def randomGradGrid2D(size : tuple[int], display_loading : bool = False, pre_text : str = "\r      Generating random gradient grid... "):
        
        I, J = size
        
        grid2D = [[(0, 0) for j in range(J)] for i in range(I)]
        
        for i in range(I):
            for j in range(J):
                x = -1 + 2 * rd.random()
                y = (-1) ** rd.randint(0, 1) * np.sqrt(1 - x ** 2)
                
                grid2D[i][j] = (x, y)
                
                if display_loading:
                    print(loading_bar(j + i * J, I * J - 1, mapGenerator.NUMBER_OF_SEGMENTS, pre_text=pre_text), end="")
        
        return grid2D
    
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
        
        n0 = mapGenerator.dotGridGradient(x0, y0, x, y, grid)
        n1 = mapGenerator.dotGridGradient(x1, y0, x, y, grid)
        ix0 = mapGenerator.interpolate(n0, n1, sx)
        n0 = mapGenerator.dotGridGradient(x0, y1, x, y, grid)
        n1 = mapGenerator.dotGridGradient(x1, y1, x, y, grid)
        ix1 = mapGenerator.interpolate(n0, n1, sx)
        value = mapGenerator.interpolate(ix0, ix1, sy)

        return value

    def generateMap2D(grid, sizeFactor = 10, display_loading : bool = False, pre_text : str = "\r      Generating map...                  "):
        size = ((len(grid) - 1) * sizeFactor, (len(grid[0]) - 1) * sizeFactor)
        I, J = size
        
        map = [[0 for j in range(J)] for i in range(I)]
        
        for i in range(I):
            for j in range(J):
                map[i][j] = mapGenerator.perlin(i/sizeFactor, j/sizeFactor, grid)
                
                if display_loading:
                    print(loading_bar(j + i * J, I * J - 1, mapGenerator.NUMBER_OF_SEGMENTS, pre_text=pre_text), end="")
        
        return map

    def colorize(value, seaLevel=0, min_value = -1, max_value = 1, isFloat = False):
        scale = 1
        if isFloat:
            scale = 1/255
        
        i = int((value - min_value) * 255/(max_value - min_value))
        s = int((value - min_value) * 200/(seaLevel - min_value))
        
        if value < seaLevel:
            return (scale * 50, scale * 50, scale * s)
        elif value == 0: #abs(value -seaLevel) <= EPSILON:
            return (scale * 255, 0, 0)
        else:
            return (scale * 50, scale * i, scale * 50)

    def setWaterLevel(map, seaLevel = 0, isFloat = False, display_loading : bool = False, pre_text : str = "\r   Generating color map...  "):
        size = (len(map), len(map[0]))
        I, J = size
        
        seaMap = [[max(map[i][j], seaLevel) for j in range(J)] for i in range(I)]
        
        min_value = min([min(v) for v in map])
        max_value = max([max(v) for v in map])
        
        colorMap = [[0 for j in range(J)] for i in range(I)]
        
        for i in range(I):
            for j in range(J):
                colorMap[i][j] = mapGenerator.colorize(map[i][j], seaLevel, min_value, max_value, isFloat)
                
                if display_loading:
                    print(loading_bar(j + i * J, I * J - 1, mapGenerator.NUMBER_OF_SEGMENTS, pre_text=pre_text), end="")
        
        return seaMap, colorMap
    
    
    #--------------------------------# class all-in methods #--------------------------------#
    
    def get2DMap(grid_sizes : int | list[int], map_factors : int | float | list[int] | list[float], map_size : tuple[int] = (1,1), display_loading : bool = True):
        # if (type(grid_sizes), type(map_factors)) not in [(int, int), (int, float), (list[int], list[int]), (list[int], list[float]), (np.ndarray, np.ndarray)]:
        #     raise TypeError("The given parameters were not of the correct type! Their types were : " + str((type(grid_sizes), type(map_factors))))
        
        # simple values
        if (type(grid_sizes), type(map_factors)) in [(int, int), (int, float)]:
            
            I, J = map_size
            global_grid = [[None for j in range(J)] for i in range(I)]
            global_map = [[None for j in range(J)] for i in range(I)]
            
            if display_loading:
                print("- Generation of the gradient grids.")
            
            for i in range(I):
                for j in range(J):
                    if display_loading:
                        print("   Map " + str(i*J + j + 1) +"/" + str(I*J) + "...")
                    grid = np.array(mapGenerator.randomGradGrid2D((grid_sizes + 1, grid_sizes + 1), display_loading=display_loading))
                    
                    if i != 0:
                        grid[0, :] = global_grid[i - 1][j][-1, :]
                    
                    if j != 0:
                        grid[:, 0] = global_grid[i][j - 1][:, -1]
                    
                    global_grid[i][j] = np.copy(grid)
                    
                    if display_loading:
                        print(GREEN_COLOR + " Success!" + DEFAULT_COLOR)
            
            
            if display_loading:
                print("\n- Generation of the maps based on gradient grids.")
            
            for i in range(I):
                for j in range(J):
                    if display_loading:
                        print("   Map " + str(i*J + j + 1) +"/" + str(I*J) + "...")
                    final_map = np.array(mapGenerator.generateMap2D(global_grid[i][j], display_loading=display_loading))
                    
                    global_map[i][j] = final_map
                    
                    if display_loading:
                        print(GREEN_COLOR + " Success!" + DEFAULT_COLOR)
            
            if I == 1 and J == 1:
                global_grid = global_grid[0][0]
                global_map = global_map[0][0]
            
            return global_grid, global_map
        
        # lists or arrays
        else:
            if (len(grid_sizes) != len(map_factors)):
                raise ValueError("Both lists must be of the same dimension!")
            
            n = len(grid_sizes)
            
            if n == 0:
                raise ValueError("Empty list error!")
            # simple values
            elif n == 1:
                return mapGenerator.get2DMap(grid_sizes[0], map_factors[0])
            # real list
            elif n > 1:
                I, J = map_size
                ppcm = ppcm_of_list(grid_sizes)
                
                global_grid = [[None for j in range(J)] for i in range(I)]
                global_map = [[None for j in range(J)] for i in range(I)]
                
                
                if display_loading:
                    print("- Generation of the gradient grids.")
                
                for i in range(I):
                    for j in range(J):
                        if display_loading:
                            print("   Map " + str(i*J + j + 1) +"/" + str(I*J) + "...")
                
                        gradGrids = [None for k in range(n)]
                        
                        for k in range(n):
                            gsi = grid_sizes[k]
                            
                            pre_text = "\r      | Generating gradient grid " + str(k + 1) + "/" + str(n) + " of size " + str(gsi) + "...  "
                            gradGrids[k] = np.array(mapGenerator.randomGradGrid2D((gsi + 1, gsi + 1), display_loading=display_loading, pre_text=pre_text))
                            
                            if i != 0:
                                gradGrids[k][0, :] = global_grid[i - 1][j][k][-1, :]
                            
                            if j != 0:
                                gradGrids[k][:, 0] = global_grid[i][j - 1][k][:, -1]
                            
                            if display_loading:
                                print(GREEN_COLOR + " Success!" + DEFAULT_COLOR)
                        
                        global_grid[i][j] = gradGrids
                
                
                
                if display_loading:
                    print("\n- Generation of the maps based on gradient grids.")
                
                for i in range(I):
                    for j in range(J):
                        if display_loading:
                            print("   Map " + str(i*J + j + 1) +"/" + str(I*J) + "...")
                        
                        maps = [None for k in range(n)]
                        
                        for k in range(n):
                            gsi = grid_sizes[k]
                            
                            pre_text = "\r      | Generating map " + str(k + 1) + "/" + str(n) + "...  "
                            maps[k] = np.array(mapGenerator.generateMap2D(global_grid[i][j][k], ppcm//gsi, display_loading=display_loading, pre_text=pre_text))
                            if display_loading:
                                print(GREEN_COLOR + " Success!" + DEFAULT_COLOR)
                        
                        factor_tot = map_factors[0]
                        final_map = map_factors[0] * maps[0]

                        for k in range(1, n):
                            factor_tot += map_factors[k]
                            final_map += map_factors[k] * maps[k]
                        
                        final_map /= factor_tot
                        
                        global_map[i][j] = final_map
                
                
                if I == 1 and J == 1:
                    global_grid = global_grid[0][0]
                    global_map = global_map[0][0]
                
                return global_grid, global_map
    
    
    
    
    def fullGen(grid_sizes : int | list[int], map_factors : int | float | list[int] | list[float], water_level : float,
                map_size : tuple[int] = (1,1), display_loading : bool = True, display_map : bool = False):
        
        I, J = map_size
        
        n = 1
        ppcm = grid_sizes * 10
        if type(grid_sizes) in LIST_TYPES:
            n = len(grid_sizes)
            ppcm = ppcm_of_list(grid_sizes)
        
        print("ppcm = ", ppcm)
        
        if display_loading:
            print(" -------------------------------------------------- MAP GENERATION : BEGIN -------------------------------------------------- ")
            print("Generation of " + str(map_size[0] * map_size[1]) + " map(s) with " + str(n) + " layers each for a total of " \
                + str(map_size[0] * map_size[1] * n) + " maps... \n")
        
        _, global_map = mapGenerator.get2DMap(grid_sizes, map_factors, map_size=map_size, display_loading=display_loading)
        
        
        # Generating unique complete_map
        complete_map = global_map
        if I != 1 or J != 1:
            complete_map = np.concatenate([np.concatenate(global_map[i], axis=1) for i in range(len(global_map))], axis=0)
        
        
        if display_loading:
            print("\n- Applying water levels on maps... ")
        
        water_map, color_map = mapGenerator.setWaterLevel(complete_map, isFloat=True, display_loading=display_loading)
    
        if display_loading:
            print(GREEN_COLOR + " Success!" + DEFAULT_COLOR)
            
            print(" --------------------------------------------------- MAP GENERATION : END --------------------------------------------------- ")
        
        
        
        
        if display_map:
            fig = plt.figure(figsize=(20, 10))
            fig.suptitle("SEED = " + str(mapGenerator.getSeed()) + "\nGrid sizes = " + str(grid_sizes) + " | Noise factors = " + str(map_factors) \
                + " | Water Level = " + str(water_level))

            # 2D Map
            ax2D = fig.add_subplot(1, 2, 1)
            ax2D.imshow(color_map)

            # 3D Map
            ax3D = fig.add_subplot(1, 2, 2, projection='3d')

            x = np.linspace(0, map_size[1], ppcm * map_size[1])
            y = np.linspace(0, map_size[0], ppcm * map_size[0])
            x, y = np.meshgrid(x, y)

            surf = ax3D.plot_surface(y, x, np.array(water_map), facecolors=np.array(color_map))
            
            xylim = max(map_size[0], map_size[1])
            ax3D.set_xlim(0, xylim)
            ax3D.set_ylim(0, xylim)
            
            zlim = min(map_size[0], map_size[1])
            ax3D.set_zlim(-zlim, zlim)

            plt.show(block=False)
            
            input("Press [Enter] to quit.")
        
        
        return complete_map, water_map, color_map
    
    
    
    # #--------------------------------# class instance methods #--------------------------------#
    
    
    
    # def __init__(self, seed="not defined") -> None:
        
    #     if not mapGenerator.isSeedValid(seed) or seed == "not defined":
    #         seed = rd.randint(0, mapGenerator.MAX_SEED)
        
    #     self.seed = seed
    #     rd.seed(self.seed)
    
    # def resetSeed(self) -> None:
    #     if mapGenerator.isSeedValid(self.seed):
    #         rd.seed(self.seed)
    
    
    if __name__ == "__main__":
        import time
        
        N = 10
        
        I = N
        J = N
        
        for i in range(1, I):
            for j in range(1, J):
                print(loading_bar(j + i*J, I*J - 1, 20, pre_text="\r test : "), end="")
                
                time.sleep(.1)
        
        print()
