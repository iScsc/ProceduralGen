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


def spliter(string:str, start=False):
    """Splits a given string on braces and commas, taking just into acount first level braces.
    For instance: "{{a},b,{c}}" -> ["{a}","b",{c}]

    Args:
        string (str): The string to split
        start (bool): A boolean indicating if it is the first iteration

    Returns:
        list(str): The list of substrings 
    """
    
    #getting brace index
    
    brace_open=[] #list of opening brace
    brace_close=[] #list of closing brace
    index_open=0 #index of the last opening brace found
    index_close=0 #index of the last closing brace found
    
    #removes unwanted chars (to simplify parsing) done only once at the start
    forbidden_char = ["\n", "\t"]
    if start: 
        for char in forbidden_char:
            string=string.replace(char,'')
    
    #removes first opening brace and last closing brace
    elif len(string)>2:
        string=string[1:-1]
    
    while index_open!=-1 and index_close!=-1: # While an opening and closing brace have been found
        index_open=string.find('{',index_open) # find the next  parethesis index starting from the last one
        index_close=string.find('}',index_close)
        if (index_open!=-1 and index_close!=-1): # If an opening and closing brace have been found
            if (len(brace_close)>0 and index_open<brace_close[-1]): # If the situation is "...{...{...}...}..." the new opening brace is a lower level one
                brace_close[-1]=index_close # last closing brace was a lower level one (inside other brace) and should not be taken into account
                index_close+=1
                index_open+=1
            else : # first level brace
                brace_open.append(index_open)
                index_open+=1
                brace_close.append(index_close)
                index_close+=1
    
    #getting substrings
    
    substr=[] # list of substrings to be returned
    
    if (len(brace_open)==0): # no brace found: split the string only on commas
        substr+=string.split(',')
        
    elif (brace_open[0]!=0): # take the begining of the string (before the first brace) and split it on commas 
        substr+=(string[0:brace_open[0]]).split(',')
        
    for i in range(len(brace_open)): # for each pair of '(' ')', gets what's inbetween
        substr.append(string[brace_open[i]:brace_close[i]+1])
        if (i<len(brace_open)-1): # gets what's inbetween pairs and split it on commas
            substr+=(string[brace_close[i]+1:brace_open[i+1]]).split(',')
            
    if (len(brace_close)>0 and brace_close[-1]!=len(string)-1): # take the end of the string (after the first brace) and split it on commas
        substr+=(string[brace_close[-1]+1:]).split(',')
        
    #epurating from void substring and unwanted substrings
    substr=[s for s in substr if s not in ['']]
    
    return substr
