
#? To authorize type hints of class itself (ie GradientGrid here)
#? answer found here : https://stackoverflow.com/questions/33533148/how-do-i-type-hint-a-method-with-the-type-of-the-enclosing-class
#from typing import Self                    #? Python 3.11+
from __future__ import annotations          #? Python 3.7+

import numpy as np
import random as rd



class GradientGrid:
    """
    Description:
    ------------
    The GradientGrid class used to store the primary randomly generated vectors responsible for the whole Perlin generation
    afterwards.
    
    Note:
    -----
    In every matrix notation, the indexes `i` and `j` respectively refers to rows and columns, so height and width.
    Sizes are expressed in `width x height`. But Matrix are thus of dimensions `height x width`.
    """
    
    
    #? -------------------------- Static ------------------------- #
    
    
    MAX_INT_SEED = 1000000000000 # Maximum integer for the automatic random seed generation
    
    VEC_PRINTING_DECIMALS = 4
    VEC_PRINTING_FORMAT = " ({{: .{}f}}, {{: .{}f}}) ".format(VEC_PRINTING_DECIMALS, VEC_PRINTING_DECIMALS)
    
    
    
    @staticmethod
    def setRandomSeed(seed: int | float | str | bytes | bytearray | None = rd.randint(1, MAX_INT_SEED)) -> None:
        """Set the seed for the future random generations.

        Args:
            `seed` (int | float | str | bytes | bytearray | None): the seed to be used in the future random generations.
                                                                   Defaults to a random integer between `1` and `GradientGrid.MAX_INT_SEED`.
        """
        
        rd.seed(seed)
    
    
    
    @staticmethod
    def write(path: str) -> None:
        #TODO
        pass
    
    
    
    @staticmethod
    def read(path: str) -> GradientGrid:
        #TODO
        return None
    
    
    #? ------------------------ Instances ------------------------ #
    
    
    def __init__(self, width: int = 2, height: int = 2, adjacent_grids: tuple = (None, None, None, None)) -> None:
        """Initialize a new random GradientGrid instance with given width and height values.
        Should be of size `2 x 2` at minimum.
        A tuple of adjacent GradientGrids can additionally be provided to apply the corresponding boundary conditions with the following order :
        `(NORTH, EAST, SOUTH, WEST)`

        Args:
            `width` (int, optional): the width of the generated random GradientGrid. Defaults to `2`.
            `height` (int, optional): the height of the generated random GradientGrid. Defaults to `2`.
            `adjacent_grids` (tuple, optional): the tuple of adjacent GradientGrids to generate a smooth transition in terrain with correct
                                                boundary conditions.
                                                The tuple is in order `(NORTH, EAST, SOUTH, WEST)`.
                                                Set a grid to None if there are no neighbours in this direction.
                                                Defaults to `(None, None, None, None)`
        """
        
        self.width = width
        self.height = height
        self.vectors = np.array([[np.zeros((2)) for j in range(self.width)] for i in range(self.height)])
        self.regenerate(adjacent_grids)
    
    
    
    def __str__(self) -> str:
        """Returns the string representation of the GradientGrid.
        Used by the print function when called with a GradientGrid.

        Returns:
            `str`: the string representation of the GradientGrid.
        """
        final_str = "-----------------------------------------\n"
        final_str += "Gradient Grid of dimensions {} x {} :\n".format(self.width, self.height)
        for i in range(self.height):
            
            for j in range(self.width):
                vec = self.vectors[i, j]
                final_str += GradientGrid.VEC_PRINTING_FORMAT.format(vec[0], vec[1])
            
            final_str += "\n"
        final_str += "-----------------------------------------\n"
        
        return final_str
    
    
    
    def regenerate(self, adjacent_grids: tuple[GradientGrid] = (None, None, None, None)) -> None:
        """Regenerates the random vectors of the GradientGrid. A tuple of adjacent GradientGrids can be given
        in order to apply the correct boundary conditions with the following order : `(NORTH, EAST, SOUTH, WEST)`.

        Args:
            `adjacent_grids` (tuple, optional): the tuple of adjacent GradientGrids to generate a smooth transition in terrain with correct
                                                boundary conditions.
                                                The tuple is in order `(NORTH, EAST, SOUTH, WEST)`.
                                                Set a grid to None if there are no neighbours in this direction.
                                                Defaults to `(None, None, None, None)`
        """
        width = self.width
        height = self.height
        
        for i in range(height):
            for j in range(width):
                sign_x = 1 - 2*rd.randint(0, 1)
                sign_y = 1 - 2*rd.randint(0, 1)
                
                x = sign_x * rd.random()
                y = sign_y * np.sqrt(1 - x**2)
                
                self.vectors[i][j] = np.array((x, y))
        
        if len(adjacent_grids) >= 4:
            north_grid = adjacent_grids[0]
            east_grid  = adjacent_grids[1]
            south_grid = adjacent_grids[2]
            west_grid  = adjacent_grids[3]
            
            if type(north_grid) is GradientGrid:
                if north_grid.width == width:
                        self.vectors[0, :] = north_grid.vectors[-1, :]
                else:
                    print("ERROR : Invalid width for north grid")
                    pass
            
            if type(east_grid) is GradientGrid:
                if east_grid.height == height:
                        self.vectors[:, -1] = east_grid.vectors[:, 0]
                else:
                    print("ERROR : Invalid height for east grid")
                    pass
            
            if type(south_grid) is GradientGrid:
                if south_grid.width == width:
                        self.vectors[-1, :] = south_grid.vectors[0, :]
                else:
                    print("ERROR : Invalid width for south grid")
                    pass
            
            if type(west_grid) is GradientGrid:
                if west_grid.height == height:
                        self.vectors[:, 0] = west_grid.vectors[:, -1]
                else:
                    print("ERROR : Invalid height for west grid")
                    pass





# --------------------------------- # Testing GradientGrid # --------------------------------- #

if __name__ == "__main__":
    print("# --------------------------------- # Testing GradientGrid # --------------------------------- #\n")
    
    GradientGrid.setRandomSeed("seed")
    
    grid = GradientGrid(5, 5)
    print(grid)
    
    
    
    print("\nTrying to regenerate : ")
    grid.regenerate()
    print(grid)
    
    
    
    print("Trying to generate a grid at the north of the first one :")
    gridn = GradientGrid(5, 3, (None, None, grid, None))
    print(gridn)
    
    
    
    print("Trying to manually modify the first grid and see if the other gris is also modified : (It shouldn't)")
    grid.vectors[0, 2, 0] = 0
    print(grid)
    print(gridn)
