
def encode(object : object, indent : int=0) -> str:
    
    obj_class = object.__class__
    
    obj_str = "{"
    obj_str += "\n\t__class__: " + str(obj_class).split("'")[1].split(".")[-1]
    
    
    #? Match base types
    
    obj_type = type(object)
    
    if (obj_type in [int, float, complex, list, tuple, str, bytearray, bytes]):
        obj_str += "\n\tvalue: " + str(object)
    
    
    #? If object :
    
    else :
        obj_dict = object.__dict__ 
           
        for arg in obj_dict:
            obj_str += ",\n\t" + arg + ": " + str(object.__getattribute__(arg))
    
    
    obj_str += "\n}"
    
    return obj_str


def decode(input_string : str) -> object:
    pass
