
#? To authorize type hints of class itself (ie Layer here)
#? answer found here : https://stackoverflow.com/questions/33533148/how-do-i-type-hint-a-method-with-the-type-of-the-enclosing-class
#from typing import Self                    #? Python 3.11+
from __future__ import annotations          #? Python 3.7+

import numpy as np
from gradientGrid import GradientGrid






class Layer:
    """
    Layer :
    -------------
    The layer class used to generate an altitude map based on a GradientGrid structure.
    """
    
    
    #? -------------------------- Static ------------------------- #
    
    
    ALT_PRINTING_DECIMALS = 4
    ALT_PRINTING_FORMAT = " {{: .{}f}} ".format(ALT_PRINTING_DECIMALS)
    
    
    @staticmethod
    def smoothstep(w: float) -> float:
        """A smoothstep function to smoothly go from 0 to 1.
        It uses the analytical function `3w² - 2w³`

        Args:
            `w` (float): the input value. Should be in `[0, 1]`

        Returns:
            float: the resulting smoothstep value.
        """
        
        if w <= 0:
            return 0.
        if w >= 1:
            return 1.
        
        return 3*w**2 - 2*w**3
    
    
    
    @staticmethod
    def interpolate(a0: float, a1: float, w: float) -> float:
        """A smooth interpolating function.

        Args:
            `a0` (float): the first value to interpolate from, which corresponds to `w = 0`.
            `a1` (float): the second value to interpolate from, which corresponds to `w = 1`.
            `w` (float): the smoothstep input. Should be between `0` and `1`.

        Returns:
            float: the interpolated value between `a0` and `a1`.
        """
        
        return a0 + (a1 - a0) * Layer.smoothstep(w)
    
    
    
    @staticmethod
    def dotGridGradient(ix: int, iy: int, x: float, y: float, grid: GradientGrid) -> float:
        """Computes the dot product between the (x - ix, y - iy) vector and the extracted vector from the given gradient grid.

        Args:
            `ix` (int): the width index to extract the vector from the gradient grid.
            `iy` (int): the height index to extract the vector from the gradient grid.
            `x` (float): the width position.
            `y` (float): the height position.
            `grid` (GradientGrid): the gradient grid to compute from.

        Returns:
            float: the computed dot product.
        """
        
        if int(ix) != ix or int(iy) != iy:
            raise ValueError("ix and iy must be integers! Are : {} and {}".format(ix, iy))
        
        ix = int(ix)
        iy = int(iy)
        
        dx = x - ix
        dy = y - iy

        v = grid.vectors[ix, iy]

        return dx * v[0] + dy * v[1]
    
    
    
    @staticmethod
    def perlin(x: float, y: float, grid: GradientGrid) -> float:
        """Computes the altitude value at position (x, y) from the given gradient grid.

        Args:
            `x` (float): the width position.
            `y` (float): the height position.
            `grid` (GradientGrid): the gradient grid to compute the altitude from.

        Returns:
            float: the final altitude value.
        """
        
        x0 = np.floor(x)
        x1 = x0 + 1
        y0 = np.floor(y)
        y1 = y0 + 1
        
        sx = x - x0
        sy = y - y0
        
        n0 = Layer.dotGridGradient(x0, y0, x, y, grid)
        n1 = Layer.dotGridGradient(x1, y0, x, y, grid)
        ix0 = Layer.interpolate(n0, n1, sx)
        n0 = Layer.dotGridGradient(x0, y1, x, y, grid)
        n1 = Layer.dotGridGradient(x1, y1, x, y, grid)
        ix1 = Layer.interpolate(n0, n1, sx)
        value = Layer.interpolate(ix0, ix1, sy)

        return value
    
    
    
    
    
    
    
    
    @staticmethod
    def generateLayerFromScratch(grid_width: int, grid_height: int, size_factor: int, adj_layers: tuple[Layer] = (None, None, None, None)) -> Layer:
        """Generates a new GradientGrid structure with the given parameters and then generate a new Layer structure from it.
        You can additionally specify adjacent layers during the generation to use their GradientGrid as neighbours for the new GradientGrid generation.

        Args:
            `grid_width` (int): the width to be used to generate an initial gradient grid.
            `grid_height` (int): the height to be used to generate an initial gradient grid.
            `size_factor` (int): the size factor to be used to generate the layer. The layer dimensions will be
                                 `(gradient_grid_dimensions - 1) * size_factor`.
            `adjacent_layers` (tuple[Layer], optional): the tuple of adjacent layers to generate a smooth transition in terrain with correct
                                                        boundary conditions.
                                                        The tuple is in order `(NORTH, EAST, SOUTH, WEST)`.
                                                        Set a layer to `None` if there are no neighbours in this direction.
                                                        Defaults to `(None, None, None, None)`.
        """
        
        if not (type(grid_width) is int and type(grid_height) is int and type(size_factor) is int and size_factor > 1):
            print("Invalid parameters : grid dimensions and size_factor should be integers. Moreover, size_factor should be > 1.")
            return
        
        adj_list = [None, None, None, None]
        for i in range(len(adj_layers)):
            layer = adj_layers[i]
            if type(layer) is Layer:
                adj_list[i] = layer.grid
        adj_tuple = tuple(adj_list)
        
        
        grid: GradientGrid = GradientGrid(grid_width, grid_height, adjacent_grids=adj_tuple)
        
        return Layer(grid=grid, size_factor=size_factor)
    
    
    
    @staticmethod
    def write(path: str, layer: Layer) -> None:
        #TODO
        pass
    
    
    
    @staticmethod
    def read(path: str) -> Layer:
        #TODO
        return None
    
    
    #? ------------------------ Instances ------------------------ #
    
    
    def __init__(self, grid: GradientGrid, size_factor: int) -> None:
        """Initializes a new layer instance from an already existing GradientGrid and a size factor.
        If no gradient grid are already generated, please use the designated static method `generateLayerFromScratch` to generate the
        appropriate GradientGrid and then generate the layer structure.

        Args:
            `grid` (GradientGrid): the gradient grid to build the layer from.
            `size_factor` (int): the size factor to be used to generate the layer. The layer dimensions will be
                                 `(gradient_grid_dimensions - 1) * size_factor`.
        """
        
        self.grid = None
        self.width = None
        self.height = None
        
        self.size_factor = None
        
        self.altitude = None
        
        
        if not (type(grid) is GradientGrid and type(size_factor) is int and size_factor > 1):
            print("Invalid parameters. Either gradient grid is None, size_factor is not an integer, or is <= 1.")
            return
        
        
        self.grid = grid
        self.size_factor = size_factor
        
        self.regenerate()
    
    
    
    def __str__(self) -> str:
        """Returns the string representation of the Layer.
        Used by the print function when called with a Layer.

        Returns:
            `str`: the string representation of the Layer.
        """
        final_str = "-----------------------------------------\n"
        final_str += "Layer of dimensions {} x {} :\n".format(self.width, self.height)
        for i in range(self.height):
            
            for j in range(self.width):
                alt = self.altitude[i, j]
                final_str += Layer.ALT_PRINTING_FORMAT.format(alt)
            
            final_str += "\n"
        final_str += "-----------------------------------------\n"
        
        return final_str
    
    
    
    def regenerate(self) -> None:
        """Regenerates the altitude values of the layer based on its `grid` and `size_factor` parameters.
        """
        
        if not (type(self.grid) is GradientGrid and type(self.size_factor) is int and self.size_factor > 1):
            print("Invalid parameters. Either gradient grid is None, size_factor is not an integer, or is <= 1.")
            return
        
        self.width = (self.grid.width - 1) * self.size_factor
        self.height = (self.grid.height - 1) * self.size_factor
        
        self.altitude = np.zeros((self.height, self.width))
        
        for i in range(self.height):
            for j in range(self.width):
                
                self.altitude[i, j] = Layer.perlin(i/self.size_factor, j/self.size_factor, self.grid)





# --------------------------------- # Testing Layer # --------------------------------- #

if __name__ == "__main__":
    print("# --------------------------------- # Testing Layer # --------------------------------- #\n")
    
    GradientGrid.setRandomSeed("seed")
    
    factor = 4
    grid = GradientGrid(5, 5)
    layer = Layer(grid=grid, size_factor=factor)
    print(layer)
    
    
    
    print("\nTrying to regenerate : ")
    grid.regenerate()
    layer.regenerate()
    print(layer)
    
    
    
    print("Trying to generate a layer at the north of the first one :")
    layern = Layer.generateLayerFromScratch(grid_width=5, grid_height=5, size_factor=factor, adj_layers=(None, None, layer, None))
    print(layer.grid)
    print(layern.grid)
    print(layern)
