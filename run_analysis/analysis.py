import os
import json
import argparse

from libraryAnalysis import *

import ROOT
ROOT.PyConfig.IgnoreCommandLineOptions = True

from ROOT import *

ROOT.gROOT.LoadMacro("root_macros.C")

gROOT.SetBatch()

parser = argparse.ArgumentParser(description='Produce root files with tracking analysis plots')
parser.add_argument('--filein', help='Name file input')
parser.add_argument('--fileout', help='Name file output', default="histo.root")
parser.add_argument('--efficiency', help='Perform analysis of tracking efficiency', action='store_true')
parser.add_argument('--fakerate', help='Perform analysis of tracking fakerate', action='store_true')
parser.add_argument('--duplicates', help='Perform analysis of tracking duplicates', action='store_true')
parser.add_argument('--compareRecoSim', help='Compare Simulated vs Reconstructed distrib', action='store_true')
parser.add_argument('--listVariables', nargs='+', help='List of variables and correspondent axis specs (ex. var:nbins:min:max:logXaxis var2:nbins2:min2:max2:logXaxis2)')
parser.add_argument('--listSelections', nargs='+', help='List of selections for each variable (ex. var:min:max:var2:min2:max2)')
parser.add_argument('-v', '--verbose', help="increase output verbosity", action="store_true")
args = parser.parse_args()

VERBOSE = args.verbose
if VERBOSE : print(args)

EFFICIENCY = args.efficiency
RECOVSSIM = args.compareRecoSim
FAKERATE = args.fakerate
DUPLICATES = args.duplicates

NAMENTUPLESFILE = args.filein
OUTPUTFILE = args.fileout

TREENAME      = "MyClicEfficiencyCalculator/"
TREENAME_EFF  = TREENAME+"simplifiedEfficiencyTree"
TREENAME_DIST = TREENAME+"perfTree"
TREENAME_FAKE = TREENAME+"perfTree"
TREENAME_DUPL = TREENAME+"perfTree"

VARIABLES = args.listVariables
if not VARIABLES:
  VARIABLES = []
  print("listVariables not filled!")


SELECTIONS = args.listSelections
if not SELECTIONS:
  SELECTIONS = [""] * len(VARIABLES)

def create_selections(custom, treeId, fixed=''):

  """Return the customised selections per variable and the fixed selections in one string which can be used directly in the correspondent TTree
  
  Input parameteres:
  custom (string): customised selection - must be in the form var:min:max
  treeId (string): tree identifier necessary to translate the variable in the correspondent branch name 
  fixed  (string): additional selection

  """

  finalCut = ''
  #add selection on variables
  #the input must be a string with varName:min:max
  if custom != "":
    varDetails = custom.split(":")
    for i in range(0, len(varDetails), 3):
      realName = translateVariable(varDetails[i], treeId)
      if realName is "None":
        print("Variable \'%s\' for the tree \'%s\' unkown."%(varDetails[i],treeId))
      else:
        if finalCut != "":
          finalCut += " && "
        finalCut += " %s >= %s && %s <= %s "%(realName, varDetails[i+1], realName, varDetails[i+2])
  #add fix selection
  if fixed!= '':
    if finalCut != "":
      finalCut += " && "
    finalCut += fixed

  return finalCut

def calculate_efficiency(inputTree, cut, treeId, variable = "theta", nbins = 100, minbin = 0.0, maxbin = 100, isLog = False):

  """Write tracking efficiency as function of a single variable on an already open TFile
  Efficiency = #Reconstructed track associated to a reconstructable MC Particle / #Reconstructable MC Particles
  
  Input parameteres:
  inputTree     (TTree): ROOT TTree used as input
  cut           (string): selections applied to both reconstucted and reconstructable
  treeId        (string): tree identifier necessary to translate the variable in the correspondent branch name
  variable      (string): variable to plot against
  nbins         (int): Number of bins for TEfficiency x-axis in output
  minbin&maxbin (float): Min and max value for TEfficiency x-axis in output 
  isLog         (bool): Set log scale on TEfficiency x-axis in output
  """

  realName = translateVariable(variable, treeId)
  if VERBOSE :
    print(">> Eff for variable %s (min=%.1f,max=%.1f,bins=%d,log=%d)"%(variable,minbin,maxbin,nbins,isLog))
    print("   Selections applied: %s"%(cut))
  cutReconstructable = cut
  cutReconstructed   = str("%s && m_reconstructed == 1 "%(cut))

  hreconstructable = TH1F("hreconstructable","hreconstructable",nbins,minbin,maxbin)
  hreconstructable.Sumw2()
  hreconstructed = TH1F("hreconstructed","hreconstructed",nbins,minbin,maxbin)
  hreconstructed.Sumw2()
  if isLog == True :
    BinLogX(hreconstructable, nbins)
    BinLogX(hreconstructed, nbins)

  inputTree.Draw(realName+">>hreconstructable",cutReconstructable)
  inputTree.Draw(realName+">>hreconstructed",cutReconstructed)

  heff = TEfficiency(hreconstructed, hreconstructable)
  heffName = "eff_vs_"+variable
  if VERBOSE :
    print("   Total reconstructed: %.f"%hreconstructed.GetEntries())
    print("   Total reconstructable: %.f"%hreconstructable.GetEntries())
  
  heff.SetName(heffName)
  heff.SetDirectory(0)
  heff.Write()
  return 

def save_distribution(treeId, inputTree, cut, variable = "theta", nbins = 100, minbin = 0.0, maxbin = 100, isLog = False):
 
  """Write variable distribution on an already open TFile
  
  Input parameteres:
  inputTree     (TTree): ROOT TTree used as input
  cut           (string): selections applied to both reconstucted and reconstructable
  treeId        (string): tree identifier necessary to translate the variable in the correspondent branch name
  variable      (string): variable to plot against
  nbins         (int): Number of bins for TH1F x-axis in output
  minbin&maxbin (float): Min and max value for TH1F x-axis in output 
  isLog         (bool): Set log scale on TH1F x-axis in output
  """

  if VERBOSE :
    print(">> Saving variable %s (min=%.1f,max=%.1f,bins=%d,log=%d)"%(variable,minbin,maxbin,nbins,isLog))
    print("   Selections applied: %s"%(cut))

  h_distr = TH1F("h_distr","h_distr",nbins,minbin,maxbin)
  h_distr.Sumw2()
  if isLog == True :
    BinLogX(h_distr, nbins)

  realName = translateVariable(variable, treeId)

  inputTree.Draw(realName+">>h_distr",cut)
  #MC particle phi is shifted w.r.t. the reco track phi
  if variable == "phi" and "true" in realName.lower():
    if not cut :
      cut = "truePhi>180"
    else:
      cut = cut + "&& truePhi>180"
    h_distr2 = TH1F("h_distr2","h_distr2",nbins,minbin,maxbin)
    h_distr2.Sumw2()
    inputTree.Draw(realName+" - 360 >> h_distr2",cut)
    h_distr.Add(h_distr2)

  if "true" in realName.lower():
    hName = "sim_" + variable
  elif "reco" in realName.lower():
    hName = "reco_" + variable
  else: 
    hName = variable
  h_distr.SetName(hName)
  h_distr.SetTitle(hName)
  if VERBOSE :
    print("Total : %.f"%h_distr.GetEntries())

  h_distr.Write()
  return

def calculate_fakerate(inputTree, cut, treeId_var, treeId_pur, purityMax = "0.75", variable = "theta", nbins = 100, minbin = 0.0, maxbin = 100, isLog = False):

  """Write tracking fakerate as function of a single variable on an already open TFile
  Fake rate = #Tracks with purity < purityMax / #Reconstructed tracks
  
  Input parameteres:
  inputTree     (TTree): ROOT TTree used as input
  cut           (string): selections applied to both reconstucted and reconstructable
  treeId_var    (string): tree identifier necessary to translate the variable in the correspondent branch name
  treeId_pur    (string): tree identifier necessary to translate the purity variable in the correspondent branch name
  purityMax     (string): purityMax to set a cut in the definition of fake track
  variable      (string): variable to plot against
  nbins         (int): Number of bins for TEfficiency x-axis in output
  minbin&maxbin (float): Min and max value for TEfficiency x-axis in output 
  isLog         (bool): Set log scale on TEfficiency x-axis in output
  """

  realName = translateVariable(variable, treeId_var)
  purityName = translateVariable("pur", treeId_pur)
  cutReco = cut
  if cut != "":
    cut += " && "
  cutFake = str("%s %s < %s "%(cut, purityName, purityMax))

  if VERBOSE :
    print(">> Fakes for variable %s (min=%.1f,max=%.1f,bins=%d,log=%d)"%(variable,minbin,maxbin,nbins,isLog))
    print("   Selections applied: %s"%(cut))
    print("   Selections to cut fakes: %s"%(cutFake))

  hReco = TH1F("hReco","hReco",nbins,minbin,maxbin)
  hReco.Sumw2()
  hFake = TH1F("hFake","hFake",nbins,minbin,maxbin)
  hFake.Sumw2()
  if isLog == True :
    BinLogX(hReco, nbins)
    BinLogX(hFake, nbins)

  inputTree.Draw(realName+">>hReco",cutReco)
  inputTree.Draw(realName+">>hFake",cutFake)

  hfake = TEfficiency(hFake, hReco)
  hfakeName = "fake_vs_"+variable
  if VERBOSE :
    print("   Total Fake: %.f"%hFake.GetEntries())
    print("   Total Reco: %.f"%hReco.GetEntries())

  hfake.SetName(hfakeName)
  hfake.SetDirectory(0)
  hfake.Write()
  return

def calculate_duplicates(inputTree, minHits, treeId, variable = "theta", nbins = 100, minbin = 0.0, maxbin = 100, isLog = False):

  if VERBOSE :
    print(">> Duplicates for variable %s (min=%.1f,max=%.1f,bins=%d,log=%d)"%(variable,minbin,maxbin,nbins,isLog))
    print('   For duplicates, only num hits is used! min nHits = %d'%minHits)

  realName = translateVariable(variable, treeId)
  hReco = TH1F("hReco","hReco",nbins,minbin,maxbin)
  hReco.Sumw2()
  hDupl = TH1F("hDupl","hDupl",nbins,minbin,maxbin)
  hDupl.Sumw2()
  if isLog == True :
    BinLogX(hReco, nbins)
    BinLogX(hDupl, nbins)

  ROOT.fill_duplicates_histos(inputTree, realName, hReco, hDupl, minHits)
  if VERBOSE :
    print("   Total Dupl: %.f"%hDupl.GetEntries())
    print("   Total Reco: %.f"%hReco.GetEntries())

  hdupl = TEfficiency(hDupl, hReco)
  hduplName = "dupl_vs_"+variable

  hdupl.SetName(hduplName)
  hdupl.SetDirectory(0)
  hdupl.Write()
  return

def main():
  print("Analysis performed on file: %s"%NAMENTUPLESFILE)
  inputTFile = TFile(NAMENTUPLESFILE)
  outputTFile = TFile(OUTPUTFILE,"UPDATE")

  if EFFICIENCY :
  #if os.path.exists(OUTPUTFILE):
  #  recreate = raw_input("%s already exists. Do you want to recreate the file [y/n]?"%OUTPUTFILE)
  #  if recreate is "y":
  #    outputTFile = TFile(OUTPUTFILE,"RECREATE")
  #  elif recreate is "n":
  #    outputTFile = TFile(OUTPUTFILE,"UPDATE")
  #  else:
  #    print("Answer not valid. Break.")
  #    return
  #else:
  #  outputTFile = TFile(OUTPUTFILE,"CREATE")
    if VERBOSE :
      print("> Producing efficiency plots ...")
    rootTTree = inputTFile.Get(TREENAME_EFF)
    treeId = "simpl"

    for var,cutCustom in zip(VARIABLES,SELECTIONS):
      #Create selection string
      #Currently there is no fixed selection on the genStatus of the MC Particle
      #but only particles belonging to the signal are considered
      cuts = "m_signal == 1"
      cuts = create_selections(cutCustom, treeId, cuts) 

      coord = var.split(":")
      calculate_efficiency(rootTTree, cuts, treeId, coord[0], int(coord[1]), float(coord[2]), float(coord[3]), json.loads(coord[4].lower()))

  if RECOVSSIM:
    if VERBOSE :
      print("> Saving distribution of reco/sim track ...")
    rootTTree = inputTFile.Get(TREENAME_DIST)

    for var,cutCustom in zip(VARIABLES,SELECTIONS):
      #Create selection string
      cutsTrue = create_selections(cutCustom, "perfTrue", "") 
      cutsReco = create_selections(cutCustom, "perfReco", "") 

      coord = var.split(":")
      save_distribution("perfTrue", rootTTree, cutsTrue, coord[0], int(coord[1]), float(coord[2]), float(coord[3]), json.loads(coord[4].lower()))
      save_distribution("perfReco", rootTTree, cutsReco, coord[0], int(coord[1]), float(coord[2]), float(coord[3]), json.loads(coord[4].lower()))

  if FAKERATE :
    if VERBOSE :
      print("> Producing fakerate plots ...")
    rootTTree = inputTFile.Get(TREENAME_FAKE)
    branchesNames = [b.GetName() for b in rootTTree.GetListOfBranches()]
    if VERBOSE :  print(" perfTree has the following branches:%s"%branchesNames)

    fakeTree = TTree()
    treeId_var = "perfReco"
    treeId_pur = "perfReco"

    if "recoPurity" not in branchesNames :
      if VERBOSE :
        print(" Tree has NO purity, using a second tree!")
      rootTTree2 = inputTFile.Get(TREENAME+"puritytree")
      if rootTTree2.GetEntries() != rootTTree.GetEntries() :
        print("ERROR: the two trees have not the same number of entries")
        return
      else:
        if VERBOSE :  print(" Total number of entries = %s"%str(rootTTree2.GetEntries()))

      mergedTTree = rootTTree.CloneTree(0)
      mergedTTree.CopyEntries(rootTTree)
      mergedTTree.AddFriend(TREENAME+"puritytree", NAMENTUPLESFILE)

      fakeTTree = mergedTTree
      treeId_pur = "purTrack"

    else:
      fakeTTree = rootTTree

    for var,cutCustom in zip(VARIABLES,SELECTIONS):
      #Create selection string
      cuts = create_selections(cutCustom, treeId_var, "") 

      coord = var.split(":")
      calculate_fakerate(fakeTTree, cuts, treeId_var, treeId_pur, "0.75", coord[0], int(coord[1]), float(coord[2]), float(coord[3]), json.loads(coord[4].lower()))
   
    
  if DUPLICATES :
    if VERBOSE :
      print("> Producing duplicates plots ...")
    rootTTree = inputTFile.Get(TREENAME_DUPL)
    treeId = "perfReco"

    for var,cutCustom in zip(VARIABLES,SELECTIONS):
      #For duplicates, only num hits is used
      minHits = 0
      varDetails = cutCustom.split(":")
      for i in range(0, len(varDetails), 3):
        realName = translateVariable(varDetails[i], treeId)
        if realName is 'recoNhits':
          minHits = int(varDetails[i+1])
      
      coord = var.split(":")
      calculate_duplicates(rootTTree, minHits, treeId, coord[0], int(coord[1]), float(coord[2]), float(coord[3]), json.loads(coord[4].lower()))


  outputTFile.Close()
  print("Analysis saved in: %s"%OUTPUTFILE)


if __name__ == "__main__":
    main()
