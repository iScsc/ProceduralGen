import random as rd

import numpy as np
import matplotlib.pyplot as plt

from mapGenerator import *

LOAD = False
GENERATE = True

if LOAD:
    path_to_load = "./saves/completeMap_test/"
    
    sea_map, color_map, map_width_in_points, map_height_in_points, sea_level, map_width_in_chunks, map_height_in_chunks = mapGenerator.loadCompleteMap(path_to_load)
    
    # print(sea_map)
    # print(color_map)
    
    mapGenerator.printMap((map_width_in_chunks, map_height_in_chunks), (map_width_in_points, map_height_in_points), sea_level, sea_map, color_map)



if GENERATE:
    start = time.time()
    #SEED = 909729265464 #rd.randint(0, 1000000000000)
    SEED = mapGenerator.getRandSeed() #1714995019 #436517554376 # mapGenerator.getRandSeed()
    # Interesting seeds :
    # 909729265464
    # 436517554376
    #
    #
    #
    print("SEED =", SEED)

    mapGenerator.setSeed(SEED)

    MAX_LENGTH = 3

    #20 first prime numbers
    prime_numbers = np.array([2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 43, 47, 53, 59, 61, 67, 71, 73])

    GRID_SIZE = [2, 5, 11] #np.array([prime_numbers[i] for i in range(0, min(len(prime_numbers), MAX_LENGTH) ) ] ) #np.array([prime_numbers[2*i] for i in range(0, min(len(prime_numbers)//2, MAX_LENGTH) ) ] )  #np.array([2, 15, 35, 80]) #np.array([5,8,10,15,20,36])
    VALUE_FACTOR = [1, 0.1, 0.01] #np.array([1/i**2 for i in range(1, len(GRID_SIZE) + 1)])  #np.array([100, 10, 1, 1]) #np.array([.9, .7, .6])


    MAP_SIZE = (5, 7) #(1,1) #(5, 7)


    ppcm = ppcm_of_list(GRID_SIZE)


    SEA_LEVEL = -0.15


    full_map, sea_map, cmap_float = mapGenerator.fullGen(GRID_SIZE, VALUE_FACTOR, SEA_LEVEL, map_size=MAP_SIZE, display_loading = False, display_map = True)
    print("The whole generation took {} seconds.".format(time.time() - start))
