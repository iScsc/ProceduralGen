
#? To authorize type hints of class itself (ie Map here)
#? answer found here : https://stackoverflow.com/questions/33533148/how-do-i-type-hint-a-method-with-the-type-of-the-enclosing-class
#from typing import Self                    #? Python 3.11+
from __future__ import annotations          #? Python 3.7+

import numpy as np
from Python.gradientGrid import GradientGrid
from Python.map import Map





class CompleteMap:
    """
    CompleteMap :
    -------------
    The CompleteMap class made out of a Map for enhanced processing.
    """
    
    
    #? -------------------------- Static ------------------------- #
    
    @staticmethod
    def colorize(value: float, sea_level: float, min_value: float, max_value: float) -> tuple[int]:
        """Returns the color associated with the given value for the given parameters. The color is a
        tuple of 3 integers between `0` and `255`.

        Args:
            `value` (float): altitude value to be colorized.
            `sea_level` (float): altitude of the sea. Below this point, an ocean color will be applied. Above, a land color will be applied.
            `min_value` (float): The minimum altitude value of the whole map. Used to determine the brightness of the color.
            `max_value` (float): The maximum altitude value of the whole map. Used to determine the brightness of the color.

        Returns:
            tuple[int]: the color associated to the given altitude value
        
        Note:
            Returns a red color `(255, 0, 0)` if the altitude value is exactly 0. Used to spot chunks.
        """
        
        #? Show red dots on zero values. Could be removed.
        if value == 0:
            return (255, 0, 0)
        elif value <= sea_level:
            blue: int = int((value - min_value) * 200. / (sea_level - min_value))
            return (50, 50, blue)
        else:
            green: int = int((value - min_value) * 255. / (max_value - min_value))
            return (50, green, 50)
    
    
    
    
    
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
        
        if type(map) != Map or type(sea_level) != float:
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
    
    
    #? ------------------------ Instances ------------------------ #





# --------------------------------- # Testing MapGenerator # --------------------------------- #

if __name__ == "__main__":
    print("# --------------------------------- # Testing MapGenerator # --------------------------------- #\n")
    
    GradientGrid.setRandomSeed("seed")