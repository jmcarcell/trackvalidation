import os

SAMPLE = "ttbar3TeV"

PATHFILESIN = "../produce_histo/results/"
PATHFILESOUT = "results_new/ttbar/"

if not os.path.exists(PATHFILESOUT):
    os.makedirs(PATHFILESOUT)

FILENAMES = ["histo_ttbar3TeV_NoOverlay_conformal.root", "histo_ttbar3TeV_Overlay_conformal.root"]
FEATURES = "\"No background:4:20\" \"3 TeV #gamma#gamma#rightarrow hadrons background:2:21\""
NHITS = "4"

HISTONAMES = "eff_vs_theta eff_vs_phi eff_vs_pt"
AXISTITLES = "\";#theta [#circ];Tracking efficiency\" \";#phi [#circ];Tracking efficiency\" \";p_{T} [GeV];Tracking efficiency\""
LOG_X = "False False True"
LOG_Y = "False False False"

listFilein = ""
for filein in FILENAMES:
  listFilein = listFilein + PATHFILESIN + filein + " "

command_plot = "python plotting.py --filesin %s --folderout %s --histonames %s --varAxes %s --sample %s --features %s --minHits %s"%(listFilein,PATHFILESOUT,HISTONAMES,AXISTITLES,SAMPLE,FEATURES,NHITS)
command_plot += " --logXaxis %s --logYaxis %s --rangeYaxis %s:%s %s:%s %s:%s"%(LOG_X,LOG_Y,"0.9","1.01","0.9","1.01","0.7","1.05")
command_plot += " --rangeXaxis %s:%s %s:%s %s:%s"%("7","90","-180","180","0.09","250")

print(command_plot)
os.system(command_plot)

#fakes
FILENAMES = ["histo_ttbar3TeV_NoOverlay_conformal_fakes.root", "histo_ttbar3TeV_Overlay_conformal_fakes.root"]
HISTONAMES = "fake_vs_theta fake_vs_phi fake_vs_pt"
AXISTITLES = "\";#theta [#circ];Tracking fakerate\" \";#phi [#circ];Tracking fakerate\" \";p_{T} [GeV];Tracking fakerate\""
LOG_Y = "True True True"

listFilein = ""
for filein in FILENAMES:
  listFilein = listFilein + PATHFILESIN + filein + " "

command_plot = "python plotting.py --filesin %s --folderout %s --histonames %s --varAxes %s --sample %s --features %s --minHits %s"%(listFilein,PATHFILESOUT,HISTONAMES,AXISTITLES,SAMPLE,FEATURES,NHITS)
command_plot += " --logXaxis %s --logYaxis %s --rangeYaxis %s:%s %s:%s %s:%s"%(LOG_X,LOG_Y,"1E-4","0.9","1E-4","0.9","1E-4","0.9")
command_plot += " --rangeXaxis %s:%s %s:%s %s:%s"%("7","90","-180","180","0.09","250")

print(command_plot)
os.system(command_plot)
