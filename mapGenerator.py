import random as rd
import numpy as np
import matplotlib.pyplot as plt
from math import ceil,floor

import os
import time

#--------------------------------# useful functions and constants #--------------------------------#

GREEN_COLOR = "\033[92m"
RED_COLOR = "\033[91m"
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
    """returns the Lowest Common Multiple of the given list.

    Args:
        l (list[int]): list of integers

    Returns:
        ppcm (int): the lcm of the list
    """
    
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



def loading_bar(value, max_value, number_of_segments, pre_text = "", end_text=""):
    """Generate a loading bar with the given parameters and return the corresponding str.

    Args:
        value (int): current value of the loading bar.
        max_value (int): maximum value that can be taken in the loading bar.
        number_of_segments (int): number of segments composing the loading bar.
        pre_text (str, optional): text to be printed before the loading bar. Defaults to "".
        end_text (str, optional): text to be printed after the loading bar. Defaults to "".

    Returns:
        str: loading str to be printed.
    """
    
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
    
    return pre_text + loading_str + end_text


#--------------------------------# Map Generator Class #--------------------------------#

class mapGenerator:
    
    MAX_SEED = 1000000000000
    CURRENT_SEED = "a simple seed"
    
    EPSILON = 5e-2
    
    NUMBER_OF_SEGMENTS = 21
    
    #--------------------------------# class global methods #--------------------------------#
    
    def isSeedValid(seed):
        """Returns whether the given seed if correct or not.

        Args:
            seed (int | float | str | bytes | bytearray): the given seed to be checked.

        Returns:
            bool: whether the seed is correct or not.
        """
        
        if seed == None or type(seed) in [int, float, str, bytes, bytearray]:
            return True
        
        return False
    
    def getRandSeed():
        """Return a random valid seed.

        Returns:
            int: a valid integer seed.
        """
        
        return rd.randint(0, mapGenerator.MAX_SEED)
    
    def setSeed(seed = "a simple seed"):
        """Set the random seed to the given seed if it is correct.

        Args:
            seed (__valid_type__, optional): Given seed to be set. Defaults to "a simple seed".
        """
        
        if mapGenerator.isSeedValid(seed):
            mapGenerator.CURRENT_SEED = seed
            rd.seed(seed)
    
    def getSeed():
        """Get and returns the current seed used.

        Returns:
            __valid_type__: returns the seed used for random generation.
        """
        return mapGenerator.CURRENT_SEED
    
    def smoothstep(w):
        """Smoothstep function to interpolate values.

        Args:
            w (float): parameter to be used for the smoothstep.

        Returns:
            float: smoothstep value.
        """
        if w > 1:
            w = 1
        elif w < 0:
            w = 0
        return w * w * (3.0 - 2.0 * w)
    
    def interpolate(a0, a1, w):
        """Interpolate value between a0 and a1 with smoothstep parameter w.

        Args:
            a0 (float): first interpolate value.
            a1 (float): second interpolate value.
            w (float): smoothstep parameter.

        Returns:
            float: the interpolated value.
        """
        return a0 + (a1 - a0) * mapGenerator.smoothstep(w)
        # return a0 + (a1 - a0) * smoothstep(w)
    
    
    
    
    
    def randomGradGrid2D(size : tuple[int], display_loading : bool = False, pre_text : str = "\r      Generating random gradient grid... "):
        """Generate the random gradient grid 2D with given size.

        Args:
            size (tuple[int]): size of the gradient grid to be generated (width x height).
            display_loading (bool, optional): whether the loading bar should be displayed or not. Defaults to False.
            pre_text (str, optional): Text to be written before the loading bar. Defaults to "\r      Generating random gradient grid... ".

        Returns:
            list[list[float]]: the generated gradient grid 2D of given size.
        """
        
        start_time = time.time()
        
        I, J = size
        
        grid2D = [[(0, 0) for j in range(J)] for i in range(I)]
        
        for i in range(I):
            for j in range(J):
                x = -1 + 2 * rd.random()
                y = (-1) ** rd.randint(0, 1) * np.sqrt(1 - x ** 2)
                
                grid2D[i][j] = (x, y)
                
                if display_loading:
                    t = time.time() - start_time
                    time_str = " | Time spent : {:.3f} s".format(t)
                    print(loading_bar(j + i * J, I * J - 1, mapGenerator.NUMBER_OF_SEGMENTS, pre_text=pre_text, end_text=time_str), end="")
        
        return grid2D
    
    def dotGridGradient(ix, iy, x, y, grid):
        """Computes and returns the dot product between the vector and the differentiate vector.

        Args:
            ix (int): vector i index.
            iy (int): vector j index.
            x (float): vector x value.
            y (float): vector y value.
            grid (list[list[float]]): gradient grid.

        Returns:
            float: the computed dot product value
        """
        
        dx = x - ix
        dy = y - iy
        
        ix = int(ix)
        iy = int(iy)
        
        return dx * grid[ix][iy][0] + dy * grid[ix][iy][1]

    def perlin(x, y, grid):
        """computes the perlin value.

        Args:
            x (float): x value in gradient grid
            y (float): y value in gradient grid
            grid (list[list[float]]): gradient grid

        Returns:
            float: the perlin value
        """
        
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
        """generates a map 2d layer from the gradient grid and size factor.

        Args:
            grid (list[list[float]]): gradient grid
            sizeFactor (int, optional): size factor to determine the amount of points to be generated between each gradient point. Defaults to 10.
            display_loading (bool, optional): whether the loading bar should be displayed or not. Defaults to False.
            pre_text (str, optional): the text to be printed before the loading bar. Defaults to "\r      Generating map...                  ".

        Returns:
            list[list[float]]: map height values.
        """
        start_time = time.time()
        
        size = ((len(grid) - 1) * sizeFactor, (len(grid[0]) - 1) * sizeFactor)
        I, J = size
        
        map = [[0 for j in range(J)] for i in range(I)]
        
        for i in range(I):
            for j in range(J):
                map[i][j] = mapGenerator.perlin(i/sizeFactor, j/sizeFactor, grid)
                
                if display_loading:
                    t = time.time() - start_time
                    time_str = " | Time spent : {:.3f} s".format(t)
                    print(loading_bar(j + i * J, I * J - 1, mapGenerator.NUMBER_OF_SEGMENTS, pre_text=pre_text, end_text=time_str), end="")
        
        return map

    def colorize(value, seaLevel=0, min_value = -1, max_value = 1, isFloat = False):
        """affect color vectors to height values.

        Args:
            value (float): height value
            seaLevel (int, optional): sea level to be used for the colorization. Defaults to 0.
            min_value (int, optional): minimum possible height value. Defaults to -1.
            max_value (int, optional): maximum possible height value. Defaults to 1.
            isFloat (bool, optional): whether it should generate float colors (0 - 1) or int colors (0 - 255). Defaults to False.

        Returns:
            list[list[tuple[float]]]: the color map of color vectors (r, g, b)
        """
        scale = 1
        if isFloat:
            scale = 1/255
        
        i = int((value - min_value) * 255/(max_value - min_value)) if min_value!=max_value else 0
        s = int((value - min_value) * 200/(seaLevel - min_value)) if min_value!=max_value else 0
        
        if value < seaLevel:
            return (scale * 50, scale * 50, scale * s)
        elif value == 0: #abs(value -seaLevel) <= EPSILON:
            return (scale * 255, 0, 0)
        else:
            return (scale * 50, scale * i, scale * 50)

    def setWaterLevel(map, seaLevel = 0., isFloat = False, display_loading : bool = False, pre_text : str = "\r   Generating color map...  "):
        """set water level on map heights and returns both the sea map and the colorized map. Values lower than the sea level will be replaced with this value.

        Args:
            map (list[list[float]]): the original map of heights values.
            seaLevel (float, optional): level of the sea. Defaults to 0.
            isFloat (bool, optional): whether the color map should be using float values or int values. Defaults to False.
            display_loading (bool, optional): whether the loading bar should be displayed or not. Defaults to False.
            pre_text (str, optional): the text to be printed before the loading bar. Defaults to "\r   Generating color map...  ".

        Returns: seaMap, colorMap
            seaMap (list[list[float]]): the final sea map of heights values, where the minimum possible value is the sea level.
            colorMap (list[list[tuple[float]]]): the map of color vectors to print the map.
        """
        start_time = time.time()
        
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
                    t = time.time() - start_time
                    time_str = " | Time spent : {:.3f} s".format(t)
                    print(loading_bar(j + i * J, I * J - 1, mapGenerator.NUMBER_OF_SEGMENTS, pre_text=pre_text, end_text=time_str), end="")
        
        return seaMap, colorMap
    
    def interpolate2D(a1:float,a2:float,a3:float,a4:float,x:float,y:float):
        return a1+(a2-a1)*mapGenerator.smoothstep(x)+(a3-a1)*mapGenerator.smoothstep(y)+(a1+a4-a2-a3)*mapGenerator.smoothstep(x)*mapGenerator.smoothstep(y)
    
    def addMeanAltitude(map_size_in_chunks : tuple[int], chunk_size_in_points : tuple[int], map : list[list[float]], max_a : float = 1, min_a : float = -0.5):
        """Add a 'mean' altitude to all chunks in a map with smoothing in order to avoid cliffs at chunks' border

        Args:
            map_size_in_chunks (tuple[int]): the number of chunks by row and columns in the map
            chunk_size_in_points (tuple[int]): the number of points by row and columns in a chunk
            map (list[list[float]]): the map
            max_a (float, optional): max added altitude. Defaults to 1.
            min_a (float, optional): min adde altitude. Defaults to -0.5
            
        Returns:
            the updated map
        """
        res=[[0 for j in range(len(map[0]))]for i in range(len(map))]
        # generating mean altitudes for each chunk + borders (to simplify border conditions)
        altitude:list[list[float]]=[] #[[0,0,0,0],[0,1,0,0],[0,0,0,0]]
        for i in range(map_size_in_chunks[0]+2):
            altitude.append([])
            for j in range(map_size_in_chunks[1]+2):
                altitude[i].append((max_a-min_a)*rd.random()+min_a)
        for i in range(0,map_size_in_chunks[0]+1):
            for j in range(0,map_size_in_chunks[1]+1):
                a1=altitude[i][j]
                a2=altitude[i+1][j]
                a3=altitude[i][j+1]
                a4=altitude[i+1][j+1]
                for pi in range(chunk_size_in_points[0]):
                    for pj in range(chunk_size_in_points[1]):
                        if (i<map_size_in_chunks[0] or pi<chunk_size_in_points[0]*0.5) and (j<map_size_in_chunks[1] or pj<chunk_size_in_points[1]*0.5) and (i!=0 or pi>=(chunk_size_in_points[0]*0.5)) and (j!=0 or pj>=(chunk_size_in_points[1]*0.5)): 
                            x=pi/chunk_size_in_points[0]
                            y=pj/chunk_size_in_points[1]
                            alt=mapGenerator.interpolate2D(a1,a2,a3,a4,x,y)
                            # if (j==0 and pj<chunk_size_in_points[1]//2+3) : print(pi+floor((i-0.5)*chunk_size_in_points[0]),pj+floor((j-0.5)*chunk_size_in_points[1]),alt)
                            res[pi+floor((i-0.5)*chunk_size_in_points[0])][pj+floor((j-0.5)*chunk_size_in_points[1])]+=alt #TODO chunk_size_in_point not even
        return res
        
    
    
    #--------------------------------# class all-in methods #--------------------------------#
    
    
    
    
    
    def printMap(map_size_in_chunks : tuple[int], map_size_in_points : tuple[int], water_level : float, water_map : list[list[float]], color_map : list[list[float]]):
        """print the given water_map with the corresponding color_map

        Args:
            map_size_in_chunks (int): dimensions of the map in chunks. Should be (width, height)
            map_size_in_points (int): dimensions of the map in points. Should be (width, height)
            water_map (list[list[float]]): the water map to be printed.
            color_map (list[list[float]]): the color map used to print the colors.
        """
        
        fig = plt.figure(figsize=(20, 10))
        fig.suptitle("Map of size width = {} height = {} and water Level = {}".format(map_size_in_points[0], map_size_in_points[1], water_level))

        # 2D Map
        ax2D = fig.add_subplot(1, 2, 1)
        ax2D.imshow(color_map)

        # 3D Map
        ax3D = fig.add_subplot(1, 2, 2, projection='3d')

        x = np.linspace(0, map_size_in_chunks[0], map_size_in_points[0])
        y = np.linspace(0, map_size_in_chunks[1], map_size_in_points[1])
        x, y = np.meshgrid(x, y)

        print(np.shape(y), np.shape(x), np.shape(water_map), np.shape(color_map))
        surf = ax3D.plot_surface(y, x, np.array(water_map), facecolors=np.array(color_map))
        
        xylim = max(map_size_in_chunks[0], map_size_in_chunks[1])
        ax3D.set_xlim(0, xylim)
        ax3D.set_ylim(0, xylim)
        
        zlim = min(map_size_in_chunks[0], map_size_in_chunks[1])
        ax3D.set_zlim(-zlim, zlim)

        plt.show(block=False)
        input("Press [Enter] to quit.")
    
    
    
    
    
    def loadSeaMap(sea_map_path : str):
        """load the sea map from a formatted file.

        Args:
            sea_map_path (str): path to the formatted sea map file.

        Raises:
            FileNotFoundError: if no file was found at the given path.

        Returns:  sea_map, sea_level, map_width, map_height
            sea_map (list[list[float]]): the numpy array of the loaded sea map.
            width (int): the map_width_in_points value extracted
            height (int): the map_height_in_points value extracted
            sea_level (float): the sea_level value extracted
            map_width (int): the map_width_in_chunk value extracted
            map_height (int): the map_height_in_chunk value extracted
        """
        
        if os.path.isfile(sea_map_path):
            
            f = open(sea_map_path, "r")
            
            lines = f.readlines()
            
            f.close()
            
            # Verify lines exist
            if len(lines) < 6:
                print(RED_COLOR + "File is too short, can not be a correct formatted file!" + RED_COLOR)
                return
            
            # Description line
            if "Sea Map" not in lines[0]:
                print(RED_COLOR + "Sea map does not contain the correct first line" + DEFAULT_COLOR)
            
            # Parameters
            width_line = lines[1]
            height_line = lines[2]
            sea_level_line = lines[3]
            map_width_line = lines[4]
            map_height_line = lines[5]
            
            width_parts = width_line.split("=")
            height_parts = height_line.split("=")
            sea_parts = sea_level_line.split("=")
            map_width_parts = map_width_line.split("=")
            map_height_parts = map_height_line.split("=")
            
            try:
                width = int(width_parts[1])
            except:
                print(RED_COLOR + "Could not convert '{}' into an int for width parameter !".format(width_parts[1]) + DEFAULT_COLOR)
                return
            
            try:
                height = int(height_parts[1])
            except:
                print(RED_COLOR + "Could not convert '{}' into an int for height parameter !".format(height_parts[1]) + DEFAULT_COLOR)
                return
            
            try:
                sea_level = float(sea_parts[1])
            except:
                print(RED_COLOR + "Could not convert '{}' into a float for sea_level parameter !".format(sea_parts[1]) + DEFAULT_COLOR)
                return
            
            try:
                map_width = int(map_width_parts[1])
            except:
                print(RED_COLOR + "Could not convert '{}' into an int for map_width parameter !".format(map_width_parts[1]) + DEFAULT_COLOR)
                return
            
            try:
                map_height = int(map_height_parts[1])
            except:
                print(RED_COLOR + "Could not convert '{}' into an int for map_height parameter !".format(map_height_parts[1]) + DEFAULT_COLOR)
                return
            
            print("Sea map has dimensions width = {} and height = {}".format(width, height))
            
            if len(lines) < 6 + height:
                print(RED_COLOR + "File is too short, can not contain the {} lines for the values!".format(height) + RED_COLOR)
                return
            
            sea_map = [[None for j in range(width)] for i in range(height)]
            
            for i in range(height):
                line = lines[6 + i]
                
                parts = line.split("\t")
                
                if len(parts) < width:
                    print(RED_COLOR + "Line {} is too short, can not be contain the {} lines for the values!".format(3 + i, width) + RED_COLOR)
                    return
                
                for j in range(width):
                    try:
                        sea_map[i][j] = float(parts[j])
                    except:
                        print(RED_COLOR + "Could not convert '{}' into a float for value at indexes i={} j={} !".format(parts[j], i, j) + DEFAULT_COLOR)
                        return
            
            return np.array(sea_map), width, height, sea_level, map_width, map_height
        
        else:
            raise FileNotFoundError(RED_COLOR + "File '{}' was not found!".format(sea_map_path) + DEFAULT_COLOR)
    
    
    
    
    
    def loadColorMap(color_map_path : str):
        """load the color map from a formatted file path.

        Args:
            color_map_path (str): path to the color map formatted file.

        Raises:
            FileNotFoundError: is the file was not found at the given path.

        Returns:
            list[list[tuple[float]]]: map of the color vectors extracted from the formatted path.
        """
        
        if os.path.isfile(color_map_path):
            
            f = open(color_map_path, "r")
            
            lines = f.readlines()
            
            f.close()
            
            # Verify lines exist
            if len(lines) < 3:
                print(RED_COLOR + "File is too short, can not be a correct formatted file!" + RED_COLOR)
                return
            
            # Description line
            if "Color Float Map" not in lines[0]:
                print(RED_COLOR + "Color float map does not contain the correct first line" + DEFAULT_COLOR)
            
            # Parameters
            width_line = lines[1]
            height_line = lines[2]
            
            width_parts = width_line.split("=")
            height_parts = height_line.split("=")
            
            try:
                width = int(width_parts[1])
            except:
                print(RED_COLOR + "Could not convert '{}' into an int for width parameter !".format(width_parts[1]) + DEFAULT_COLOR)
                return
            
            try:
                height = int(height_parts[1])
            except:
                print(RED_COLOR + "Could not convert '{}' into an int for height parameter !".format(height_parts[1]) + DEFAULT_COLOR)
                return
            
            print("Color float map has dimensions width = {} and height = {}".format(width, height))
            
            if len(lines) < 3 + height:
                print(RED_COLOR + "File is too short, can not contain the {} lines for the color values!".format(height) + RED_COLOR)
                return
            
            color_map = [[None for j in range(width)] for i in range(height)]
            
            for i in range(height):
                line = lines[3 + i]
                
                parts = line.replace("(", "").replace(")", "").split("\t")
                
                if len(parts) < width:
                    print(RED_COLOR + "Line {} is too short, can not be contain the {} lines for the color values!".format(3 + i, width) + RED_COLOR)
                    return
                
                for j in range(width):
                    vector_parts = parts[j].split(",")
                    
                    try:
                        r = float(vector_parts[0])
                    except:
                        print(RED_COLOR + "Could not convert '{}' into a float for red value at indexes i={} j={} !".format(vector_parts[0], i, j) + DEFAULT_COLOR)
                        return
                    
                    try:
                        g = float(vector_parts[1])
                    except:
                        print(RED_COLOR + "Could not convert '{}' into a float for green value at indexes i={} j={} !".format(vector_parts[1], i, j) + DEFAULT_COLOR)
                        return
                    
                    try:
                        b = float(vector_parts[2])
                    except:
                        print(RED_COLOR + "Could not convert '{}' into a float for blue value at indexes i={} j={} !".format(vector_parts[2], i, j) + DEFAULT_COLOR)
                        return
                    
                    
                    
                    color_map[i][j] = (r, g, b)
            
            return np.array(color_map)
        
        else:
            raise FileNotFoundError(RED_COLOR + "File '{}' was not found!".format(color_map_path) + DEFAULT_COLOR)
    
    
    
    
    
    def loadCompleteMap(folder_path : str):
        """Load sea map and color map from a given formatted complete map folder.

        Args:
            folder_path (str): path to the formatted folder.

        Raises:
            FileNotFoundError: if the folder was not found at the given path.

        Returns: sea_map, color_map, sea_level, map_width_in_chunks, map_height_in_chunks
            sea_map (list[list[float]]): the extracted sea_map from the formatted directory.
            color_map (list[list[tuple[float]]]): the extracted color_map from the formatted directory.
            map_width_in_points (int): the map_width_in_points value extracted
            map_height_in_points (int): the map_height_in_points value extracted
            sea_level (float): the sea_level value extracted
            map_width_in_chunks (int): the map_width_in_chunk value extracted
            map_height_in_chunks (int): the map_height_in_chunk value extracted
        """
        
        if os.path.isdir(folder_path):
            sea_map_path = os.path.join(folder_path, "sea_map.txt")
            
            color_map_path = os.path.join(folder_path, "color_float_map.txt")
            
            sea_map, map_width_in_points, map_height_in_points, sea_level, map_width_in_chunks, map_height_in_chunks = mapGenerator.loadSeaMap(sea_map_path)
            
            color_map = mapGenerator.loadColorMap(color_map_path)
            
            return sea_map, color_map, map_width_in_points, map_height_in_points, sea_level, map_width_in_chunks, map_height_in_chunks
        
        else:
            raise FileNotFoundError(RED_COLOR + "Directory '{}' was not found!".format(folder_path) + DEFAULT_COLOR)
    
    
    
    
    
    def get2DMap(grid_sizes : int | list[int], map_factors : int | float | list[int] | list[float], map_size : tuple[int] = (1,1), display_loading : bool = True):
        """generate the 2D Map with the given parameters.

        Args:
            grid_sizes (int | list[int]): size(s) to generate the gradient grids from.
            map_factors (int | float | list[int] | list[float]): noise factors to be used to generate the final map.
            map_size (tuple[int], optional): global map sizes in chunks. Defaults to (1,1).
            display_loading (bool, optional): whether the generation should print loading bars or not. Defaults to True.

        Raises:
            ValueError: if a list is empty
            ValueError: if grid_sizes and map_factors does not have the same dimension

        Returns: global_grid, global_map
            global_grid (list[list[tuple[float]]]): the final grid layers to the global map.
            global_map (list[list[float]]): the final map composed from the multiple gradient grids and noise layers.
        """
        
        begin_2D_map_time = time.time()
        
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
            
            if display_loading:
                t = time.time() - begin_2D_map_time
                print("2D maps generation took {:.3f} s.".format(t))
            
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
                            
                            pre_text = "\r      | Generating gradient grid " + str(k + 1) + "/" + str(n) + " of size " + str(gsi + 1) + "...  "
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
                
                
                if display_loading:
                    t = time.time() - begin_2D_map_time
                    print("2D maps generation took {:.3f} s.".format(t))
                
                return global_grid, global_map
    
    
    
    
    
    
    def fullGen(grid_sizes : int | list[int], map_factors : int | float | list[int] | list[float], water_level : float,
                map_size : tuple[int] = (1,1), display_loading : bool = True, display_map : bool = False):
        """Make the full generation of the complete_map, the water_map and the color_map.

        Args:
            grid_sizes (int | list[int]): gradient grid sizes used for the generation.
            map_factors (int | float | list[int] | list[float]): noise factors used to add the different layers.
            water_level (float): water level used for the generation.
            map_size (tuple[int], optional): size of the final global map. In chunks. Defaults to (1,1).
            display_loading (bool, optional): whether the loading bars should be displayed or not. Defaults to True.
            display_map (bool, optional): whether the final map should be plotted or not. Defaults to False.

        Returns: complete_map, water_map, color_map
            complete_map (list[list[float]]): the final height map with no water applied.
            water_map (list[list[float]]): the final height map with water applied.
            color_map (list[list[float]]): the final color vectors map to print the map.
        """
        
        begin_gen_time = time.time()
        
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
        
        complete_map = mapGenerator.addMeanAltitude(map_size,(ppcm,ppcm),complete_map)
        
        if display_loading:
            print("\n- Applying water levels on maps... ")
        
        water_map, color_map = mapGenerator.setWaterLevel(complete_map, seaLevel=water_level, isFloat=True, display_loading=display_loading)
    
        if display_loading:
            print(GREEN_COLOR + " Success!" + DEFAULT_COLOR)
            
            t = time.time() - begin_gen_time
            print("Full generation took {:.3f} s.".format(t))
            
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

            print(np.shape(y), np.shape(x), np.shape(water_map), np.shape(color_map))
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
