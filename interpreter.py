
def encode(object : object) -> str:
    
    obj_class = object.__class__
    
    #? Match base types
    
    
    #? If object :
    
    obj_dict = object.__dict__
    
    
    obj_str = "{"
    obj_str += "\n\t__class__: " + str(obj_class).split("'")[1].split(".")[-1]
    
    
    for arg in obj_dict:
        obj_str += ",\n\t" + arg + ": " + str(object.__getattribute__(arg))
    
    
    obj_str += "\n}"
    
    return obj_str


def decode(input_string : str) -> object:
    pass
