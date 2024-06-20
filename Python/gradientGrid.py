import numpy as np
import random as rd

class gradientGrid:
    """
    Description:
    ------------
    The gradientGrid class used to store the primary randomly generated vectors responsible for the whole Perlin generation
    afterwards.
    
    Note:
    -----
    In every matrix notation, the indexes `i` and `j` respectively refers to rows and columns, so height and width.
    Sizes are expressed in `width x height`. But Matrix are thus of dimensions `height x width`.
    """
    
    MAX_INT_SEED = 1000000000000 # Maximum integer for the automatic random seed generation
    
    VEC_PRINTING_DECIMALS = 4
    VEC_PRINTING_FORMAT = " ({{: .{}f}}, {{: .{}f}}) ".format(VEC_PRINTING_DECIMALS, VEC_PRINTING_DECIMALS)
    
    
    def __init__(self, width: int = 2, height: int = 2) -> None:
        """Initialize a new random gradientGrid instance with given width and height values.
        Should be of size `2 x 2` at minimum.

        Args:
            `width` (int, optional): the width of the generated random gradientGrid. Defaults to `2`.
            `height` (int, optional): the height of the generated random gradientGrid. Defaults to `2`.
        """
        
        self.width = width
        self.height = height
        self.vectors = [[np.zeros((2)) for j in range(self.width)] for i in range(self.height)]
        self.regenerate()
    
    
    def __str__(self) -> str:
        """Returns the string representation of the gradientGrid.
        Used by the print function when called with a gradientGrid.

        Returns:
            `str`: the string representation of the gradientGrid.
        """
        final_str = "-----------------------------------------\n"
        final_str += "Gradient Grid of dimensions {} x {} :\n".format(self.width, self.height)
        for i in range(self.height):
            
            for j in range(self.width):
                vec = self.vectors[i][j]
                final_str += gradientGrid.VEC_PRINTING_FORMAT.format(vec[0], vec[1])
            
            final_str += "\n"
        final_str += "-----------------------------------------\n"
        
        return final_str
    
    
    def regenerate(self) -> None:
        """Regenerates the random vectors of the gradientGrid.
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
    
    
    @staticmethod
    def setRandomSeed(seed: int | float | str | bytes | bytearray | None = rd.randint(1, MAX_INT_SEED)) -> None:
        """Set the seed for the future random generations.

        Args:
            `seed` (int | float | str | bytes | bytearray | None): the seed to be used in the future random generations.
                                                                   Defaults to a random integer between `1` and `gradientGrid.MAX_INT_SEED`.
        """
        
        rd.seed(seed)





# --------------------------------- # Testing gradientGrid # --------------------------------- #

if __name__ == "__main__":
    print("# --------------------------------- # Testing gradientGrid # --------------------------------- #\n")
    
    gradientGrid.setRandomSeed("seed")
    
    grid = gradientGrid(5, 5)
    print(grid)
    
    print("\nTrying to regenerate : ")
    grid.regenerate()
    
    print(grid)
