import ConfigParser
import tempfile




def strip(value):
    if value[0] == '"':
        return value[1:-1]
    return value

def createConfig(prjFile):
    prj = ConfigParser.ConfigParser()
    prj.read(prjFile)
#    import sys
#    sys.path.append(r"D:\eclipse\eclipse\plugins\org.python.pydev.debug_2.5.0.2012050419\pysrc")
#    import pydevd
    
    #if not "lint" in prj:
    #prj["lint"] = {}
    
    out = ConfigParser.ConfigParser()
    out.add_section('MASTER')
    out.set('MASTER', "profile" , "no")
    out.set('MASTER', "ignore" , "CVS, .git, .svn")
    out.set('MASTER', "persistent" , "yes")
    out.set('MASTER', "load-plugins" , "")
    # out["MASTER"] = {
        # "profile"      : "no",
        # "ignore"       : "CVS, .git, .svn",
        # "persistent"   : "yes",
        # "load-plugins" : "",
    # }
    
    enable = ""
    disable = "C0111"
    
    out.add_section("MESSAGES CONTROL")
    out.set("MESSAGES CONTROL",  "enable" if enable else "#enable", enable)
    out.set("MESSAGES CONTROL", "disable" if disable else "#disable", disable)
    
#    out["MESSAGES CONTROL"] = {
#        "enable" if enable else "#enable"    : enable,
#        "disable" if disable else "#disable" : disable
#    }

    
    out.add_section("REPORTS")
    out.set("REPORTS",  "include-ids"  , "no")
    out.set("REPORTS", "files-output" , "no")
    out.set("REPORTS", "reports"      , "no")
    out.set("REPORTS", "evaluation"   , "10.0 - ((float(5 * error + warning + refactor + convention) / statement) * 10)")
    out.set("REPORTS", "comment"      , "no")
    
#    out["REPORTS"] = {
#        "include-ids"  : "no",
#        "files-output" : "no",
#        "reports"      : "no",
#        "evaluation"   : "10.0 - ((float(5 * error + warning + refactor + convention) / statement) * 10)",
#        "comment"      : "no",
#    }

    out.add_section("SIMILARITIES")
    out.set("SIMILARITIES",  "min-similarity-lines" , "4")
    out.set("SIMILARITIES", "ignore-comments"      , "yes")
    out.set("SIMILARITIES", "ignore-docstrings"    , "yes")
    
#    out["SIMILARITIES"] = {
#        "min-similarity-lines" : "4",
#        "ignore-comments"      : "yes",
#        "ignore-docstrings"    : "yes",
#    }

    
    out.add_section("VARIABLES")
    out.set("VARIABLES",  "init-import"         , "no")
    out.set("VARIABLES", "dummy-variables-rgx" , "_|dummy")
    out.set("VARIABLES", "additional-builtins" , "PySide.PythonQt_QtAll, PySide.QtGui")
    
    
#    out["VARIABLES"] = {
#        "init-import"         : "no",
#        "dummy-variables-rgx" : prj["lint"].get("dummyVar", "_|dummy"),
#        "additional-builtins" : "PySide.PythonQt_QtAll, PySide.QtGui",
#    }

    out.add_section("MISCELLANEOUS")
    out.set("MISCELLANEOUS",  "notes" , "FIXME, XXX, TODO")
    #out.set("VARIABLES", "dummy-variables-rgx" , prj["lint"].get("dummyVar", "_|dummy"))
    
#    out["MISCELLANEOUS"] = {
#        "notes" : "FIXME, XXX, TODO",
#    }

    out.add_section("TYPECHECK")
    out.set("TYPECHECK",  "ignore-mixin-members" , "yes")
    out.set("TYPECHECK",  "ignored-classes"      , "SQLObject")
    out.set("TYPECHECK",  "zope"                 , "no")
    out.set("TYPECHECK",  "generated-members"    , "REQUEST, acl_users, aq_parent")
    
#    out["TYPECHECK"] = {
#        "ignore-mixin-members" : "yes",
#        "ignored-classes"      : "SQLObject",
#        "zope"                 : "no",
#        "generated-members"    : "REQUEST, acl_users, aq_parent"
#    }
    
    
    out.add_section("BASIC")
    out.set("BASIC",  "required-attributes" , "")
    out.set("BASIC",  "bad-functions", "map, filter, apply, input")
    out.set("BASIC",  "module-rgx", "(([a-z_][a-z0-9_]*)|([A-Z][a-zA-Z0-9]+))$")
    out.set("BASIC",  "const-rgx", "(([A-Z_][A-Z0-9_]*)|(__.*__))$")
    out.set("BASIC",  "class-rgx", "[A-Z_][a-zA-Z0-9]+$")
    out.set("BASIC",  "function-rgx", "[a-z_][a-z0-9_]{2,30}$")
    out.set("BASIC",  "method-rgx", "[a-z_][a-z0-9_]{2,30}$")
    out.set("BASIC",  "attr-rgx", "[a-z_][a-z0-9_]{2,30}$")
    out.set("BASIC",  "argument-rgx", "[a-z_][a-z0-9_]{2,30}$")
    out.set("BASIC",  "variable-rgx", "[a-z_][a-z0-9_]{2,30}$")
    out.set("BASIC",  "inlinevar-rgx", "[A-Za-z_][A-Za-z0-9_]*$")
    out.set("BASIC",  "good-names", "i, j, k, ex, Run, _")
    out.set("BASIC",  "bad-names", "foo, bar, baz")
    out.set("BASIC",  "no-docstring-rgx", "__.*__")
    
#    out["BASIC"] = {
#        "required-attributes" : "",
#        "bad-functions"       : prj["lint"].get("badFunc", "map, filter, apply, input"),
#        "module-rgx"          : strip(prj["lint"].get("checkModule", "(([a-z_][a-z0-9_]*)|([A-Z][a-zA-Z0-9]+))$")),
#        "const-rgx"           : strip(prj["lint"].get("checkConst", "(([A-Z_][A-Z0-9_]*)|(__.*__))$")),
#        "class-rgx"           : strip(prj["lint"].get("checkClass", "[A-Z_][a-zA-Z0-9]+$")),
#        "function-rgx"        : strip(prj["lint"].get("checkFunc", "[a-z_][a-z0-9_]{2,30}$")),
#        "method-rgx"          : strip(prj["lint"].get("checkMethod", "[a-z_][a-z0-9_]{2,30}$")),
#        "attr-rgx"            : strip(prj["lint"].get("checkAttr", "[a-z_][a-z0-9_]{2,30}$")),
#        "argument-rgx"        : strip(prj["lint"].get("checkArg", "[a-z_][a-z0-9_]{2,30}$")),
#        "variable-rgx"        : strip(prj["lint"].get("checkVar", "[a-z_][a-z0-9_]{2,30}$")),
#        "inlinevar-rgx"       : strip(prj["lint"].get("checkInlineVar", "[A-Za-z_][A-Za-z0-9_]*$")),
#        "good-names"          : strip(prj["lint"].get("goodNames", "i, j, k, ex, Run, _")),
#        "bad-names"           : strip(prj["lint"].get("badNames", "foo, bar, baz")),
#        "no-docstring-rgx"    : "__.*__"
#    }
    
    out.add_section("FORMAT")
    out.set("FORMAT",  "maxLineLength" , "80")
    out.set("FORMAT",  "maxModLines"      , "1000")
    out.set("FORMAT",  "indent-string"    , "'    '")
    
#    out["FORMAT"] = {
#        "max-line-length"  : prj["lint"].get("maxLineLength", "80"),
#        "max-module-lines" : prj["lint"].get("maxModLines", "1000"),
#        "indent-string"    : "'    '"
#    }

    out.add_section("IMPORTS")
    out.set("IMPORTS",  "deprecated-modules" , "regsub, string, TERMIOS, Bastion, rexec")
    out.set("IMPORTS",  "import-graph"       , "")
    out.set("IMPORTS",  "ext-import-graph"   , "")
    out.set("IMPORTS",  "int-import-graph"   , "")
   
#    out["IMPORTS"] = {
#        "deprecated-modules" : "regsub, string, TERMIOS, Bastion, rexec",
#        "import-graph"       : "",
#        "ext-import-graph"   : "",
#        "int-import-graph"   : ""
#    }

    out.add_section("DESIGN")
    out.set("DESIGN",  "max-args", "5")
    out.set("DESIGN",  "ignored-argument-names", "_.*")
    out.set("DESIGN",  "max-locals", "15")
    out.set("DESIGN",  "max-returns", "6")
    out.set("DESIGN",  "max-branchs", "12")
    out.set("DESIGN",  "max-statements", "50")
    out.set("DESIGN",  "max-parents", "7")
    out.set("DESIGN",  "max-attributes", "7")
    out.set("DESIGN",  "min-public-methods", "2")
    out.set("DESIGN",  "max-public-methods", "20")

    
#    out["DESIGN"] = {
#        "max-args"               : prj["lint"].get("maxArgs", "5"),
#        "ignored-argument-names" : prj["lint"].get("ignArgNames", "_.*"),
#        "max-locals"             : prj["lint"].get("maxLocals", "15"),
#        "max-returns"            : prj["lint"].get("maxReturns", "6"),
#        "max-branchs"            : prj["lint"].get("maxBranchs", "12"),
#        "max-statements"         : prj["lint"].get("maxStats", "50"),
#        "max-parents"            : prj["lint"].get("maxParents", "7"),
#        "max-attributes"         : prj["lint"].get("maxAttrs", "7"),
#        "min-public-methods"     : prj["lint"].get("minPublic", "2"),
#        "max-public-methods"     : prj["lint"].get("maxPublic", "20"),
#    }

    
    out.add_section("CLASSES")
    out.set("CLASSES",  "ignore-iface-methods"        , "isImplementedBy, deferred, extends, names, namesAndDescriptions, queryDescriptionFor, getBases, getDescriptionFor, getDoc, getName, getTaggedValue, getTaggedValueTags, isEqualOrExtendedBy, setTaggedValue, isImplementedByInstancesOf, adaptWith, is_implemented_by")
    out.set("CLASSES",  "defining-attr-methods"       , "__init__, __new__, setUp")
    out.set("CLASSES",  "valid-classmethod-first-arg" , "cls")

    
#    out["CLASSES"] = {
#        "ignore-iface-methods"        : "isImplementedBy, deferred, extends, names, namesAndDescriptions, queryDescriptionFor, getBases, getDescriptionFor, getDoc, getName, getTaggedValue, getTaggedValueTags, isEqualOrExtendedBy, setTaggedValue, isImplementedByInstancesOf, adaptWith, is_implemented_by",
#        "defining-attr-methods"       : "__init__, __new__, setUp",
#        "valid-classmethod-first-arg" : "cls",
#    }

    out.add_section("EXCEPTIONS")
    out.set("EXCEPTIONS",  "overgeneral-exceptions" , "Exception")
    
#    out["EXCEPTIONS"] = {
#        "overgeneral-exceptions" : "Exception",
#    }

    name = ""
    with tempfile.NamedTemporaryFile(mode='w+t', delete=False) as configfile:
        out.write(configfile)
        name = configfile.name
        
    return name
    
#createConfig("/home/jes/workspace/pynotosup/pynoto.pynotoprj")