
#? To authorize type hints of class itself (ie GradientGrid here)
#? answer found here : https://stackoverflow.com/questions/33533148/how-do-i-type-hint-a-method-with-the-type-of-the-enclosing-class
#from typing import Self                    #? Python 3.11+
from __future__ import annotations          #? Python 3.7+

import numpy as np
from gradientGrid import GradientGrid
from layer import Layer


class Chunk:
    """
    Chunk :
    -------------
    The Chunk class used to generate an altitude map by superposition of multiple layers.
    """
    
    
    #? -------------------------- Static ------------------------- #
    
    
    ALT_PRINTING_DECIMALS = 4
    ALT_PRINTING_FORMAT = " {{: .{}f}} ".format(ALT_PRINTING_DECIMALS)
    
    
    
    
    
    @staticmethod
    def generateChunkFromScratch(grid_widths: list[int], grid_heights: list[int], size_factors: list[int], layers_factors: list[float],
                                 adj_chunks: tuple[Chunk] = (None, None, None, None)) -> Chunk:
        
        return None
    
    
    
    @staticmethod
    def generateChunkFromGradientGrids(grids: list[GradientGrid], size_factors: list[int], layers_factors: list[float]) -> Chunk:
        
        return None
    
    
    
    @staticmethod
    def write(path: str, chunk: Chunk) -> None:
        #TODO
        pass
    
    
    
    @staticmethod
    def read(path: str) -> Chunk:
        #TODO
        return None
    
    
    #? ------------------------ Instances ------------------------ #
    
    
    def __init__(self, layers_factors: list[float], layers: list[Layer]) -> None:
        """Initializes a new Chunk structure from the given list of already existing layers and factors.
        If layers should be generated in the process, please use one of the designated static methods.

        Args:
            layers_factors (list[float], optional): the list of factors to be used in the weighted average to generate the chunk.
            layers (list[Layer], optional): the list of existing chunks to be averaged to generate the chunk.
        """
        
        self.layers_factors = None
        self.layers = None
        
        self.altitude = None
        
        if not (type(layers_factors) is list and type(layers) is list):
            return
        
        len_fac = len(layers_factors)
        len_lay = len(layers)
        
        if len_fac != len_lay:
            print("Invalid sizes : list of layers and list of factors are not of the same length! ({} and {})"
                  .format(len_lay, len_fac))
            return
        
        
        # Check types
        for i in range(len_fac):
            fac = layers_factors[i]
            
            if not str(fac).isnumeric():
                print("Layer factor {} at index {} is not numeric".format(fac, i))
                return
            
            if not type(layers[i]) is Layer:
                print("Layer at index {} is not a layer.".format(i))
                return
        
        # Store layers and factors
        self.layers = layers
        self.layers_factors = layers_factors
        
        # Compute altitude
        self.regenerate()
            
    
    
    
    def __str__(self) -> str:
        return ""
    
    
    
    def regenerate(self) -> None:
        return





# --------------------------------- # Testing Chunk # --------------------------------- #

if __name__ == "__main__":
    print("# --------------------------------- # Testing Chunk # --------------------------------- #\n")
    
    GradientGrid.setRandomSeed("seed")
