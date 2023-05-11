import os

SAMPLE = "muon"
MIN_Y = "0.955"
MAX_Y = "1.01"

#fixedPt
PATHFILESIN = "../produce_histo/results/"
PATHFILESOUT = "results/muons/fixedPt/"

if not os.path.exists(PATHFILESOUT):
    os.makedirs(PATHFILESOUT)

FILENAMES = ["histo_1GeV.root", "histo_10GeV.root", "histo_100GeV.root"]
FEATURES = "\"p_{T} = 1 GeV:1:22\" \"p_{T} = 10 GeV:2:21\" \"p_{T} = 100 GeV:4:20\""

HISTONAMES = "eff_vs_theta eff_vs_phi"
AXISTITLES = "\";#theta [#circ];Tracking efficiency\" \";#phi [#circ];Tracking efficiency\""
LOG_X = "False False"

listFilein = ""
for filein in FILENAMES:
    listFilein = listFilein + PATHFILESIN + filein + " "

command_plot = "python plotting.py --filesin %s --folderout %s --histonames %s --varAxes %s --sample %s --features %s"%(listFilein,PATHFILESOUT,HISTONAMES,AXISTITLES,SAMPLE,FEATURES)
command_plot += " --logXaxis %s --rangeYaxis %s:%s %s:%s"%(LOG_X,MIN_Y,MAX_Y,MIN_Y,MAX_Y)
command_plot += " --rangeXaxis %s:%s %s:%s "%("7","90","0","180")

print(command_plot)
os.system(command_plot)

#fixedTheta
PATHFILESIN = "../produce_histo/results/"
PATHFILESOUT = "results/muons/fixedTheta/"

if not os.path.exists(PATHFILESOUT):
    os.makedirs(PATHFILESOUT)

FILENAMES = ["histo_10deg.root", "histo_30deg.root", "histo_89deg.root"]
FEATURES = "\"#theta = 10#circ (forward):1:22\" \"#theta = 30#circ (transition):2:21\" \"#theta = 89#circ (barrel):4:20\""

HISTONAMES = "eff_vs_pt eff_vs_phi"
AXISTITLES = "\";p_{T} [GeV];Tracking efficiency\" \";#phi [#circ];Tracking efficiency\""
LOG_X = "True False"

listFilein = ""
for filein in FILENAMES:
    listFilein = listFilein + PATHFILESIN + filein + " "

command_plot = "python plotting.py --filesin %s --folderout %s --histonames %s --varAxes %s --sample %s --features %s "%(listFilein,PATHFILESOUT,HISTONAMES,AXISTITLES,SAMPLE,FEATURES)
command_plot += " --logXaxis %s --rangeYaxis %s:%s %s:%s"%(LOG_X,MIN_Y,MAX_Y,MIN_Y,MAX_Y)
command_plot += " --rangeXaxis %s:%s %s:%s"%("0.09","250","0","180")

print(command_plot)
os.system(command_plot)
