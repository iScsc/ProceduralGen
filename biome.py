import json

class Biome:
    
    #? Mean altitude != mean(altitude_range) -> https://fr.wikipedia.org/wiki/Loi_normale_asym%C3%A9trique
    def __init__(self, name = "default_biome", altitude_range = (-1, 1), mean_altitude = 0) -> None:
        self.name = name
        self.altitude_range = altitude_range
        self.mean_altitude = mean_altitude
    
    
    def toJSON(self):
        return json.dumps(self.__dict__, indent="\t")
    
    def from_JSON_str(loading_str : str):
        json_dict = json.loads(loading_str)
        
        new_biome = Biome()
        
        for key in json_dict:
            setattr(new_biome, key, json_dict[key])
        
        return new_biome





if __name__ == "__main__":
    
    LOAD = True
    SAVE = False
    
    biomes = []
    
    if not LOAD:
        print("Trying to generate some biomes")
        
        for i in range(5):
            b = Biome("biome_nb{}".format(i), altitude_range=(-i, i), mean_altitude=(3*i)/4)
            biomes.append(b)
    
        print("Printing their conversion to json...")
        for i in range(5):
            print(biomes[i].toJSON())
    
        print("Creating a new one from the last json string being modified and verifying the result")
        new_json = biomes[-1].toJSON().replace("biome", "Nice_biome")
        new_biome = Biome.from_JSON_str(new_json)
        print(new_json)
        print(new_biome.name)
        biomes.append(new_biome)
    else:
        loading_path = "./saves/biomes_test.txt" #"./saves/biomes.txt"
        print("Loading biomes from the file at '{}'".format(loading_path))
        f = open(loading_path, "r")
        lines = f.readlines()
        f.close()
        
        # for line in lines:
        #     biomes.append(Biome.from_JSON_str(line))
        
        file_str = ""
        for line in lines:
            file_str += line
        
        print(json.loads(file_str))
        
        print("Here are the loaded biomes : ")
        for b in biomes:
            print(b.toJSON())
    
    if SAVE:
        saving_path = "./saves/biomes_test.txt"
        print("Saving all these biomes to the file '{}'".format(saving_path))
        f = open(saving_path, "w")
        for b in biomes:
            f.write(b.toJSON() + "\n")
        f.close()
        print("Finished saving biomes")
