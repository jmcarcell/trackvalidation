
from DIRAC import S_OK, S_ERROR

class Params(object):

  def __init__(self):
    self.raw = False
    self.pingsToDo = 1

  def setRawResult(self, value):
    self.raw = True
    return S_OK()

  def setNumOfPingsToDo(self, value):
    print("hello")
    try:
      self.pingsToDo = max(1, int(value))
    except ValueError:
      return S_ERROR("Number of pings to do has to be a number")
    return S_OK()
