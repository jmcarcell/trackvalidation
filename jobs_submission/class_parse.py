
from DIRAC import S_OK, S_ERROR

class Params(object):

  def __init__(self):
    self.particle = ""
    self.theta = ""
    self.release = ""
    self.detector = ""
    self.njobs = ""
    self.nev = ""
    self.group = ""
    self.lib = ""

  def setParticle(self, value):
    print("helloPart")
    try:
      self.particle = str(value)
    except ValueError:
      return S_ERROR("Particles has to be a string")
    return S_OK()

  def setTheta(self, value):
    print("helloThet")
    try:
      self.theta = str(value)
    except ValueError:
      return S_ERROR("Theta angle has to be a string")
    return S_OK()

  def setRelease(self, value):
    print("helloRel")
    try:
      self.release = str(value)
    except ValueError:
      return S_ERROR("Reease name has to be a string")
    return S_OK()

  def setDetector(self, value):
    print("helloDetect")
    try:
      self.detector = str(value)
    except ValueError:
      return S_ERROR("Detector name has to be a string")
    return S_OK()

  def setNJobs(self, value):
    print("helloNJObs")
    try:
      self.njobs = int(value)
    except ValueError:
      return S_ERROR("NJobs has to be an int")
    return S_OK()

  def setNEvents(self, value):
    print("helloNEv")
    try:
      self.nev = int(value)
    except ValueError:
      return S_ERROR("Num events has to be an int")
    return S_OK()

  def setGroup(self, value):
    print("helloGrou")
    try:
      self.group = str(value)
    except ValueError:
      return S_ERROR("Group has to be a string")
    return S_OK()

  def setLibrary(self, value):
    print("helloLib")
    try:
      self.lib = str(value)
    except ValueError:
      return S_ERROR("Library has to be a string")
    return S_OK()


