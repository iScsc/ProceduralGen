import random as rd
import time
import numpy as np
import matplotlib.pyplot as plt

from old_mapGenerator import mapGenerator as old_MapGenerator
from mapGenerator import MapGenerator

LOAD = True
GENERATE = False

if LOAD:
    path_to_load = "../saves/completeMap_test/"
    
    sea_map, color_map, map_width_in_points, map_height_in_points, sea_level, map_width_in_chunks, map_height_in_chunks = old_MapGenerator.loadCompleteMap(path_to_load)
    
    # print(sea_map)
    # print(color_map)
    
    old_MapGenerator.printMap((map_width_in_chunks, map_height_in_chunks), (map_width_in_points, map_height_in_points), sea_level, sea_map, color_map)



if GENERATE:
    start = time.time()
    
    SEED = MapGenerator.getRandomSeed() #1714995019 #436517554376
    # Interesting seeds :
    # 909729265464
    # 436517554376
    #
    #
    #
    SEED = "seed"
    print("SEED =", SEED)

    MapGenerator.setRandomSeed(SEED)

    MAX_LENGTH = 3

    #20 first prime numbers
    prime_numbers = np.array([2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 43, 47, 53, 59, 61, 67, 71, 73])

    GRID_SIZES = [3, 5, 9]                  #[2, 5, 11] #np.array([prime_numbers[i] for i in range(0, min(len(prime_numbers), MAX_LENGTH) ) ] ) #np.array([prime_numbers[2*i] for i in range(0, min(len(prime_numbers)//2, MAX_LENGTH) ) ] )  #np.array([2, 15, 35, 80]) #np.array([5,8,10,15,20,36])
    LAYERS_FACTORS = [1, 0.1, 0.01]         #[1, 0.1, 0.01] #np.array([1/i**2 for i in range(1, len(GRID_SIZE) + 1)])  #np.array([100, 10, 1, 1]) #np.array([.9, .7, .6])


    MAP_SIZE = (3, 2)                       #(1,1) #(5, 7)


    lcm = MapGenerator.lcmOfArray(GRID_SIZES)


    SEA_LEVEL = -0.3
    
    
    new_map = MapGenerator.fullGen(GRID_SIZES, LAYERS_FACTORS, MAP_SIZE[0], MAP_SIZE[1], SEA_LEVEL)
    
    
    print("The whole generation took {} seconds.".format(time.time() - start))
    print("Plotting now...")
    
    MapGenerator.plotCompleteMap(new_map)
