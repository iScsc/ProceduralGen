import sys

def encode(object : object, indent : int=0) -> str:
    
    obj_class = object.__class__
    
    obj_str = ""
    
    #? Match base types
    
    if (obj_class in [int, float, complex, list, tuple, str, bytearray, bytes] or object==None):
        if (indent>0):
            obj_str=str(object)
        else:
            obj_str = "{"
            obj_str += "\n\t__class__: " + str(obj_class).split("'")[1].split(".")[-1]
            obj_str += "\n\tvalue: " + str(object)
            obj_str += "\n}"
    
    
    #? If object :
    
    else :
        obj_str = "{"
        obj_str += "\n\t" + "\t"*indent + "__class__: " + str(obj_class).split("'")[1].split(".")[-1]
        
        obj_dict = object.__dict__ 
           
        for arg in obj_dict:
            obj_str += ",\n\t" + "\t"*indent + arg + ": " + encode(object.__getattribute__(arg), indent=indent+1)
            
        obj_str += "\n" + "\t"*indent + "}"
    
    
    
    return obj_str


def decode(input_string : str) -> object:
    str_class=""
    str_dict={}
    obj_class=getattr(sys.modules[__name__], str_class)
    object=None
    if (obj_class in [int, float, complex, list, tuple, str, bytearray, bytes]):
        pass
    else:
        object=object.__new__(obj_class)
        for arg in str_dict:
            object.__dict__[arg] = decode(str_dict[arg])
    return object
