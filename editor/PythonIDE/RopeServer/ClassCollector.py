import os
import sys
import pydoc
import inspect
import pickle
import pkgutil

_namesCache = []
_rawItems = []
_cache = []
data = {}

def showClasses(mod, allowUnder = False, path = ""):
    for obj in inspect.getmembers(mod):
        
        if mod.__name__+" "+obj[0] in _cache:
            continue
            
        _cache.append(mod.__name__+" "+obj[0])
        
        ch = True
        if obj[0][0] == "_":
            ch = allowUnder
        
        if ch and inspect.isclass(obj[1]) and obj[1].__module__ == mod.__name__:
            if not mod.__name__ in data:
                data[mod.__name__] = []
            data[mod.__name__].append(obj[0])
            #print(mod.__name__, obj[0])
            
        if inspect.ismodule(obj[1]) and ((path != "" and inspect.getfile(obj[1]).startswith(path)) or path == ""):
            showClasses(obj[1], allowUnder, path)

def process(modname, allowUnder = False, path = ""):
    if modname in _namesCache:
        return
    if modname.startswith("encodings") or \
       modname.startswith("anti") or \
       modname.startswith("lib2to3") or \
       modname.startswith("eric") or \
       modname.startswith("test") or \
       modname[0] == "_":
        return
    try:
        _namesCache.append(modname)
        mod = __import__(modname, globals(), locals(), [], 0)
        if (path != "" and inspect.getfile(mod).startswith(path)) or path == "":
            showClasses(mod, allowUnder, path)
    except:
        return
        
def complete():
    print("len:%s" % len(_rawItems))
    for count, name in enumerate(_rawItems):
        process(name[1])
        sys.stdout.write("count:%s\n" % count)
        sys.stdout.flush()
    
def callback(path, modname, neco):
    _rawItems.append((path, modname))

class NullDevice():
    def write(self, string):
        pass
        
if __name__ == '__main__':
    collectOther(sys.argv[1])
        
def collectOther(path):
    sys.stderr = NullDevice()
    pydoc.ModuleScanner().run(callback, key=".", completer=complete)
    with open(os.path.join(path, "classes"), "wb") as fil:
        pickle.dump(data, fil)

def completeProject(path):
    for count, name in enumerate(_rawItems):
        if name[0] is not None and name[0].startswith(path):
            process(name[1], True, path)

def collectProject(path, dicts):
    global data, _cache
    data = dicts
    for mod, clss in data.items():
        for cls in clss:
            _cache.append(mod+" "+cls)
            
    os.chdir(path)
    sys.path.append(path)
    pydoc.ModuleScanner().run(callback, key=".", completer=lambda tp = path: completeProject(tp))
    return data
    