
from DIRAC import S_OK, S_ERROR

class Params(object):

  def __init__(self):
    self.particle = ""
    self.theta = ""
    self.energy = ""
    self.pt = ""
    self.release = ""
    self.detector = ""
    self.njobs = ""
    self.nev = ""
    self.group = ""
    self.lib = ""
    self.sim_folder = ""

  def setParticle(self, value):
    try:
      self.particle = str(value)
    except ValueError:
      return S_ERROR("Particles has to be a string")
    return S_OK()

  def setTheta(self, value):
    try:
      self.theta = str(value)
    except ValueError:
      return S_ERROR("Theta angle has to be a string")
    return S_OK()

  def setEnergy(self, value):
    try:
      self.energy = str(value)
    except ValueError:
      return S_ERROR("Energy has to be a string")
    return S_OK()

  def setPt(self, value):
    try:
      self.pt = str(value)
    except ValueError:
      return S_ERROR("Pt has to be a string")
    return S_OK()

  def setRelease(self, value):
    try:
      self.release = str(value)
    except ValueError:
      return S_ERROR("Reease name has to be a string")
    return S_OK()

  def setDetector(self, value):
    try:
      self.detector = str(value)
    except ValueError:
      return S_ERROR("Detector name has to be a string")
    return S_OK()

  def setNJobs(self, value):
    try:
      self.njobs = int(value)
    except ValueError:
      return S_ERROR("NJobs has to be an int")
    return S_OK()

  def setNEvents(self, value):
    try:
      self.nev = int(value)
    except ValueError:
      return S_ERROR("Num events has to be an int")
    return S_OK()

  def setGroup(self, value):
    try:
      self.group = str(value)
    except ValueError:
      return S_ERROR("Group has to be a string")
    return S_OK()

  def setLibrary(self, value):
    try:
      self.lib = str(value)
    except ValueError:
      return S_ERROR("Library has to be a string")
    return S_OK()

  def setSIMFolder(self, value):
    try:
      self.sim_folder = str(value)
    except ValueError:
      return S_ERROR("Library has to be a string")
    return S_OK()


