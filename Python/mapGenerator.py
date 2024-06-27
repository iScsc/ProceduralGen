
#? To authorize type hints of class itself (ie Map here)
#? answer found here : https://stackoverflow.com/questions/33533148/how-do-i-type-hint-a-method-with-the-type-of-the-enclosing-class
#from typing import Self                    #? Python 3.11+
from __future__ import annotations          #? Python 3.7+

import numpy as np
import matplotlib.pyplot as plt
from gradientGrid import GradientGrid
from layer import Layer
from chunk import Chunk
from map import Map


class CompleteMap:
    """
    CompleteMap :
    -------------
    The CompleteMap class made out of a Map for enhanced processing.
    """
    
    
    #? -------------------------- Static ------------------------- #
    
    @staticmethod
    def colorize(value: float, sea_level: float, min_value: float, max_value: float) -> tuple[float]:
        """Returns the color associated with the given value for the given parameters. The color is a
        tuple of 3 floats between `0.` and `1.`.

        Args:
            `value` (float): altitude value to be colorized.
            `sea_level` (float): altitude of the sea. Below this point, an ocean color will be applied. Above, a land color will be applied.
            `min_value` (float): The minimum altitude value of the whole map. Used to determine the brightness of the color.
            `max_value` (float): The maximum altitude value of the whole map. Used to determine the brightness of the color.

        Returns:
            tuple[int]: the color associated to the given altitude value
        
        Note:
            Returns a red color `(1., 0., 0.)` if the altitude value is exactly 0.. Used to spot chunks.
        """
        
        #? Show red dots on zero values. Could be removed.
        if value == 0:
            return (1., 0., 0.)
        elif value <= sea_level:
            blue = int((value - min_value) * 200. / (sea_level - min_value)) / 255
            o = 50 / 255
            return (o, o, blue)
        else:
            green = int((value - min_value) * 255. / (max_value - min_value)) / 255
            o = 50 / 255
            return (o, green, o)
    
    
    
    
    
    @staticmethod
    def generateCompleteMapFromScratch(grid_widths: list[int], grid_heights: list[int], size_factors: list[int], layers_factors: list[float],
                                        map_width: int, map_height: int, sea_level: float) -> CompleteMap:
        """Generates a new Map structure and every required pre-structure based on the given parameters.

        Args:
            `grid_widths` (list[int]): the list of width to be used to generate the GradientGrids of each Chunk.
            `grid_heights` (list[int]): the list of height to be used to generate the GradientGrids of each Chunk.
            `size_factors` (list[int]): the list of size factors to be used to generate the Layers of each Chunk.
            `layers_factors` (list[float]): the list of layer factors to be used to generate each Chunk.
            `map_width` (int): the width of the map in chunks.
            `map_height` (int): the height of the map in chunks.
            `sea_level` (float): the altitude of the sea in the final CompleteMap.

        Returns:
            CompleteMap: the newly generated CompleteMap structure.
        
        Note:
            Every list should have the same length, which is the number of layers per Chunk.
            The sum of layer factors should not be 0.
            The given gradient grid dimensions and size factors should match so that the product `grid_dim * size_factor` is a constant Vector2D.
        """
        
        #* -------------- Verifications : Begin --------------
        
        #TODO
        
        #* -------------- Verifications : End   --------------
        
        
        
        new_map = Map.generateMapFromScratch(grid_widths=grid_widths, grid_heights=grid_heights, size_factors=size_factors, layers_factors=layers_factors,
                                             map_width=map_width, map_height=map_height)
        
        return CompleteMap(new_map, sea_level)
    
    
    
    @staticmethod
    def write(path: str, complete_map: CompleteMap) -> None:
        #TODO
        pass
    
    
    
    @staticmethod
    def read(path: str) -> CompleteMap:
        #TODO
        return None
    
    
    #? ------------------------ Instances ------------------------ #
    
    def __init__(self, map: Map, sea_level: float) -> None:
        """Initialize a CompleteMap structure based on the given Map and sea altitude.

        Args:
            map (Map): the map structure to build the complete map from.
            sea_level (float): the altitude of the sea.
        """
        
        self.map = None
        self.sea_level = 0
        
        self.sea_values = None
        self.color_map = None
        
        if type(map) != Map or float(sea_level) != sea_level:
            return
        
        self.map = map
        self.sea_level = sea_level
        
        if type(map.altitude) == np.ndarray:
            alt = map.altitude
        else:
            alt = np.array(map.altitude)
        self.sea_values = np.where(alt >= sea_level, alt, sea_level)
        
        self.generateColorMap()
    
    
    
    def generateColorMap(self) -> None:
        """Initialize and generate the color_map array of this complete map object.
        """
        
        width = self.map.map_width * self.map.chunk_width
        height = self.map.map_height * self.map.chunk_height
        
        if type(self.map.altitude) == np.ndarray:
            alt = self.map.altitude
        else:
            alt = np.array(self.map.altitude)
        
        max_alt = np.max(alt)
        min_alt = np.min(alt)
        
        self.color_map = [[(0, 0, 0) for j in range(width)] for i in range(height)]
        
        for i in range(height):
            for j in range(width):
                self.color_map[i][j] = CompleteMap.colorize(alt[i][j], self.sea_level, min_alt, max_alt)
        
        
        
        


class MapGenerator:
    """
    MapGenerator :
    -------------
    The static MapGenerator class used to process Maps, generate CompleteMaps, and display.
    """
    
    
    #? -------------------------- Static ------------------------- #
    
    @staticmethod
    def gcd(a: int, b: int) -> int:
        pass
    
    
    @staticmethod
    def lcm(a: int, b: int) -> int:
        pass
    
    
    @staticmethod
    def lcmOfArray(array: list[int]) -> int:
        pass
    
    
    
    
    
    @staticmethod
    def get2DMap(grid_sizes: list[int], layers_factors: list[float], map_width: int, map_height: int) -> Map:
        return None
    
    
    
    @staticmethod
    def fullGen(grid_sizes: list[int], layers_factors: list[float], map_width: int, map_height: int, sea_level: float) -> CompleteMap:
        return None
    
    
    
    @staticmethod
    def plotCompleteMap(complete_map: CompleteMap) -> None:
        
        #? --- Get data ---
        
        grid_sizes = []
        
        first_chunk: Chunk = complete_map.map.chunks[0][0]
        layer_factors = []
        
        layers_list: list[Layer] = first_chunk.layers
        for layer in layers_list:
            gsize = (layer.grid.width, layer.grid.height)
            
            if gsize[0] == gsize[1]:
                grid_sizes.append(gsize[0])
            else:
                grid_sizes.append(gsize)
        
        layer_factors = first_chunk.layers_factors
        
        
        map_size = (complete_map.map.map_width, complete_map.map.map_height)
        
        chunk_size = (complete_map.map.chunk_width, complete_map.map.chunk_height)
        
        #? --- Plotting ---
        
        fig = plt.figure(figsize=(20, 10))
        fig.suptitle("SEED = " + str(GradientGrid.CURRENT_SEED) + "\nCompleteMap of sizes = " + str(map_size) \
            + "\nGrid sizes = " + str(grid_sizes) + " | Noise factors = " + str(layer_factors) \
            + " | Sea Level = " + str(complete_map.sea_level))
        
        # 2D Map
        ax2D = fig.add_subplot(1, 2, 1)
        ax2D.imshow(complete_map.color_map)
        
        # 3D Map
        ax3D = fig.add_subplot(1, 2, 2, projection='3d')
        
        x = np.linspace(0, map_size[0], chunk_size[0] * map_size[0])
        y = np.linspace(0, map_size[1], chunk_size[1] * map_size[1])
        x, y = np.meshgrid(x, y)
        
        # print(np.shape(y), np.shape(x), np.shape(complete_map.sea_values), np.shape(complete_map.color_map))
        surf = ax3D.plot_surface(y, x, np.array(complete_map.sea_values), facecolors=np.array(complete_map.color_map))
        
        xylim = max(map_size[0], map_size[1])
        ax3D.set_xlim(0, xylim)
        ax3D.set_ylim(0, xylim)
        
        zlim = min(map_size[0], map_size[1])
        ax3D.set_zlim(-zlim, zlim)
        
        plt.show(block=False)
        
        input("Press [Enter] to quit.")
    
    
    #? ------------------------ Instances ------------------------ #





# --------------------------------- # Testing MapGenerator # --------------------------------- #

if __name__ == "__main__":
    print("# --------------------------------- # Testing MapGenerator # --------------------------------- #\n")
    
    GradientGrid.setRandomSeed("seed")
    
    grid_dim = [4, 6]
    size_factors = [5, 3]
    layer_factors = [1, .1]
    
    
    map_dimensions = (2, 3)
    
    print("Generating a new map from scratch with parameters : ")
    print(" - map dimensions : ", map_dimensions)
    print(" - grids square dimensions : ", grid_dim)
    print(" - layers size_factors : ", size_factors)
    print(" - chunk layer factors : ", layer_factors)
    
    map = Map.generateMapFromScratch(grid_dim, grid_dim, size_factors, layer_factors, map_dimensions[0], map_dimensions[1])
    
    
    sea_level=0
    
    complete_map = CompleteMap(map, sea_level)
    
    
    MapGenerator.plotCompleteMap(complete_map)