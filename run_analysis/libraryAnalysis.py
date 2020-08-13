from math import *
from array import *

def BinLogX(histo, bins):
  '''Re-binning X-axis of TH1 for log-scale'''
  axis = histo.GetXaxis()
  bins_log = histo.GetNbinsX()

  from_axis = axis.GetXmin()
  to_axis   = axis.GetXmax()
  width = (to_axis - from_axis)/bins_log;
  new_bins = array('d')

  for single_bin in range(0,bins+1):
    new_bin = pow(10, from_axis + single_bin*width)
    #print(new_bin)
    new_bins.append(new_bin)
  
  axis.Set(bins,new_bins)
  
  return

def translateVariable(variable_name, treeId):
  '''Translate variable in the correspondent name in the tree'''
  var_treeName = "None"
  var_list = []
  if treeId is "simpl":
    var_list = [ 'm_signal', 'm_type', 'm_pt', 'm_theta', 'm_phi', 'm_vertexR', 'm_distClosestMCPart', 'm_purity', 'm_nHitsMC']
  elif treeId is "perfReco":
    var_list = [ 'recoPt', 'recoTheta', 'recoPhi', 'recoD0', 'recoZ0', 'recoP', 'recoNhits', 'recoChi2OverNDF', 'recoPurity']
  elif treeId is "perfTrue":
    var_list = [ 'trueTheta', 'truePt', 'truePhi', 'trueD0', 'trueZ0', 'trueP', 'trueID', 'trueVertexR']
  elif treeId is "purTrack":
    var_list = [ 'trk_nhits_vtx', 'trk_nhits_trk', 'trk_nhits', 'trk_purity']
  elif treeId is "purMC":
    var_list = [ 'innermostR', 'mc_pdg', 'mc_theta', 'mc_phi', 'mc_p']
  else:
    print("Tree id UNKNOWN! Impossible to convert variables.")

  for v in var_list :
    if variable_name.lower() in v.lower():
      var_treeName = v
  #print("Translated %s in %s for this tree."%(variable_name,var_treeName))
    
  return var_treeName

 
