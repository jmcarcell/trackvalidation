import os

YEAR = "2020"
RELEASE = "ILCSoft-2020-02-07"
#YEAR = "2019"
#RELEASE = "ILCSoft-2019-09-04"
DETECTOR = "CLICo3v14"
PATHFILESIN = "/eos/experiment/clicdp/grid/ilc/user/e/ericabro/CLIC/%s/%s/%s/efficiencies/"%(YEAR,DETECTOR,RELEASE)
PATHFILESOUT = "results/"

if not os.path.exists(PATHFILESOUT):
    os.makedirs(PATHFILESOUT)

FILENAMES=["merged_ttbar3TeV_NoOverlay_conformal.root", "merged_ttbar3TeV_Overlay_conformal.root"]
ROOTNAMES=["histo_ttbar3TeV_NoOverlay_conformal.root", "histo_ttbar3TeV_Overlay_conformal.root"]

#efficiency
ANALYSIS_FLAGS = "--efficiency"

LISTVAR = "theta:83:7:90:False pt:100:-1:3:True phi:180:-180:180:False"
CUTS    = "pt:1:10000:vertexR:-1:50:purity:0.75:1.0:dist:0.02:1000:nHitsMC:4:1000 theta:10:170:vertexR:-1:50:purity:0.75:1.0:dist:0.02:1000:nHitsMC:4:1000 theta:10:170:pt:1:10000:vertexR:-1:50:purity:0.75:1.0:dist:0.02:1000:nHitsMC:4:1000"

for filein, fileout in zip(FILENAMES,ROOTNAMES):
    filein = PATHFILESIN + filein
    fileout = PATHFILESOUT + fileout
    command_analysis = ""
    command_analysis = "python analysis.py --filein %s --fileout %s --listVariables %s --listSelections %s"%(filein,fileout,LISTVAR,CUTS)

    command_analysis += " %s"%(ANALYSIS_FLAGS)
    print(command_analysis)
    os.system(command_analysis)

#fakes
ANALYSIS_FLAGS = "--compareRecoSim --fakerate"
LISTVAR = "theta:83:7:90:False pt:100:-1:3:True phi:180:-180:180:False"
CUTS    = "pt:1:10000:nHits:4:1000 theta:10:170:nHits:4:1000 theta:10:170:pt:1:10000:nHits:4:1000"

for filein, fileout in zip(FILENAMES,ROOTNAMES):
    filein = PATHFILESIN + filein
    fileout = PATHFILESOUT + fileout
    command_analysis = ""
    command_analysis = "python analysis.py --filein %s --fileout %s --listVariables %s --listSelections %s"%(filein,fileout,LISTVAR,CUTS)

    command_analysis += " %s"%(ANALYSIS_FLAGS)
    print(command_analysis)
    os.system(command_analysis)
