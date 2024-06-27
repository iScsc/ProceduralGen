
#? To authorize type hints of class itself (ie Chunk here)
#? answer found here : https://stackoverflow.com/questions/33533148/how-do-i-type-hint-a-method-with-the-type-of-the-enclosing-class
#from typing import Self                    #? Python 3.11+
from __future__ import annotations          #? Python 3.7+

import numpy as np
import random as rd
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
    def generateBaseAltitude() -> float:
        #? Can be modified to affect every `base_altitude` values generated in chunks.
        
        return -1 + 2*rd.random()
        
        # return -0.5 + 2*rd.random()
    
    
    
    
    
    @staticmethod
    def generateChunkFromScratch(grid_widths: list[int], grid_heights: list[int], size_factors: list[int], layers_factors: list[float],
                                 adj_chunks: tuple[Chunk] = (None, None, None, None)) -> Chunk:
        """Generates a Chunk structure and every sub-structure required based on the given parameters.
        Additionally, adjacent chunks can be specified to generate an adjacent chunk.

        Args:
            `grid_widths` (list[int]): the list of width to be used to generate the GradientGrids.
            `grid_heights` (list[int]): the list of height to be used to generate the GradientGrids.
            `size_factors` (list[int]): the list of size factors to be used to generate the Layers.
            `layers_factors` (list[float]): the list of layer factors to be used to generate the final Chunk.
            `adj_chunks` (tuple[Chunk], optional): the tuple of adjacent chunks to generate a smooth transition in terrain with correct
                                                    boundary conditions.
                                                    The tuple is in order `(NORTH, EAST, SOUTH, WEST)`.
                                                    Set a chunk to `None` if there are no neighbours in this direction.
                                                    Defaults to `(None, None, None, None)`.

        Returns:
            Chunk: the final Chunk structure generated.
        
        Note:
            Every list should have the same length.
            The sum of layer factors should not be 0.
            The given gradient grid dimensions and size factors should match so that the product `grid_dim * size_factor` is a constant Vector2D.
        """
        
        #* -------------- Verifications : Begin --------------
        
        if not (type(grid_widths) is list and type(grid_heights) is list and type(size_factors) is list and type(layers_factors) is list):
            print("Error: `grid_widths`, `grid_heights`, `size_factors` and `layers_factors` should be lists!")
            return None
        
        len_wid = len(grid_widths)
        len_hei = len(grid_heights)
        len_siz = len(size_factors)
        len_lay = len(layers_factors)
        
        if not (len_wid == len_hei and len_hei == len_siz and len_siz == len_lay):
            print("Invalid sizes : lists `grid_widths`, `grid_heights`, `size_factors` and `layers_factors` are not of the same length! ({}, {}, {} and {})"
                  .format(len_wid, len_hei, len_siz, len_lay))
            return None
        
        
        if len_wid == 0:
            return Chunk([], [])
        
        
        
        final_width = (grid_widths[0] - 1) * size_factors[0]
        final_height = (grid_heights[0] - 1) * size_factors[0]
        factor_sum = 0
        
        for i in range(len_wid):
            wid = grid_widths[i]
            hei = grid_heights[i]
            siz = size_factors[i]
            lay = layers_factors[i]
            
            if not str(wid).isnumeric():
                print("Grid width {} at index {} is not numeric".format(wid, i))
                return None
            
            if not str(hei).isnumeric():
                print("Grid height {} at index {} is not numeric".format(hei, i))
                return None
            
            if not str(siz).isnumeric():
                print("Size factor {} at index {} is not numeric".format(siz, i))
                return None
            
            
            if not (wid - 1) * siz == final_width:
                print("Invalid width {} at index {} with size factor {} -> does not match final width {}".format(wid, i, siz, final_width))
                return None
            
            if not (hei - 1) * siz == final_height:
                print("Invalid height {} at index {} with size factor {} -> does not match final height {}".format(hei, i, siz, final_height))
                return None
            
            
            if not float(lay) == lay:
                print("Layer factor {} at index {} is not a float".format(lay, i))
                return
            
            factor_sum += lay
        
        if factor_sum == 0:
            print("Invalid layer factors : sum is 0")
            return None
        
        #* -------------- Verifications : End --------------
        
        
        layers = [None for i in range(len_lay)]
        for i in range(len_lay):
            adj_layers = [None, None, None, None]
            for j in range(len(adj_chunks)):
                chunk = adj_chunks[j]
                if type(chunk) is Chunk:
                    adj_layers[j] = chunk.layers[i]
            adj_layers = tuple(adj_layers)
            
            layers[i] = Layer.generateLayerFromScratch(grid_width=grid_widths[i], grid_height=grid_heights[i],
                                                       size_factor=size_factors[i], adj_layers=adj_layers)
        
        
        return Chunk(layers_factors=layers_factors, layers=layers)
    
    
    
    @staticmethod
    def generateChunkFromGradientGrids(grids: list[GradientGrid], size_factors: list[int], layers_factors: list[float]) -> Chunk:
        """Generates a Chunk structure and every sub-layer structures required based on the given GradientGrid structures and layer factors.

        Args:
            `grids` (list[int]): the list of GradientGrids to build the Layers from.
            `size_factors` (list[int]): the list of size factors to be used to generate the Layers.
            `layers_factors` (list[float]): the list of layer factors to be used to generate the final Chunk.

        Returns:
            Chunk: the final Chunk structure generated.
        
        Note:
            Every list should have the same length.
            The sum of layer factors should not be 0.
            The given grids' dimensions and size factors should match so that the product `grid_dim * size_factor` is a constant Vector2D.
        """
        
        
        #* -------------- Verifications : Begin --------------
        
        if not (type(grids) is list and type(size_factors) is list and type(layers_factors) is list):
            print("Error: `grids`, `size_factors` and `layers_factors` should be lists!")
            return None
        
        len_gri = len(grids)
        len_siz = len(size_factors)
        len_lay = len(layers_factors)
        
        if not (len_gri == len_siz and len_siz == len_lay):
            print("Invalid sizes : lists `grids`, `size_factors` and `layers_factors` are not of the same length! ({}, {} and {})"
                  .format(len_gri, len_siz, len_lay))
            return None
        
        
        if len_gri == 0:
            return Chunk([], [])
        
        
        
        final_width = (grids[0].width - 1) * size_factors[0]
        final_height = (grids[0].height - 1) * size_factors[0]
        factor_sum = 0
        
        for i in range(len_gri):
            grid = grids[i]
            siz = size_factors[i]
            lay = layers_factors[i]
            
            if not type(grid) is GradientGrid:
                print("Grid at index {} is not a grid.".format(i))
                return None
            
            if not str(siz).isnumeric():
                print("Size factor {} at index {} is not numeric".format(siz, i))
                return None
            
            
            if not (grid.width - 1) * siz == final_width:
                print("Invalid grid width {} at index {} with size factor {} -> does not match final width {}".format(grid.width, i, siz, final_width))
                return None
            
            if not (grid.height - 1) * siz == final_height:
                print("Invalid height {} at index {} with size factor {} -> does not match final height {}".format(grid.height, i, siz, final_height))
                return None
            
            
            if not float(lay) == lay:
                print("Layer factor {} at index {} is not a float".format(lay, i))
                return
            
            factor_sum += lay
        
        if factor_sum == 0:
            print("Invalid layer factors : sum is 0")
            return None
        
        #* -------------- Verifications : End --------------
        
        
        layers = [None for i in range(len_lay)]
        for i in range(len_lay):
            layers[i] = Layer(grid=grids[i], size_factor=size_factors[i])
        
        
        return Chunk(layers_factors=layers_factors, layers=layers)
    
    
    
    @staticmethod
    def newVirtualChunk(chunk_width: int, chunk_height: int) -> Chunk:
        """Generates a new virtual chunk.

        Args:
            `chunk_width` (int): width of the virtual chunk.
            `chunk_height` (int): height of the virtual chunk.

        Returns:
            Chunk: the newly generated virtual chunk
        """
        
        # Initialize an empty chunk
        virtual_chunk = Chunk([], [])
        
        virtual_chunk.width = chunk_width
        virtual_chunk.height = chunk_height
        
        virtual_chunk.base_altitude = .5 #Chunk.generateBaseAltitude()
        
        return virtual_chunk
    
    
    
    
    @staticmethod
    def write(path: str, chunk: Chunk) -> None:
        #TODO
        pass
    
    
    
    @staticmethod
    def read(path: str) -> Chunk:
        #TODO
        return None
    
    
    #? ------------------------ Instances ------------------------ #
    
    
    def __init__(self, layers: list[Layer], layers_factors: list[float]) -> None:
        """Initializes a new Chunk structure from the given list of already existing layers and factors.
        If layers should be generated in the process, please use one of the designated static methods.

        Args:
            `layers` (list[Layer], optional): the list of existing chunks to be averaged to generate the chunk.
            `layers_factors` (list[float], optional): the list of factors to be used in the weighted average to generate the chunk.
        """
        
        self.layers_factors = None
        self.layers = None
        
        self.altitude = None
        self.base_altitude = 0
        
        
        
        #* -------------- Verifications : Begin --------------
        
        # Checking types
        if not (type(layers_factors) is list and type(layers) is list):
            return
        
        len_fac = len(layers_factors)
        len_lay = len(layers)
        
        if len_fac != len_lay:
            print("Invalid sizes : list of layers and list of factors are not of the same length! ({} and {})"
                  .format(len_lay, len_fac))
            return
        
        
        if len_fac == 0:
            return
        
        
        for i in range(len_fac):
            fac = layers_factors[i]
            
            if not float(fac) == fac:
                print("Layer factor {} at index {} is not a float".format(fac, i))
                return
            
            if not type(layers[i]) is Layer:
                print("Layer at index {} is not a layer.".format(i))
                return
        
        #* -------------- Verifications : End --------------
        
        
        # Store layers and factors
        self.layers = layers.copy()
        self.layers_factors = layers_factors.copy()
        
        # Compute altitude
        self.regenerate()
    
    
    
    def __str__(self) -> str:
        """Returns the string representation of the chunk.
        Used by the print function when called with a Chunk.

        Returns:
            `str`: the string representation of the Chunk.
        """
        final_str = "-----------------------------------------\n"
        final_str += "Chunk of dimensions {} x {} composed of {} layer(s) with the following layer factors :\n"\
            .format(self.width, self.height, len(self.layers))
        final_str += str(self.layers_factors)
        final_str += "\nAltitude values :\n"
        for i in range(self.height):
            
            for j in range(self.width):
                alt = self.altitude[i, j]
                final_str += Layer.ALT_PRINTING_FORMAT.format(alt)
            
            final_str += "\n"
        final_str += "-----------------------------------------\n"
        
        return final_str
    
    
    
    def regenerate(self) -> None:
        """Regenerates the altitude values of the chunk based on its `layers` and `layer_factors` parameters.
        """
        
        #* -------------- Verifications : Begin --------------
        
        # Checking types
        if not (type(self.layers_factors) is list and type(self.layers) is list):
            return
        
        len_fac = len(self.layers_factors)
        len_lay = len(self.layers)
        
        if len_fac != len_lay:
            print("Invalid sizes : list of layers and list of factors are not of the same length! ({} and {})"
                  .format(len_lay, len_fac))
            return
        if len_lay == 0:
            print("Chunk's `layers` list is empty.")
            return
        
        
        self.width = self.layers[0].width
        self.height = self.layers[0].height
        factor_sum = 0
        
        for i in range(len_fac):
            fac = self.layers_factors[i]
            
            if not float(fac) == fac:
                print("Layer factor {} at index {} is not a float".format(fac, i))
                return
            
            factor_sum += fac
            
            layer = self.layers[i]
            if not type(layer) is Layer:
                print("Layer at index {} is not a layer.".format(i))
                return
            
            if layer.width != self.width or layer.height != self.height:
                print("Layer at index {} has invalid dimensions : {} while chunk has dimensions {}"
                      .format(i, (layer.width, layer.height), (self.width, self.height)))
                return
        
        if factor_sum == 0:
            print("Invalid layer factors: the sum of layer factors is 0.")
            return
        
        #* -------------- Verifications : End --------------
        
        
        
        self.altitude = np.zeros((self.height, self.width))
        
        for i in range(len_fac):
            fac = self.layers_factors[i]
            layer = self.layers[i]
            
            self.altitude += fac * layer.altitude
        
        self.altitude /= factor_sum
        self.base_altitude = 0 #Chunk.generateBaseAltitude()





# --------------------------------- # Testing Chunk # --------------------------------- #

if __name__ == "__main__":
    print("# --------------------------------- # Testing Chunk # --------------------------------- #\n")
    
    GradientGrid.setRandomSeed("seed")
    
    #? Will lead to dimensions 16x16
    grid_dim = [3, 5]
    size_factors = [8, 4]
    layer_factors = [1, .1]
    
    grid1 = GradientGrid(grid_dim[0], grid_dim[0])
    layer1 = Layer(grid1, size_factors[0])
    
    grid2 = GradientGrid(grid_dim[1], grid_dim[1])
    layer2 = Layer(grid2, size_factors[1])
    
    chunk = Chunk(layers=[layer1, layer2], layers_factors=layer_factors)
    print(chunk)
    
    
    
    
    print("Generating a south chunk from gradient grids structures :")
    grid3 = GradientGrid(grid_dim[0], grid_dim[0], (grid1, None, None, None))
    grid4 = GradientGrid(grid_dim[1], grid_dim[1], (grid2, None, None, None))
    
    print(" ---> gradient grid of the first layer :")
    print(grid1)
    print(grid3)
    
    print(" ---> final chunk :")
    chunk_s = Chunk.generateChunkFromGradientGrids(grids=[grid3, grid4], size_factors=size_factors, layers_factors=layer_factors)
    print(chunk_s)
    
    
    
    
    print("Generating an east chunk (from the last one) from scratch :")
    chunk_e = Chunk.generateChunkFromScratch(grid_widths=grid_dim, grid_heights=grid_dim, size_factors=size_factors, layers_factors=layer_factors,
                                             adj_chunks=(None, None, None, chunk_s))
    
    print(" ---> gradient grid of the first layer :")
    print(grid3)
    print(chunk_e.layers[0].grid)
    
    print(" ---> gradient grid of the second layer :")
    print(grid4)
    print(chunk_e.layers[1].grid)
    
    print(" ---> final chunk :")
    chunk_s = Chunk.generateChunkFromGradientGrids(grids=[grid3, grid4], size_factors=size_factors, layers_factors=layer_factors)
    print(chunk_s)
