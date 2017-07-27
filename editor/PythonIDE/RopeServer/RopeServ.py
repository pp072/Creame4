import os
import sys
import pickle
import threading
import SocketServer
sys.path.append('RopeServer')

from  SimpleXMLRPCServer import SimpleXMLRPCDispatcher

from pylint.lint import PyLinter
from pylint import checkers
from pylint.reporters import BaseReporter
from pylint.interfaces import IReporter

import rope.base.project as RopeProject
from rope.contrib.autoimport import AutoImport
import rope.contrib.codeassist
import rope.base.libutils
from rope.base import worder
from LintConfig import createConfig
from rope.contrib.findit import find_definition

import traceback
import socket

global myGlobalisServerSatrt
myGlobalisServerSatrt = True 
#import sys
#sys.path.append(r"D:\eclipse\eclipse\plugins\org.python.pydev.debug_2.5.0.2012050419\pysrc")
#import pydevd

ROPE_PREFS = {'ignore_syntax_errors': True,
              'ignore_bad_imports': True,
              'automatic_soa' : True,
              'automatic_soi' : True,
              'soa_followed_calls' : 1,
              'perform_doa' : True,
              'import_dynload_stdmods' : True,
              'extension_modules': [
                  "PythonQt","PythonQt.QtGui","PySide", "PySide.QtGui","os.path", "numpy", "scipy",
                  "PIL", "OpenGL", "array", "audioop", "binascii", "cPickle",
                  "cStringIO", "cmath", "collections", "datetime", "errno", "exceptions",
                  "gc", "imageop", "imp", "itertools", "marshal", "math", "mmap",
                  "msvcrt", "nt", "operator", "os", "parser", "rgbimg", "signal", "strop",
                  "sys", "thread", "time", "wx", "wxPython", "xxsubtype", "zipimport",
                  "zlib", "multiprocessing", "sys"
              ]}

def run_in_thread(fn):
    def run(*k, **kw):
        try:
            t = threading.Thread(target=fn, args=k, kwargs=kw)
            t.start()
        except:
            pass
    return run
srv = SimpleXMLRPCDispatcher()
ropeProject = None
lint = None
autoimp = None
lock = threading.Lock()


class CheckReporter(BaseReporter):
    __implements____ = IReporter

    def __init__(self):
        BaseReporter.__init__(self, sys.stdout)
        self.messages = []

    def add_message(self, msg_id, location, msg):
        module, object, line, col = location[1:]
        self.messages.append(
                {
                "line" : line,
                "message" : "(%s: %s)" % (msg_id, msg),
                "type" : msg_id
            }
        )

    def finalize(self):
        ret = self.messages
        self.messages = []
        return ret

    def display_results(self, layout):
        """ignore layouts"""


def initProject(path, script, prjFileName, classesPath):
    sys.path.append(path)
    global ropeProject
    global lint

    ropeProject = RopeProject.Project(path, fscommands=None, ropefolder=None, **ROPE_PREFS)
    ropeProject.validate()

    try:
        conf = createConfig(prjFileName)
        os.chdir(path)
        lint = PyLinter(pylintrc = conf, reporter = CheckReporter())
        checkers.initialize(lint)
        lint.read_config_file()
        lint.load_config_file()
        lint.load_configuration()
        lint.quiet = 1
        initImport(path, classesPath)
#        pydevd.settrace("localhost") 
    except:
        sys.stderr.write(traceback.format_exc())
        sys.stderr.flush()

    return True

@run_in_thread
def initImport(path, classesPath):
    global autoimp
    global ropeProject
    from ClassCollector import collectProject

    #dicts = {}
    
    #with open(classesPath, 'rb') as f:
    #    dicts = pickle.load(f)

    #autoimp = AutoImport(ropeProject, names=dicts)

    #dicts = collectProject(path, dicts)
    #autoimp = AutoImport(ropeProject, names=dicts)
   

 

def revalidate(path, script, prjFileName, classesPath):
    global ropeProject
    global lint

    ropeProject = RopeProject.Project(path, fscommands=None, ropefolder=None, **ROPE_PREFS)
    ropeProject.validate()

    os.chdir(path)
    conf = createConfig(prjFileName)
    lint = PyLinter(pylintrc = conf, reporter = CheckReporter())
    checkers.initialize(lint)
    lint.read_config_file()
    lint.load_config_file()
    lint.load_configuration()
    lint.quiet = 1

    initImport(path, classesPath)

def errorsList(file):
    global lock
    global lint
    with lock:
        try:
            lint.check(file, True)
            return lint.reporter.finalize()
        except:
            sys.stderr.write(traceback.format_exc())
            sys.stderr.flush()
        return []

def autocompletitionList(file, text, position):
    global ropeProject
    if ropeProject is None:
        return []

    ropeProject.validate()
    try:
        try:
            resource = rope.base.libutils.path_to_resource(ropeProject, file)
        except:
            resource = None

        proposals = rope.contrib.codeassist.code_assist(ropeProject, text, int(position), resource)
        proposals = rope.contrib.codeassist.sorted_proposals(proposals)
        list = ["%s|%s|%s" % (proposal.name, proposal.type, proposal.scope) for proposal in proposals] #("("+", ".join(proposal.parameters)+")" if proposal.type == 'function' else "")
        it = worder.Worder(text, True).get_splitted_primary_before(int(position))
    except:
        sys.stderr.write(traceback.format_exc())
        sys.stderr.flush()
        return "", []
    return it[1], list

def findImplementation(file, text, position):
    global ropeProject
    if ropeProject is None:
        return "", 0

    try:
        resource = rope.base.libutils.path_to_resource(ropeProject, file)

        defin = find_definition(ropeProject, text, int(position), resource)
        if defin is not None and defin.resource is not None:
            return defin.resource.real_path, defin.offset
    except:
        sys.stderr.write(traceback.format_exc())
        sys.stderr.flush()

    return "", 0

def classNames(prefix):
    return [it[0]+":"+it[1] for it in autoimp.import_assist(prefix)]

def stopServ():
    print "rope server stoping..."
    global myGlobalisServerSatrt
    myGlobalisServerSatrt = False
srv.register_function(stopServ, "stopServ")
srv.register_function(initProject, "initProject")
srv.register_function(errorsList, "errorsList")
srv.register_function(revalidate, "revalidate")
srv.register_function(autocompletitionList, "autocompletitionList")
srv.register_function(findImplementation, "findImplementation")
srv.register_function(classNames, "classNames")

class ThreadedTCPServer(SocketServer.ThreadingMixIn, SocketServer.TCPServer):
    pass

import struct

class ThreadedTCPRequestHandler(SocketServer.BaseRequestHandler):
    def handle(self):
        global srv
        block = self.request.recv(1024)
        req = block
        while len(block) == 1024:
            block = self.request.recv(1024)
            req += block
        data = srv._marshaled_dispatch(req.decode("utf-8"))

        self.request.send(struct.pack('L', len(data)))
        totalsent = 0
        while totalsent < len(data):
            sent = self.request.send(data[totalsent:])
            if sent == 0:
                sys.stderr.write("Error send")
                sys.stderr.flush()
            totalsent = totalsent + sent

            #sys.stderr.write("write %s" % totalsent)
            #sys.stderr.flush()

if __name__ == "__main__":
    server = ThreadedTCPServer(("localhost", 0), ThreadedTCPRequestHandler)
    ip, port = server.server_address
    sys.stdout.write("ropeserstart %s %s \n" % (ip, port))
    sys.stdout.flush()

    server_thread = threading.Thread(target=server.serve_forever)
    server_thread.setDaemon(True)
    server_thread.start()
    import time 
    
    while(myGlobalisServerSatrt):
        1==1
    server.shutdown()
    print "ropeserstpd\n"