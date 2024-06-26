
#? To authorize type hints of class itself (ie Map here)
#? answer found here : https://stackoverflow.com/questions/33533148/how-do-i-type-hint-a-method-with-the-type-of-the-enclosing-class
#from typing import Self                    #? Python 3.11+
from __future__ import annotations          #? Python 3.7+

import numpy as np
import copy
from Python.gradientGrid import GradientGrid
from Python.layer import Layer
from Python.chunk import Chunk



class Map:
    """
    Map :
    -------------
    The Map class used to generate large terrain by gathering chunks.
    """
    
    
    #? -------------------------- Static ------------------------- #
    
    
    ALT_PRINTING_DECIMALS = 4
    ALT_PRINTING_FORMAT = " {{: .{}f}} ".format(ALT_PRINTING_DECIMALS)
    
    
    
    @staticmethod
    def interpolate2D(a1: float, a2: float, a3: float, a4: float, x: float, y: float) -> float:
        """Interpolates the four given values in 2D with a smoothstep function. `(x, y)` represent the point of interpolation.

        Args:
            `a1` (float): the first value, located in `(0, 0)`.
            `a2` (float): the second value, located in `(1, 0)`.
            `a3` (float): the third value, located in `(0, 1)`.
            `a4` (float): the fourth value, located in `(1, 1)`.
            `x` (float):  the relative x coordinate from the four values. Should be in range `[0, 1]`.
            `y` (float): the relative y coordinate from the four values. Should be in range `[0, 1]`.

        Returns:
            float: _description_
        """
        
        return a1 + (a2 - a1) * Layer.smoothstep(x) + (a3 - a1) * Layer.smoothstep(y) + (a1 + a4 - a2 - a3) * Layer.smoothstep(x) * Layer.smoothstep(y)
    
    
    
    
    
    
    @staticmethod
    def generateMapFromScratch(grid_widths: list[int], grid_heights: list[int], size_factors: list[int], layers_factors: list[float],
                               map_width: int, map_height: int) -> Map:
        """Generates a new Map structure and every required pre-structure based on the given parameters.

        Args:
            `grid_widths` (list[int]): the list of width to be used to generate the GradientGrids of each Chunk.
            `grid_heights` (list[int]): the list of height to be used to generate the GradientGrids of each Chunk.
            `size_factors` (list[int]): the list of size factors to be used to generate the Layers of each Chunk.
            `layers_factors` (list[float]): the list of layer factors to be used to generate each Chunk.
            `map_width` (int): the width of the map in chunks.
            `map_height` (int): the height of the map in chunks.

        Returns:
            Map: the newly generated Map structure.
        
        Note:
            Every list should have the same length, which is the number of layers per Chunk.
            The sum of layer factors should not be 0.
            The given gradient grid dimensions and size factors should match so that the product `grid_dim * size_factor` is a constant Vector2D.
        """
        
        
        #* -------------- Verifications : Begin --------------
        
        #TODO
        
        #* -------------- Verifications : End   --------------
        
        
        chunks = [[None for j in range(map_width)] for i in range(map_height)]
        
        
        for i in range(map_height):
            for j in range(map_width):
                
                #`(NORTH, EAST, SOUTH, WEST)`
                adj_chunks = [None, None, None, None]
                if i > 0:
                    adj_chunks[0] = chunks[i - 1][j]
                if j > 0:
                    adj_chunks[3] = chunks[i][j - 1]
                
                chunk = Chunk.generateChunkFromScratch(grid_widths, grid_heights, size_factors, layers_factors, tuple(adj_chunks))
                
                chunks[i][j] = chunk
        
        chunk_width = chunk.width
        chunk_height = chunk.height
        
        virtual_chunks = [Chunk.newVirtualChunk(chunk_width, chunk_height) for i in range(2*(map_width + 2) + 2*(map_height + 2) - 4)]
        
        
        return Map(map_width=map_width, map_height=map_height, chunks=chunks, virtual_chunks=virtual_chunks)
    
    
    
    #? ------------------------ Instances ------------------------ #
    
    
    
    def getVirtualChunk(self, width_idx: int, height_idx: int) -> Chunk:
        """Get the virtual chunk of the map at the given width and height indexes.

        Args:
            `width_idx` (int): width index of the virtual chunk. Should be in range `[0, map_width + 1]`.
            `height_idx` (int): height index of the virtual chunk. Should be in range `[0, map_height + 1]`.
        
        Returns:
            Chunk: the virtual chunk at the given indexes, or `None` if indexes were incorrect.
        """
        
        width = self.map_width + 2
        height = self.map_height + 2
        
        if (height_idx < 0 or height_idx >= height or width_idx < 0 or width_idx >= width):
            return None
        
        if width_idx == 0:
            return self.virtual_chunks[height_idx]
        elif width_idx == width - 1:
            return self.virtual_chunks[height + height_idx]
        elif height_idx == 0:
            return self.virtual_chunks[2*height + width_idx - 1]
        elif height_idx == height - 1:
            return self.virtual_chunks[2*height + width-2 + width_idx - 1]
        else:
            return None
    
    
    
    def __init__(self, map_width: int, map_height: int, chunks: list[list[Chunk]], virtual_chunks: list[Chunk]) -> None:
        """Initializes a new Map structure from the given list of already existing chunks and virtual chunks.
        If chunks should be generated in the process, please use one of the designated static methods.

        Args:
            `map_width` (int): width of the map in chunks.
            `map_height` (int): height of the map in chunks.
            `chunks` (list[list[Chunk]]): two-dimension matrix-like structure of chunks. First dimension is height, second is width.
            `virtual_chunks` (list[Chunk]): list of virtual chunks to generate the map altitudes. Virtual chunks should be stored in the following order :
                                            `Column0, ColumnN, Row0, RowN` without any repetition of the corners.
        """
        
        
        self.map_width = 0
        self.map_height = 0
        
        self.chunk_width = 0
        self.chunk_height = 0
        
        self.altitude = None
        self.chunks = None
        self.virtual_chunks = None
        
        #* -------------- Verifications : Begin --------------
        
        #TODO
        
        #* -------------- Verifications : End   --------------
        
        self.map_width = map_width
        self.map_height = map_height
        
        self.chunk_width = chunks[0][0].width
        self.chunk_height = chunks[0][0].height
        
        # "This version does not copy types like module, class, function, method, nor stack trace, stack frame, nor file,
        # socket, window, nor any similar types."
        self.chunks = copy.deepcopy(chunks) #? To keep both dimensions copied. Or use an np.ndarray ?
        self.virtual_chunks = virtual_chunks.copy()
        
        self.regenerate()
    
    
    
    def __str__(self) -> str:
        """Returns the string representation of the map.
        Used by the print function when called with a Map.

        Returns:
            `str`: the string representation of the Map.
        """
        final_str = "-----------------------------------------\n"
        final_str += "Map of {} x {} chunks of dimension {} x {}\n"\
            .format(self.map_width, self.map_height, self.chunk_width, self.chunk_height)
        # final_str += "\nAltitude values :\n"
        # for i in range(self.height):
            
        #     for j in range(self.width):
        #         alt = self.altitude[i, j]
        #         final_str += Layer.ALT_PRINTING_FORMAT.format(alt)
            
        #     final_str += "\n"
        final_str += "-----------------------------------------\n"
        
        return final_str
    
    
    
    def regenerate(self) -> None:
        """Regenerates the altitude values of the map based on its chunks.
        """
        
        #* -------------- Verifications : Begin --------------
        
        #TODO
        
        #* -------------- Verifications : End   --------------
        
        width = self.map_width * self.chunk_width
        height = self.map_height * self.chunk_height
        
        self.altitude = np.zeros((height, width))
        
        # First copies altitudes from chunks
        for i in range(height):
            for j in range(width):
                
                current_chunk = self.chunks[i//self.chunk_height][j//self.chunk_width]
                self.altitude[i][j] = current_chunk.altitude[i%self.chunk_height][j%self.chunk_width]
        
        
        self.applyMeanAltitude()
    
    
    
    def applyMeanAltitude(self) -> None:
        """Apply each chunk and virtual chunks mean altitude value on map final altitude values.
        """
        
        mw = self.map_width
        mh = self.map_height
        
        mean_altitudes = np.zeros((mh + 2, mw + 2))
        
        for i in range(1, mh-1):
            for j in range(1, mw-1):
                mean_altitudes[i][j] = self.chunks[i][j].base_altitude
        
        for i in range(0, mh+2):
            mean_altitudes[i][0] = self.getVirtualChunk(0, i).base_altitude
            mean_altitudes[i][-1] = self.getVirtualChunk(mw + 1, i).base_altitude
        
        for j in range(0, mw+2):
            mean_altitudes[0][j] = self.getVirtualChunk(j, 0).base_altitude
            mean_altitudes[-1][j] = self.getVirtualChunk(j, mh + 1).base_altitude
        
        cw = self.chunk_width
        ch = self.chunk_height
        
        
        for i in range(mh):
            for j in range(mw):
                
                a1 = mean_altitudes[i][j]
                a2 = mean_altitudes[i+1][j]
                a3 = mean_altitudes[i][j+1]
                a4 = mean_altitudes[i+1][j+1]
                
                for pi in range(ch):
                    for pj in range(cw):
                        
                        alt = Map.interpolate2D(a1, a2, a3, a4, pj/cw, pi/ch)
                        
                        alt_i = pi + int((i-0.5)*ch)
                        alt_j = pj + int((j-0.5)*cw)
                        self.altitude[alt_i][alt_j] += alt





# --------------------------------- # Testing Map # --------------------------------- #

if __name__ == "__main__":
    print("# --------------------------------- # Testing Map # --------------------------------- #\n")
    
    GradientGrid.setRandomSeed("seed")
    
    grid_dim = [3, 5]
    size_factors = [8, 4]
    layer_factors = [1, .1]
    
    
    print("Generating a map of size (2 x 1) :")
    
    chunk1 = Chunk.generateChunkFromScratch(grid_widths=grid_dim, grid_heights=grid_dim, size_factors=size_factors, layers_factors=layer_factors,
                                            adj_chunks=(None, None, None, None))
    
    
    chunk2 = Chunk.generateChunkFromScratch(grid_widths=grid_dim, grid_heights=grid_dim, size_factors=size_factors, layers_factors=layer_factors,
                                            adj_chunks=(None, None, None, chunk1))
    
    
    
    chunk_dim = grid_dim[0] * size_factors[0]
                                                                #? Perimeter :  2*(width + 2) + 2*(height + 2) - 4
    virtual_chunks = [Chunk.newVirtualChunk(chunk_dim, chunk_dim) for i in range(2*(1 + 2) + 2*(2 + 2) - 4)]
    
    map1 = Map(2, 1, [[chunk1, chunk2]], virtual_chunks)
    
    print(map1)
    
    
    
    
    map_dimensions = (2, 3)
    
    print("Generating a new map from scratch with parameters : ")
    print(" - map dimensions : ", map_dimensions)
    print(" - grids square dimensions : ", grid_dim)
    print(" - layers size_factors : ", size_factors)
    print(" - chunk layer factors : ", layer_factors)
    
    map = Map.generateMapFromScratch(grid_dim, grid_dim, size_factors, layer_factors, map_dimensions[0], map_dimensions[1])
    
    print(map)
    