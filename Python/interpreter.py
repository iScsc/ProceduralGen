import sys


### ---------- Main Methods ---------- ###

def encode(obj : object, indent : int=0) -> str:
    """Encodes a given object in a simili JSON format"""
    
    try:
        obj_class = obj.__class__

        obj_str = ""
        
        #? Match base types 
        if (obj_class in [int, float, complex, list, tuple, str, bytearray, bytes] or obj==None):
            if (indent>0):
                if obj_class==str: obj_str="\""+obj+"\""
                elif obj_class==bytes: obj_str="b'"+obj.hex()+"'"
                elif obj_class==bytearray: obj_str="bytearray("+obj.hex()+")"
                else : obj_str=str(obj)
            else:
                obj_str = "{"
                obj_str += "\n\t__class__: " + str(obj_class).split("'")[1].split(".")[-1]+";"
                if obj_class==str: obj_str += "\n\t__value__: " +"\""+obj+"\"" #TODO forbid char { } ( ) [ ] , ;
                elif obj_class==bytes: obj_str += "\n\t__value__: b'" + obj.hex()+"'"
                elif obj_class==bytearray: obj_str += "\n\t__value__: bytearray("+obj.hex()+")"
                else : obj_str += "\n\t__value__: " + str(obj)
                obj_str += "\n}"
        
        #? If object :
        else :
            obj_str = "{"
            obj_str += "\n\t" + "\t"*indent + "__class__: " + str(obj_class).split("'")[1].split(".")[-1]
            
            obj_dict = obj.__dict__ 
            
            for arg in obj_dict:
                obj_str += ";\n\t" + "\t"*indent + arg + ": " + encode(obj.__getattribute__(arg), indent=indent+1)
                
            obj_str += "\n" + "\t"*indent + "}"
        
        return obj_str
    
    except Exception:
        raise InterpreterError("Couldn't encode object"+obj)


def decode(input_string : str, start:bool = False) -> object:
    """Decodes a given object from its formatted string (if manual use set start as True)"""
    try:
        str_list=spliter(input_string,False,start)
    except (IndexError,TypeError):
        raise InterpreterError("The given string couldn't be parsed into an Arg list")
    str_dict=getDict(str_list)
    try:
        obj_class=getClass(str_dict.pop('__class__'))
        obj=None
        if (obj_class in [int, float, complex]):
            obj=obj_class(str_dict['__value__'])
        elif obj_class==str: obj=obj_class(str_dict['__value__'])[1:-1]
        elif obj_class==list: obj=listFromString(str_dict['__value__'])
        elif obj_class==tuple: obj=tuple(listFromString(str_dict['__value__']))
        elif obj_class==bytes: obj=bytes.fromhex(str_dict['__value__'][2:-1])
        elif obj_class==bytearray: obj=bytearray.fromhex(str_dict['__value__'][10:-1])
        elif obj_class==None: return None
        else:
            obj=object.__new__(obj_class)
            for arg in str_dict:
                obj.__dict__[arg] = decode(str_dict[arg])
        return obj
    except KeyError as e:
        raise DecodeDictError() from e



def decodeList(input_string: str) -> list[object]:
    """Decodes a list of object from its formatted string"""
    object_list=[]
    try:
        str_list=spliter(input_string,True,True)
    except (IndexError,TypeError):
        raise InterpreterError("The given string couldn't be parsed into an object list")
    for object_string in str_list:
        try:
            object_list.append(decode(object_string))
        except InterpreterError as e:
            if EXCEPT: print(e)
            else: raise DecodeListError("Couldn't decode object: "+object_string) from e
    return object_list


def encodeList(object_list: list[object]) -> str:
    """Encodes a list of objects in a simili JSON format"""
    list_string=""
    for object in object_list:
        try:
            list_string+=encode(object)+'\n'
        except InterpreterError as e:
            if EXCEPT: print(e)
            else: raise
    return list_string





### ---------- Auxiliary Methods ---------- ###

def spliter(string:str, start=False, epurate=False):
    """Splits a given string on braces and semi-colons, taking just into acount first level braces.
    For instance: "{{a};b;{c}}" -> ["{a}";"b";{c}]

    Args:
        string (str): The string to split
        start (bool): A boolean indicating if it is the first iteration

    Returns:
        list(str): The list of substrings 
    """
    
    #removes unwanted chars (to simplify parsing) done only once at the start
    forbidden_char = ["\n", "\t"]
    if epurate: 
        for char in forbidden_char:
            string=string.replace(char,'')
    
    #removes first opening brace and last closing brace
    if not start and len(string)>2:
        string=string[1:-1]
        
    #getting brace index
    brace_open=[] #list of opening brace
    brace_close=[] #list of closing brace
    index_open=0 #index of the last opening brace found
    index_close=0 #index of the last closing brace found
    
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
    
    if (len(brace_open)==0): # no brace found: split the string only on semi-colons
        substr+=string.split(';')
        
    elif (brace_open[0]!=0): # take the begining of the string (before the first brace) and split it on semi-colons 
        substr+=(string[0:brace_open[0]]).split(';')
        
    for i in range(len(brace_open)): # for each pair of '{' '}', gets what's inbetween braces included and add what preceded (variable name)
        substr[-1]+=(string[brace_open[i]:brace_close[i]+1])
        if (i<len(brace_open)-1): # gets what's inbetween pairs and split it on semi-colons
            substr+=(string[brace_close[i]+1:brace_open[i+1]]).split(';')
            
    if (len(brace_close)>0 and brace_close[-1]!=len(string)-1): # take the end of the string (after the first brace) and split it on semi-colons
        substr+=(string[brace_close[-1]+1:]).split(';')
        
    #epurating from void substring and unwanted substrings
    substr=[s for s in substr if s not in ['']]
    
    return substr



def getClass(class_arg: str) -> object:
    """Returns the class given by a formatted string: "<class>"
    """
    if class_arg in ['int', 'float', 'complex', 'list', 'tuple', 'str', 'bytearray', 'bytes','None']:
        return eval(class_arg)
    try:
        return getattr(sys.modules[__name__], class_arg)
    except AttributeError as e:
        raise DecodeClassError(class_arg) from e


def getDict(str_list: list[str]) -> dict[str: str]:
    """Returns a dict from a formatted stringlist: ["<key>: <value>",...]"""
    dict={}
    for arg in str_list:
        try:
            key,value=getKeyValue(arg)
            dict[key]=value
        except DecodeArgError as e:
            if EXCEPT: print(e)
            else: raise
    return dict


def getKeyValue(string: str) -> tuple[str,str]:
    """Returns a pair (key, value) from a formatted strong "key: value"."""
    try:
        split_index=string.index(':')
        key=string[:split_index]
        value=string[split_index+2:]
        if value[0] != "{" and key!="__value__" and key!='__class__': #primitive types
            value='{'+getType(value)+';__value__: '+value+'}'
        return key,value
    except (ValueError,IndexError,TypeError) as e:
        raise DecodeArgError(string) from e


def getType(string: str) -> str:
    """Inferes the type of a formatted string and returns "__class__: <class>"."""
    res="__class__: "
    char=string[0]
    if char in ['\'','\"']: return res+'str'
    elif char=='[': return res+'list'
    elif char=='(': return res+'tuple'
    elif char in [str(i) for i in range(10)] or char=='-':
        if string.__contains__("j"): return res+'complex'
        elif string.__contains__("."): return res+'float'
        else: return res+'int'
    elif char=='b':
        if string.__contains__("bytearray"): return res+'bytearray'
        else: return res+'bytes'
    elif char=='N': return res+'None'
    else: raise DecodeTypeError(string)



def listFromString(string: str) -> list:
    """Creates a list from a formatted string: "[a,b,c,d]"->[a,b,c,d]"""
    res=[]
    
    for x in listStringSpliter(string):
        try:
            temp_str = '{__class__: temp;obj: '+x+'}'
            res.append(decode(temp_str).obj)
        except InterpreterError as e:
            if EXCEPT: print(e)
            else: raise DecodeListError("Couldn't decode object from string: "+x) from e
        
    
    return res


def listStringSpliter(string: str) -> list[str]:
    """Splits a string representing a list in its elements"""
    nbrParenthesis=0
    nbrBraket=0
    nbrBrace=0
    
    start_index=1
    end_index=1
    
    res=[]
    try:
        while (end_index<len(string)):
            if string[end_index]==',' and nbrParenthesis==nbrBraket==nbrBrace==0:
                res.append(string[start_index:end_index])
                if string[end_index+1]==' ':
                    end_index+=1
                start_index=end_index+1
            elif string[end_index]=='(': nbrParenthesis+=1
            elif string[end_index]==')': nbrParenthesis-=1
            elif string[end_index]=='[': nbrBraket+=1
            elif string[end_index]==']': nbrBraket-=1
            elif string[end_index]=='{': nbrBrace+=1
            elif string[end_index]=='}': nbrBrace-=1
            end_index+=1
            
        res.append(string[start_index:-1])
    except Exception as e: #Should not be useful for the append method and the indexing cannot raise errors in this conditions
        raise DecodeListError("Could not split list string: "+string) from e
    else:
        res = [x for x in res if x!='']
        return res
    
  
  
    
    
### ---------- Class ---------- ###

class temp:
    """A buffer class, used to simplify some process"""
    def init(self, object=None):
        obj=object





### ---------- Exceptions ---------- ###

EXCEPT=True #if set to True the functions will ignore most of raised exception if possible
class InterpreterError(Exception):
    """Base class for Exceptions specific to the encoding or decoding process."""
    pass
class DecodeListError(InterpreterError):
    """Exceptions occuring during the decoding of a list"""
    pass
class DecodeTypeError(InterpreterError):
    """Occures when a type isn't recognized"""
    def __init__(self,string:str):
        super().__init__("Unrecognized type: "+string)
class DecodeArgError(InterpreterError):
    """Occures when a key-value pair couldn't be infered from a given string"""
    def __init__(self,string:str):
        super().__init__("Undecoded arg: "+string)
class DecodeClassError(InterpreterError):
    """Occures when a non-primitve class couldn't be found in present modules"""
    def __init__(self,string:str):
        super().__init__("Unfound class: "+string)
class DecodeDictError(InterpreterError):
    """Occures when an object dict hasn't been properly initialised"""
    pass






### ---------- Binary encoding methods  ---------- ###

from numpy import uint8, float64

BYTES_TRUE = b'\x01'
BYTES_FALSE = b'\x00'
INT_BITS_NBR = 24 #24 should be a multiple of 8
FLOAT_BITS_EXP = 5 #5 should be < 8
FLOAT_BITS_MANTISS = 18 
FLOAT_BITS_NBR = 1 + FLOAT_BITS_EXP + FLOAT_BITS_MANTISS #24 should be a multiple of 8

BYTES_VERSION = b'\x00'

def bytesNumber(x : int | float | float64 | uint8) -> bytes:
    """Returns the encoding of a given number.

    Args:
        x (int | float | float64 | uint8): the given number

    Returns:
        bytes: the encoding corresponding
    """
    if type(x)==uint8:
        res=bytes([x]) #stored on one byte
        return res
    elif type(x)==int and int.bit_length(x)<INT_BITS_NBR: #can be encoded
        temp = 0 if x>0 else 128 #sign bit
        x = abs(x)
        l=[temp + x // (2**(INT_BITS_NBR-8))] + [x % (2**(INT_BITS_NBR-i)) // (2**(INT_BITS_NBR-i-8)) for i in range(8,INT_BITS_NBR,8)] #[sign bit + first numeral bits to get a byte] + [remaining bytes]
        res = bytes(l)
        return res
    elif type(x)==float or type(x)==float64: 
        temp = 0 if x>0 else 128 #sign bit
        x = abs(x)
        exp = 0
        if x<2**(FLOAT_BITS_MANTISS): #negative exponant
            while exp>-2**(FLOAT_BITS_EXP-1) and x<2**(FLOAT_BITS_MANTISS-1):
                exp-=1
                x*=2
        else: #positive exponant
            while exp<2**(FLOAT_BITS_EXP-1)-1 and x>=2**(FLOAT_BITS_MANTISS):
                exp+=1
                x/=2
        exp+=16 #offsetting exponant
        x=int(x)
        l = [temp + exp*2**(8-(1+FLOAT_BITS_EXP)) + x//2**(FLOAT_BITS_MANTISS-(8-(1+FLOAT_BITS_EXP)))] #sign bit + exponant bits + first numeral bits to get a byte
        #remaining numeral bytes
        l += [x%2**(FLOAT_BITS_MANTISS+(1+FLOAT_BITS_EXP)-i) // (2**(FLOAT_BITS_MANTISS+(1+FLOAT_BITS_EXP)-i-8)) for i in range(8,FLOAT_BITS_MANTISS+(1+FLOAT_BITS_EXP),8)]
        res = bytes(l)
        return res

def nextInt(bytes_str : bytes) -> tuple[int, bytes]:
    """Returns the next int from the first bytes of a given byte string.

    Args:
        bytes_str (bytes): the given byte string

    Returns:
        tuple[int, bytes]: the int and the remaining bytes
    """
    x = int.from_bytes(bytes_str[:INT_BITS_NBR//8]) #easier to work with ints
    if x//2**(INT_BITS_NBR-1)==1: x=-x%2**(INT_BITS_NBR-1) #first bit is a sign bit
    return x,bytes_str[INT_BITS_NBR//8:]

def nextFloat(bytes_str : bytes) -> tuple[float, bytes]:
    """Returns the next float from the first bytes of a given byte string.

    Args:
        bytes_str (bytes): the given byte string

    Returns:
        tuple[float, bytes]: the float and the remaining bytes
    """
    x = int.from_bytes(bytes_str[:FLOAT_BITS_NBR//8]) #easier to work with ints
    if x//(2**(FLOAT_BITS_NBR-1))==1: s=-1 #first bit is a sign bit
    else: s=1
    x=x%(2**(FLOAT_BITS_NBR-1)) #getting rid of first sign bit
    exp=x//(2**(FLOAT_BITS_MANTISS)) #obtaining exponants bits
    exp-=16 #de-offsetting exponant
    x=x%(2**(FLOAT_BITS_MANTISS)) #getting rid of exponants bits
    return s*x*2**exp,bytes_str[FLOAT_BITS_NBR//8:] #reconstructing float number
    
def nextUInt8(bytes_str : bytes) -> tuple[uint8, bytes]:
    """Returns the next uint8 (numpy) from the first byte of a given byte string.

    Args:
        bytes_str (bytes): the byte string

    Returns:
        tuple[uint8, bytes]: the uint8 decoded and the remaining bytes
    """
    x = int.from_bytes(bytes_str[0:1])
    x = uint8(x)
    return x,bytes_str[1:]


def read(data_in: str|bytes) -> tuple[object, bytes]:
    """Decodes an object from a given binary file or string

    Args:
        data_in (str | bytes): either path to file to decode, or bytes to be decoded

    Returns:
        tuple[object, bytes]: the decoded object and eventual residual bytes
    """
    
    #importing objects to be matched
    from layer import Layer
    from gradientGrid import GradientGrid
    from chunk import Chunk
    from map import Map
    from mapGenerator import CompleteMap
    
    #bytes to be decoded
    bytes_str : bytes
    if type(data_in)==str:
        f=open(data_in,'rb')
        bytes_str=f.read()
        if bytes_str[0:1]==BYTES_VERSION:
            bytes_str=bytes_str[1:]
        else: bytes_str==None
        f.close()
    elif type(data_in)==bytes:
        bytes_str=data_in
        if bytes_str[0:1]==BYTES_VERSION:
            bytes_str=bytes_str[1:]
        else: bytes_str==None
    else: bytes_str=None
    
    #decoding bytes
    if bytes_str!=None:
        isList=bytes_str[0:1]==BYTES_TRUE
        bytes_str=bytes_str[1:]
        
        if isList:
            nbr, bytes_str = nextInt(bytes_str) #number of elements in the list
            res = []
            for _ in range(nbr):
                x, bytes_str = read(bytes_str)
                res.append(x)
            return res, bytes_str
        
        elif bytes_str[0:1]==GradientGrid.GRID_ENCODING:
            return GradientGrid.read(None,bytes_str)
        elif bytes_str[0:1]==Layer.LAYER_ENCODING:
            return Layer.read(None,bytes_str)
        elif bytes_str[0:1]==Chunk.CHUNK_ENCODING:
            return Chunk.read(None,bytes_str)
        elif bytes_str[0:1]==Map.MAP_ENCODING:
            return Map.read(None,bytes_str)
        elif bytes_str[0:1]==CompleteMap.COMPLETE_MAP_ENCODING:
            return CompleteMap.read(None,bytes_str)
        
    else: return None #failed

def write(obj: object, path: str=None) -> bytes:
    """Encodes a given object into a binary file or string.

    Args:
        obj (object): the object to be encoded
        path (str, optional): the file path where to store encoded bytes. Defaults to None.

    Returns:
        bytes: encoded bytes
    """
    
    #importing object to match
    from layer import Layer
    from gradientGrid import GradientGrid
    from chunk import Chunk
    from map import Map
    from mapGenerator import CompleteMap
    
    bytes_str: bytes = b''
    bytes_str += BYTES_VERSION
    
    if type(obj)==list or type(obj)==tuple:
        bytes_str += BYTES_TRUE
        bytes_str += bytesNumber(len(obj))
        for x in obj:
            bytes_str += write(x)     
    else: 
        bytes_str += BYTES_FALSE
        
        if type(obj)==GradientGrid:
            bytes_str+=GradientGrid.write(obj)
        elif type(obj)==Layer:
            bytes_str+=Layer.write(obj,True)
        elif type(obj)==Chunk:
            bytes_str+=Chunk.write(obj)
        elif type(obj)==Map:
            bytes_str+=Map.write(obj)
        elif type(obj)==CompleteMap:
            bytes_str+=CompleteMap.write(obj)
            
    if path!=None:
        f=open(path,'wb')
        f.write(bytes_str)
        f.close()
        
    return bytes_str

def readAll(data_in: str|bytes) -> list[object]:
    """Decodes object from a given binary file or string up to bytes depletion.

    Args:
        data_in (str | bytes): path to the file or bytes

    Returns:
        list[object]: all decoded ojects
    """
    
    #bytes to be decoded
    bytes_str : bytes
    if type(data_in)==str:
        f=open(data_in,'rb')
        bytes_str=f.read()
        f.close()
    elif type(data_in)==bytes:
        bytes_str=data_in
    else: return None
    
    #decoding
    res = []
    while len(bytes_str)>0:
        x, bytes_str = read(bytes_str)
        res.append(x)
    return res



if __name__ == "__main__":
    
    # from biome import Biome
    # a = Biome()
    # a.bio = Biome()
    # s = encode(a)
    # print(s)
    # l = spliter(s,False,True)
    # print(l)
    # b = decode(s,True)
    # print(b.name)
    # print(b.bio.altitude_range)
    
    # t = temp()
    # t.obj=bytearray("A",'utf-8')
    
    # print(encode(t))
    # print(decode(encode(t),True).obj)
    
    
    
    from gradientGrid import GradientGrid

    grid = GradientGrid(1, 1)
    print(grid)
    
    print("Writing grid in file")
    write(grid,"../saves/testing.data")
    
    print("Reading map in file")
    gridl=read("../saves/testing.data")[0]
    print(gridl)