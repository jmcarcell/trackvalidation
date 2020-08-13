import os

YEAR = "2020"
RELEASE = "ILCSoft-2020-02-07"
DETECTOR = "CLICo3v14"
PATHFILESIN = "/eos/experiment/clicdp/grid/ilc/user/e/ericabro/CLIC/%s/%s/%s/efficiencies/"%(YEAR,DETECTOR,RELEASE)
PATHFILESOUT = "results/"

if not os.path.exists(PATHFILESOUT):
    os.makedirs(PATHFILESOUT)

#In case of debugging
FILENAMES=["merged_reco_muons_pt_1GeV.root"]
ROOTNAMES=["histo.root"]
#FILENAMES=["merged_reco_muons_pt_1GeV.root", "merged_reco_muons_pt_10GeV.root", "merged_reco_muons_pt_100GeV.root",
#           "merged_Pow1_muons_10deg.root", "merged_Pow1_muons_30deg.root", "merged_Pow1_muons_89deg.root"]
#ROOTNAMES=["histo_1GeV.root", "histo_10GeV.root", "histo_100GeV.root",
#           "histo_10deg.root", "histo_30deg.root", "histo_89deg.root"]

#efficiency
ANALYSIS_FLAGS = "--efficiency"

LISTVAR_FIXED_THETA    = "pt:100:-2:3:True phi:180:-180:180:False"
CUTS_FIXED_THETA       = "vertexR:0:0.0001:nHitsMC:3:1000 vertexR:0:0.0001:nHitsMC:3:1000"

LISTVAR_FIXED_PT       = "theta:83:7:90:False phi:180:-180:180:False"
CUTS_FIXED_PT          = "vertexR:0:0.0001:nHitsMC:3:1000 vertexR:0:0.0001:nHitsMC:3:1000"

for filein, fileout in zip(FILENAMES,ROOTNAMES):
  filein = PATHFILESIN + filein
  fileout = PATHFILESOUT + fileout
  command_analysis = ""
  if "GeV" in filein : 
    command_analysis = "python analysis.py --filein %s --fileout %s --listVariables %s --listSelections %s"%(filein,fileout,LISTVAR_FIXED_PT,CUTS_FIXED_PT)
  elif "deg" in filein :
    command_analysis = "python analysis.py --filein %s --fileout %s --listVariables %s --listSelections %s"%(filein,fileout,LISTVAR_FIXED_THETA,CUTS_FIXED_THETA)
  else :
    print("Not sure which variable list to use. Please double check the name of the file.")
    continue

  command_analysis += " %s"%(ANALYSIS_FLAGS)
  print(command_analysis)
  os.system(command_analysis)

#comparison and fakerate
ANALYSIS_FLAGS = "--compareRecoSim --fakerate"

CUTS_FIXED_THETA       = "nHits:3:1000 nHits:3:1000"
CUTS_FIXED_PT          = "nHits:3:1000 nHits:3:1000"

for filein, fileout in zip(FILENAMES,ROOTNAMES):
  filein = PATHFILESIN + filein
  fileout = PATHFILESOUT + fileout
  command_analysis = ""
  if "GeV" in filein :
    command_analysis = "python analysis.py --filein %s --fileout %s --listVariables %s --listSelections %s"%(filein,fileout,LISTVAR_FIXED_PT,CUTS_FIXED_PT)
  elif "deg" in filein :
    command_analysis = "python analysis.py --filein %s --fileout %s --listVariables %s --listSelections %s"%(filein,fileout,LISTVAR_FIXED_THETA,CUTS_FIXED_THETA)
  else :
    print("Not sure which variable list to use. Please double check the name of the file.")
    continue

  command_analysis += " %s"%(ANALYSIS_FLAGS)
  print(command_analysis)
  os.system(command_analysis)
