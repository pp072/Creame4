# Python module for 3D GEO 
# Python module for 3D GEO 
from PythonQt import *


editor = GUIModule.Editor

editor.openScene("D://LocalProjects//3DEngines//Horde3D-master//editor//src//Content//test.scn")

engine = Game.EngineHRD()
g_o = Game.GameObject(engine.findObjectByName("Medved2"))
print g_o.getNodeID()
print g_o.addAnimation("models/Bear/Bear.anim")
g_o.autoSetupStages()
g_o.playAnimation(0)