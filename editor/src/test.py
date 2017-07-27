import SmartBody 
from SmartBody import *
scene = SmartBody.getScene()
c = scene.createPawn("mypawn7")
print type(c)
vec = SrVec(0, 100, 40)
c.setPosition(vec)
print "sdfsdf"